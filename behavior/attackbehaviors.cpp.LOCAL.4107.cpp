#include <algorithm>
#include "model/gamemodel.h"
#include "skill/driveball.h"
#include "skill/kicktopoint.h"
#include "include/config/team.h"
#include "include/config/globals.h"
#include "utilities/measurments.h"
#include "utilities/region.h"
#include "strategy/attackstrategy.h"


/************************************************
 ******** Attack Behaviors Configuration ********
 ************************************************/
#define ATTACK_BEHV_DEBUG 0

/* Defines the RoboCup's maximum distance a single robot can drive
 * the ball before a penalty is called
 */
#define LEGAL_DRIVE_DIST   500
/* Defines a "safe" drive distance, such that, due to vision errors
 * and estimation, the robot will stop driving before actually reaching
 * the maximum allowed drive distance. Default 75% legal drive distance
 */
#define SAFE_DRIVE_DIST    (LEGAL_DRIVE_DIST*0.75)
/* Defines default (regular) distance blocker will stay away
 * from driver robot
 */
#define BLOCK_DIST_DEFAULT 650
/* Defines how far the blockers will move out when the driver is
 * about to kick the ball
 */
#define BLOCK_DIST_OUTER   1500
/* Defines, for the blocker robots, what the angle difference is
 * that each robot moves out from one another around the driver
 */
#define BLOCK_ANGLE_DIFF   (M_PI / 6)
/* Defines the number of calls to perform on the blocker robots that
 * must be done in order to begin moving to the ner robot
 * with the ball.
 */
#define BLOCK_WAIT_COUNT   350
/* Defines, for the receiving attacker robot, how many calls to
 * findMostStrategicRegion must be made for a new most strategic region
 * is calculated.
 */
#define RECV_UPDATE_COUNT  250
/* Defines, for the receiving attacker robot, the number of regions to
 * divide the top and lower half of the opponent side of the field into.
 * So the total number of regions considered is 2x this
 */
#define RECV_NUM_REGIONS   2
/* Defines the radius that AttackDefendGoal robots will stay away from the
 * goal then forming a line against the goal, moving to get in front of
 * the ball
 */
#define DEFEND_GOAL_RADIUS 400
/* Defines the absolute separation that each AttackDefendGoal robot
 * will have from one another when forming a line on the goal. This
 * should be at least 2*ROBOT_RADIUS
 */
#define DEFEND_GOAL_SEP    (2*ROBOT_RADIUS + 75)


/************************************************
 * AttackDriver Behavior
 ************************************************/

AttackDriver::AttackDriver(const ParameterList& list)
    : state(Initial)
    , current_skill(nullptr)
    , recieveAttackBot(NULL)
    , hasReachedBall(false)
    , hasKicked(false)
{
    try {
        this->recieveAttackBot = list.getParam<Robot*>("recieveBot");
    }
    catch(std::exception& e) {
        std::string msg = "AttackDriver missing ParameterList args: "
                + std::string(e.what());
        throw std::runtime_error(msg.c_str());
    }
}


AttackDriver::~AttackDriver()
{
    delete current_skill;
}


void AttackDriver::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();
    Point robPoint = robot->getRobotPosition();
    Point goal = gm->getOpponentGoal();
    Point ballPoint = gm->getBallPoint();

    switch(state)
    {
    case Initial:
        driveStartPoint = ballPoint;
        hasReachedBall  = false;
        hasKicked = false;
        if(Measurments::distance(driveStartPoint, goal) > LEGAL_DRIVE_DIST*5) {
            float ballGoalAng = Measurments::angleBetween(ballPoint, goal);
            driveEndPoint = driveStartPoint
                    + Point(SAFE_DRIVE_DIST * cos(ballGoalAng),
                            SAFE_DRIVE_DIST * sin(ballGoalAng));
            current_skill = new Skill::DriveBall(driveEndPoint, ballGoalAng);
            state = Driving;
        } else {
            current_skill = new Skill::KickToPoint(goal, SAFE_DRIVE_DIST);
            state = KTPing;
        }
        break;
    case Driving:
        if(Measurments::isClose(robPoint, driveEndPoint, 250)) {
            delete current_skill;
            Point passTarget = recieveAttackBot->getRobotPosition();
            current_skill = new Skill::KickToPoint(passTarget);
            state = Passing;
        }
        break;
    case KTPing:
        /* There's nothing to do here but let the skill perform.
         * But I want it here as an explicitly different state than
         * Driving
         */
        break;
    case Passing:
        /* Again, there is nothing special do to here. A possibility is
         * to add exit conditions 
         */
        break;
    }

    if(!hasReachedBall &&
        Measurments::isClose(robPoint, ballPoint, SAFE_DRIVE_DIST*0.50)) {
        hasReachedBall = true;
    }

    bool status = current_skill->perform(robot);
    if(state == KTPing || state == Passing)
        hasKicked = status;
}


bool AttackDriver::isAboutToKick(Robot* robot)
{
    if(hasReachedBall && (state == Driving || state == Passing)) {
        Point robPos = robot->getRobotPosition();
        return Measurments::distance(robPos, driveStartPoint) > SAFE_DRIVE_DIST*0.60;
    }
    return false;
}


bool AttackDriver::hasDoneKick()
{
    return hasKicked;
}




/************************************************
 * AttackReceiver Behavior
 ************************************************/
AttackReceiver::AttackReceiver(const ParameterList& list)
    : driveAttackBot(NULL)
    , isMoveOk(true)
{
    try {
        this->driveAttackBot = list.getParam<Robot*>("driveBot");
    }
    catch(std::exception& e) {
        std::string msg = "AttackReceiver missing ParameterList args: "
                + std::string(e.what());
        throw std::runtime_error(msg.c_str());
    }
}


void AttackReceiver::perform(Robot* robot)
{
    Point robPos = robot->getRobotPosition();
    Point drvPos = driveAttackBot->getRobotPosition();
    float robDriverAng = Measurments::angleBetween(robPos, drvPos);
    robot->setDrible(true);

    if(isMoveOk) {
        Region* best = findMostStrategicRegion(robot);
        setMovementTargets(best->centre(), robDriverAng, true);
    } else {
        setMovementTargets(robPos, robDriverAng, false);
    }
    GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
}


void AttackReceiver::setMoveStatus(bool canMove)
{
    isMoveOk = canMove;
}


Region* AttackReceiver::findMostStrategicRegion(Robot* robot)
{
    static Region lastMostStrategic;
    static int    regionUpdateCount = 0;

    if(--regionUpdateCount <= 0) {
        /* Time to calculate a new lastMostStrategic Region.
         * Since, at the time of writing, Region isn't widely utilized,
         * there are no standard regions set up across the field. Here,
         * I generate a bunch of them from the centre to the opponent goal
         * and find the best.
         */
        const int REGION_LENGTH = (FIELD_LENGTH_2 / RECV_NUM_REGIONS);
    #if TEAM == TEAM_BLUE
         const int SIGN = 1;
    #else
         const int SIGN = -1;
    #endif
        std::array<Region, RECV_NUM_REGIONS*2> regions;

        /* We're repeatedly defining two regions that look
         * like this sweeping from centre field to the opponent goal:
         *      topY1
         *      X---X
         * minX | 1 | maxX
         *      X---X botY1 = topY2 = 0
         *      | 2 |
         *      X---X
         *      botY2
         */

        for(int i = 0; i != RECV_NUM_REGIONS*2; i+=2)
        {
            int minX  = 0 + SIGN * REGION_LENGTH * i;
            int maxX  = minX + SIGN * REGION_LENGTH;
            int topY1 = 0 + FIELD_WIDTH_2;
            int botY1 = 0;
            int topY2 = 0;
            int botY2 = 0 - FIELD_WIDTH_2;

            regions[i+0] = Region(minX, maxX, topY1, botY1);
            regions[i+1] = Region(minX, maxX, topY2, botY2);
            findStrategicValue(regions[i+0], robot);
            findStrategicValue(regions[i+1], robot);
        }

        static auto maxStratValueFn = [](Region& a, Region& b) {
            return a.getStrategicValue() < b.getStrategicValue();
            };
        lastMostStrategic = *std::max_element
            (regions.begin(), regions.end(), maxStratValueFn);

        regionUpdateCount = RECV_UPDATE_COUNT;    //Reset count
    }

    /* Returning a pointer to a static variable is OK */
    return &lastMostStrategic;
}


void AttackReceiver::findStrategicValue(Region& r, Robot* receiver)
{
    /* For this behavior, the region's strategic value is based on
     * three factors (Descending in order of importance):
     * 1) The number of enemy robots
     * 2) The closeness to the opponent goal
     * 3) The distance from the current position of the robot
     */
    Point robPos = receiver->getRobotPosition();
    Point opGoal = GameModel::getModel()->getOpponentGoal();
    int   numOpRobots   = r.numOfOpponents();
    float goalCloseness = Measurments::distance(r.centre(), opGoal) / FIELD_LENGTH_2;
    float robCloseness  = Measurments::distance(r.centre(), robPos) / FIELD_LENGTH_2;
    r.setStrategicValue( ceil(numOpRobots + goalCloseness*2 + robCloseness) );
}




/************************************************
 * AttackBlocker Behavior
 ************************************************/
int AttackBlocker::numAttackBlockers = 0;
int AttackBlocker::nextSide = 1;
 
AttackBlocker::AttackBlocker(const ParameterList& list)
    : theDriverRobot(NULL)
    , isWaitingCount(0)
    , curDistance(BLOCK_DIST_DEFAULT)
{
#if 1
    this->theDriverRobot = list.getParam<Robot*>("driverRobot");
#endif
    ++numAttackBlockers;
    nextSide *= -1;
    myBlockerNum = floor((float)numAttackBlockers/2);
    //myBlockerNum = numAttackBlockers;
    mySide = nextSide;
}


AttackBlocker::~AttackBlocker()
{
    --numAttackBlockers;
}


void AttackBlocker::perform(Robot *robot)
{
    if(isWaitingCount >= BLOCK_WAIT_COUNT) {
        GameModel* gm   = GameModel::getModel();
        Point robPos    = robot->getRobotPosition();
        Point driverPos = theDriverRobot->getRobotPosition();
        Point opGoalPos = gm->getOpponentGoal();
        float drvrGoalAng = Measurments::angleBetween(driverPos, opGoalPos);
        float robGoalDist = Measurments::distance(robPos, opGoalPos) - ROBOT_SIZE*3;
        float actualDist  = std::min(robGoalDist, curDistance);
        float myOffsetAng = drvrGoalAng + myBlockerNum * mySide * BLOCK_ANGLE_DIFF;

        Point myTargetPoint =
            driverPos + Point(cos(myOffsetAng), sin(myOffsetAng)) * actualDist;

        setMovementTargets(myTargetPoint, 0, true);
        setVelocityMultiplier(1.5);
        GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
    }
    else {
        ++isWaitingCount;
    }
}


void AttackBlocker::moveAwayFromDriver()
{
    curDistance = BLOCK_DIST_OUTER;
}




/************************************************
 * AttackDefendGoal Behavior
 ************************************************/
/* TODO: When the ball dist from goal is <= DEFEND_GOAL_RADIUS,
 * they should do something. Or, when it is <= 3*DEFEND_GOAL_RADIUS,
 * one should possibly move out to kick it back.
 * >>We need to store the ball's velocity.
 */
int AttackDefendGoal::numGoalDefenders = 0;
int AttackDefendGoal::nextSide = -1;
 
 
AttackDefendGoal::AttackDefendGoal(const ParameterList& list)
{
    UNUSED_PARAM(list);
    ++numGoalDefenders;    //Add length to next position
    nextSide *= -1;        //Swap sides
    myGoalDefenderSpot = numGoalDefenders;
    mySide = nextSide;
}


AttackDefendGoal::~AttackDefendGoal()
{
    --numGoalDefenders;
#if ATTACK_BEHV_DEBUG
    if(numGoalDefenders < 0) {
        throw std::runtime_error("Number of AttackDefendGoal was < 0!");
    }
#endif
}


void AttackDefendGoal::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();
    Point ballPoint   = gm->getBallPoint();
    Point myGoalPoint = gm->getMyGoal();
    float goalBallAng = Measurments::angleBetween(myGoalPoint, ballPoint);

    Point middlePoint = myGoalPoint
        + Point(DEFEND_GOAL_RADIUS * cos(goalBallAng),
                DEFEND_GOAL_RADIUS * sin(goalBallAng));

    Point myTargetPoint =
        middlePoint + Point(cos(goalBallAng + M_PI_2), sin(goalBallAng + M_PI_2))
            * mySide * DEFEND_GOAL_SEP;

    if(!Measurments::isClose(robot->getRobotPosition(), myTargetPoint)) {
        setMovementTargets(myTargetPoint, goalBallAng, true);
    } else {
        setMovementTargets(myTargetPoint, goalBallAng, false);
    }

    GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
}

