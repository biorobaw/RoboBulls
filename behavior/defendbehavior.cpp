#include "include/config/simulated.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "utilities/region/rectangle.h"
#include "utilities/region/defencearea.h"

#define DEFENDBEHAVIOR_DEBUG 0

/* Check to see if kicking is done or not.
 * It happens when the ball has high velocity that is not facing
 * our goal. This means one of us kicked.
 */
static bool ballIsMovingAway()
{
    Point bp = gameModel->getBallPoint();
    Point bv = gameModel->getBallVelocity();
    float bs = gameModel->getBallSpeed();
    float ba = atan2(bv.y, bv.x);
    float ballGoalAng = Measurements::angleBetween(bp, gameModel->getMyGoal());
    return (bs > 0.25) && !(Measurements::isClose(ba, ballGoalAng, 90*(M_PI/180)));
}

/* Return true when we think the ball is stopped or moving slow
 */
static bool ballIsStopped()
{
    return gameModel->getBallSpeed() < 0.25;
}

/************************************************************/

//Points claimed for robots to sit at (can move with ball being near)
Point DefendState::defendPoints[10];

//Index into `defendPoints` indexed by robot ID for bots to move to.
int DefendState::claimed[10];

//ID of who is currently moving to kick the ball or -1 if none.
int DefendState::kicker_ID = -1;

//Used to not have DefendState's base `action` run too often
int DefendState::updateCount = 0;

//Points to sit at at ball angle of 0 (and when not on our side)
const Point DefendState::defaultPoints[] = {
    Point(-1500,    0),
    Point(-2000,  500),
    Point(-2000, -500),
    Point(-2500, 1000),
    Point(-2500,-1000)
};

void DefendState::clearClaimedPoints()
{
    for(Point& p : defendPoints)
        p = Point(0,0);
    for(int& i : claimed)
        i = -1;
}

DefendState* DefendState::action(Robot* robot)
{
    (void)(robot);

    //Because every robot is going to call this; but it only
    //needs to happen once per game loop.
    if(++updateCount < 5)
        return nullptr;
    updateCount = 0;

    Point bp = gameModel->getBallPoint();
    Point gl = gameModel->getMyGoal();

    if(bp.x < 0)
    {
        /* If the ball is on our side, we make the robots sway, while
         * still in formation, to face the ball. These are the coefficients
         * for multiplying cos and sin by for changing from goal.
         * o_coeffs are the angle offsets, for sideways formation.
         * o is the angle difference betwween two adjacent robots.
         *
         * dAngle is the differencein angle each robot sits from the adjacent one.
         * Small means they sit more tightly packed together
         *
         * Multiplying X by `opSide` makes the addition to the goal point
         * always point torwards the middle. (Not really though)
         */
        static const float dAngle = (15 + 10*!SIMULATED) * (M_PI/180);
        static int coeffs[] = {1500, 1300, 1300, 1100, 1100};
        static int o_coeffs[] = {0, 1, -1,  2, -2};
        float a = Measurements::angleBetween(gl, bp);

        for(int i = 0; i != 5; ++i)
        {
            Point offset;
            offset.x = coeffs[i] * cos(a + dAngle * o_coeffs[i]);
            offset.y = coeffs[i] * sin(a + dAngle * o_coeffs[i]);
            defendPoints[i] = gl +  offset;
        }
    }
    else {
        for(int i = 0; i != 5; ++i) {
             defendPoints[i] = defaultPoints[i];
        }
    }

    return nullptr;
}

Point* DefendState::getClaimedPoint(Robot* robot)
{
    int index = claimed[robot->getID()];
    if(index != -1)
        return &defendPoints[index];
    return nullptr;
}

void DefendState::setupClaimedPoints()
{
    updateCount = 0;
    clearClaimedPoints();
    for(int i = 0; i != 5; ++i) {
        defendPoints[i] = defaultPoints[i];
    }
}

Point* DefendState::searchClaimPoint(Robot* robot)
{
    for(int i = 0; i != 10; ++i)
    {
        // Here, we check the `claimed` array to check if any
        // slots contain the point we want to claim. If nobody is
        // pointing there, we claim it.
        Point& p = defendPoints[i];
        bool alreadyClaimed = false;
        for(int k : claimed) {
            if(k == -1)
                continue;
            if(defendPoints[k] == p) {
                alreadyClaimed = true;
                break;
            }
        }
        if(alreadyClaimed) {
            continue;
        }

        // Here we ensure there is no other robot there physically
        Point test(p.x, p.y);
        Robot* closest = Comparisons::distance(test).minMyTeam();
        if(closest->getID() != robot->getID()
            && Measurements::distance(test, closest) < ROBOT_RADIUS) {
            continue;
        }
        claimed[robot->getID()] = i;
        break;
    }

    return getClaimedPoint(robot);
}

DefendState::~DefendState() {

}

/************************************************************/

#if SIMULATED
 #define LINE_DISTANCE 400  //Distance ball must be to robot to move to kick
 #define GOALIE_DIST   800  //Distance ball must be away from goal to invervene
#else
 #define LINE_DISTANCE 1200
 #define GOALIE_DIST   300
#endif

DSIdle::DSIdle()
{
#if DEFENDBEHAVIOR_DEBUG
    std::cout << "DefendStateIdle Created" << std::endl;
#endif
}

DefendState* DSIdle::action(Robot* robot)
{
//    std::cout << "DefendStateIdle" << std::endl;

    // Base class's `action` updates claimed points to rotate the formation
    // to face the ball (does not return anything)
    DefendState::action(robot);

    robot->setDribble(false);

    if(getClaimedPoint(robot) == nullptr)
        searchClaimPoint(robot);
    else {
        //Idle at the point, facing the ball
        Point* chosenPointPtr = getClaimedPoint(robot);

        if(chosenPointPtr == nullptr)
            abort();

        Point chosenPoint = *chosenPointPtr;

        float robBallAng = Measurements::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(chosenPoint, robBallAng);
        GenericMovementBehavior::perform(robot);

        /* If the ball is coming to us, and we are certain, we want to kick the ball.
         * Conditions checked are:
         * 1) The ball's predicted X is on our side of the field
         * 2) The ball-to-predicion line distance to robot is small
         * 3) The balls speed is significant
         * 4) the ball is heading torwards the goal
         */
        Point bp  = gameModel->getBallPoint();
        Point bv  = gameModel->getBallVelocity();
        Point bpr = gameModel->getBallStopPoint();
        Point gl  = gameModel->getMyGoal();
        float bs  = gameModel->getBallSpeed();
        float velang = atan2(bv.y, bv.x);

        if( ( abs(bpr.x - gl.x) < 1000 ) &&
            ( Measurements::lineDistance(chosenPoint, bp, bpr) < LINE_DISTANCE ) &&
            ( bs > 0.2 ) &&
            ( Measurements::isClose(velang, Measurements::angleBetween(bp, gl), 90*(M_PI/180))))
        {
           return new DSIntercept();
        }

        /* And here, we check if the ball is sitting near us. If it is, kick it back.
         * "If the ball is close AND nobody is kicking AND I'm closest to ball...
         * "AND the ball near the goal AND the ball is not moving away..." Kick it.
         */
        Rectangle our_half(-HALF_FIELD_LENGTH, -HALF_FIELD_WIDTH, 0, HALF_FIELD_WIDTH);
        DefenceArea our_da(OUR_TEAM);

        if( our_half.contains(bp)
            && our_da.contains(bp, 2000)
            && kicker_ID == -1
            && Comparisons::distanceBall().minMyTeam() == robot)
        {
            kicker_ID = robot->getID();
            return new DSKick();
        }
    }

    return this;
}


/************************************************************/
DSKick::DSKick()
{
#if DEFENDBEHAVIOR_DEBUG
    std::cout << "DefendStateKick Created" << std::endl;
#endif
    ktpo = new Skill::KickToPointOmni(gameModel->getOppGoal());
}

DSKick::~DSKick()
{
    delete ktpo;
}

DefendState* DSKick::action(Robot* robot)
{
//    std::cout << "DefendStateKick" << std::endl;

    if(ktpo->perform(robot) || ballIsMovingAway()) {
        kicker_ID = -1;
        return new DSIdle();
    }
    return this;
}


/************************************************************/


DSIntercept::DSIntercept()
    : ktpo(nullptr)
    , chosenLinePoint(false)
    , kickingBall(false)
    , kickBallTimeout(0)
{
#if DEFENDBEHAVIOR_DEBUG
    std::cout << "DefendStateKick Created" << std::endl;
#endif
    setMovementTolerances(DIST_TOLERANCE/10, ROT_TOLERANCE);
}

DSIntercept::~DSIntercept()
{
    delete ktpo;
    kicker_ID = -1;
}

DefendState* DSIntercept::action(Robot* robot)
{
//    std::cout << "DefendStateIntercept" << std::endl;

    Point bp = gameModel->getBallPoint();
    Point goal = gameModel->getMyGoal();

    if(not(chosenLinePoint)) {
        //The conditions to go to this state validate the ball is RIGHT NOW
        //Heading to us. We go to the closet point on the ball's path.
        if(!tryGetValidLinePoint(robot)) {
            return new DSIdle();
        }
    }
    else {
        //Inside here we are just in the line of the ball and are waiting
        if(not(kickingBall)) {
            tryGetValidLinePoint(robot);
            float ballRobAng = Measurements::angleBetween(robot, bp);
            setMovementTargets(linePoint, ballRobAng);
            GenericMovementBehavior::perform(robot);

            //If the ball is close, and we are the closet to the ball,
            //and nobody else is kicking, we kick.
            if(Measurements::distance(robot, bp) < 600
                && kicker_ID == -1
                && Comparisons::distanceBall().minMyTeam() == robot) 
            {
                kicker_ID = robot->getID();
                ktpo = new Skill::KickToPointOmni(gameModel->getOppGoal());
                kickingBall = true;
            }

            //If the ball is coming and it stops, go back to idle.
            if(ballIsStopped()) {
                return new DSIdle();
            }
        }
        else {
            //Here we are actually kicking the ball away.
            //Timeout is reset if a kick is made.
            if(ktpo->perform(robot))
                kickBallTimeout = 0;

            //Second condition to return to idle. Stop if it moves too far away
            if(Measurements::distance(robot, bp) > LINE_DISTANCE*3) {
                return new DSIdle();
            }
        }

        //Tiemout conditions. If we have waited too long, or the ball is
        //is too close to the goal, we go back to idle.
        if(++kickBallTimeout > 400
            || Measurements::distance(goal, bp) < GOALIE_DIST //Goalie action distance
            || ballIsMovingAway())
        {
        #if DEFENDBEHAVIOR_DEBUG
            std::cout << "DefendStateKick Timeout " << robot->getID() << std::endl;
        #endif
            return new DSIdle();
        }
        robot->setDribble(true);
    }

    return this;
}

/* Tries to get a valid line point along the ball's projected path.
 * If it finds one, sets this->chosenLinePoint to it and
 * returns true.
 */
bool DSIntercept::tryGetValidLinePoint(Robot* r)
{
    Point bp = gameModel->getBallPoint();
    Point bpp = gameModel->getBallStopPoint();
    Point goal = gameModel->getMyGoal();
    Point p = Measurements::lineSegmentPoint(r->getPosition(), bp, bpp);
    if(abs(goal.x - p.x) < 2500 && Measurements::distance(r, p) < LINE_DISTANCE*2) {
        linePoint = p;
        chosenLinePoint = true;
        return true;
    }
    return false;
}

/************************************************************/

int DefendBehavior::currentUsers = 0;

DefendBehavior::DefendBehavior()
    : state(new DSIdle())
{ 
    if(currentUsers == 0) {
        DefendState::setupClaimedPoints();
    }
    currentUsers += 1;
}

DefendBehavior::~DefendBehavior()
{
    //If 0 (Nobody on DefendBehavior), unclaim all points
    currentUsers -= 1;
    if(currentUsers == 0) {
        DefendState::clearClaimedPoints();
    }
    delete state;
}

void DefendBehavior::perform(Robot* robot)
{
    if(state != nullptr)
    {
        DefendState* nextState = state->action(robot);
        if(nextState != state) {
            delete state;
            state = nextState;
        }
    }
}
