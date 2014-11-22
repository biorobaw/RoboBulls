#include <vector>
#include "include/config/team.h"
#include "behavior/behavior.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/stopbehavior.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "model/gamemodel.h"
#include "strategy/normalgamestrategy.h"

bool NormalGameStrategy::isOnAttack = true;

/*************************************************/
/** CONFIGURATION **/
/* Defines the number of calls to considerSwitchCreiteria, in which the
 * switching condition is true, that must be made until the attack/defend
 * switch is actually made
 */
#define NORMAL_SWITCH_COUNT 16

/* Defines the number of times the ball must be seen outside of the goal
 * to have the robots start moving again. Used to prevent jerkey movement
 * on the field, and causes a delay when the ball *does* come out of the goal.
 */
#define MIN_BALLINGOAL_COUNT 24


/*************************************************/
/** BEHAVIORS **/

/* MiddleSitter
 * A behavior that should not be nessecery that overrides
 * GenericMovementBehavior used to go to the middle of the field
 * The targetPoint is set via setBehParam
 */
class MiddleSitter : public GenericMovementBehavior
{
public:
    MiddleSitter(const ParameterList& list)
        : GenericMovementBehavior(list)
        {}
    void perform(Robot* robot)
    {
        GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
    }
};

/* OpBallBlocker
 * A behavior that should be nessecery that, if applicable, places the robot
 * in the middle of the enemy passer/reciever team. Since there are only three
 * robots, this is an easier task.
 */
class OpBallBlocker : public GenericMovementBehavior
{
public:
    OpBallBlocker(const ParameterList& list)
        : GenericMovementBehavior(list)
        , one(NULL), two(NULL)
    {
        GameModel* gm = GameModel::getModel();
        std::vector<Robot*> opTeam = gm->getOponentTeam();
        //Remove 5 from the team
        opTeam.erase(std::find_if(opTeam.begin(), opTeam.end(),
                     [](Robot* r){return r->getID()==5;}));
        //Get Robot 1 and 2 from above and store
        one = opTeam[0];
        two = opTeam[1];
    }
    void perform(Robot *robot)
    {
        //Midpoint between 1 and 2 and angle to ball
        Point midPoint = Measurments::midPoint(
                    one->getRobotPosition(),
                    two->getRobotPosition());
        float myAngle = Measurments::angleBetween(
                    robot->getRobotPosition(),
                    GameModel::getModel()->getBallPoint());

        setMovementTargets(midPoint, myAngle, true, true);
        GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
    }
private:
    Robot* one, *two;   //Robots to get inbetween
};


/* Behavior StayStill
 * A simple behavior that sends the robot to its current position.
 * Used when the ball is close to the goal
 */
class StayStill : public GenericMovementBehavior
{
public:
    StayStill(const ParameterList& list)
    { UNUSED_PARAM(list); }

    void perform(Robot* robot) override
    {
        setMovementTargets(robot->getRobotPosition(), 0, false);
        GenericMovementBehavior::perform(robot);
    }
};


/* Behavior GoalKickReceiver
 * Sends the robot to a point where it can receive the ball from
 * the goalkeeper.
 */
class GoalKickReciever : public GenericMovementBehavior
{
public:
    GoalKickReciever(const ParameterList& list)
    { UNUSED_PARAM(list); }

    void perform(Robot* robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point wait_point = Point(gm->getMyGoal().x*0.7, 700);
        double wait_orientation = Measurments::angleBetween(robot->getRobotPosition(),gm->getBallPoint());

        setMovementTargets(wait_point, wait_orientation);
        GenericMovementBehavior::perform(robot);
    }
};

/*************************************************/
/** PUBLIC FUNCTIONS **/

NormalGameStrategy::NormalGameStrategy()
    : currentMainAttacker(NULL)
    , currentSuppAttacker(NULL)
    {}

void NormalGameStrategy::assignBeh()
{
    if(GameModel::getModel()->getMyTeam().size() == 3) {
        isOnAttack = considerSwitchCreiteria();
        if(isOnAttack) {
            assignAttackBehaviors();
        } else {
            assignDefendBehaviors();
        }
    }
}


bool NormalGameStrategy::update()
{
    static int ballNotInGoalCount = 0;
    bool oldAttack = isOnAttack;
    GameModel* gm = GameModel::getModel();
    Point ball = gm->getBallPoint();
    Point opGoal = gm->getOpponentGoal();
    Point myGoal = gm->getMyGoal();

    /* This strategy is designed for the Nov.26 presentation
     * and must have three robots to function
     */
    if(gm->getMyTeam().size() != 3) {
        return false;
    }

    isOnAttack = considerSwitchCreiteria();

    if(Measurments::isClose(ball, opGoal, 999))
    {
        /* If the ball is close to opponent goal, we want to have
         * all the robots not go for it. This makes games a lot
         * better on the field and in the simulator
         *
         * It's better to send the Robots away from the
         * keeper than to just stop them where they stand.
         * - Shamsi
         */
        ballNotInGoalCount = 0;
//        BehaviorAssignment<StayStill> ss(true);
//        ss.assignBeh([](Robot* r){return r->getID() != 5;});

//        //*** Assign goalie to ID 5
//        BehaviorAssignment<DefendFarFromBall> goalie_5(true);
//        goalie_5.assignBeh({5});
        assignDefendBehaviors();
    }
    else if(Measurments::isClose(ball, myGoal, 999))
    {
        /* If the ball is close to team goal, one robot
         * moves to receive it from the goalie and the
         * other positions itself mid-field
         */
        ballNotInGoalCount = 0;
        assignGoalKickBehaviors();
    }

    else {
        if(ballNotInGoalCount < MIN_BALLINGOAL_COUNT) {
            /* This ensures that the ball must be detected out of the goal
             * a number of times before the robots move. Otherwise, we sould see
             * very jerkey movement on the field with the ball being falsely
             * detected outside of the goal
             */
            ++ballNotInGoalCount;
            return false;
        }
        if(oldAttack != isOnAttack) {
            /* If the attack status has switched, we stop and return true so that
             * StrategyController can assign this strategy again with the new mode/
             */
            return true;
        }
        else if(isOnAttack) {
            if(currentMainAttacker == nullptr) {
                assignBeh();
                return false;
            }
            AttackMain* attackMain =
                    dynamic_cast<AttackMain*>(currentMainAttacker->getCurrentBeh());
            if(attackMain == nullptr) {
                assignBeh();
                return false;
            }
            if(attackMain->hasKicked()) {
                /* If the attacker has kicked, it has made a pass or a goal, so
                 * we switch attack/defend behaviors. See assignAttackBehaviors
                 */
                assignAttackBehaviors();
            }
        }
        else {
            /* Bug fix: This is entered if the ball comes out from the goal,
             * and the robots shoulddirectly go into defend. This allows swithcing
             * between modes to always work.
             */
            assignDefendBehaviors();
        }
        return false;
    }

    return false;
}


/*************************************************/
/** PRIVATE FUNCTIONS **/
/* If the enemy has the ball, switch to defend. If we have the ball,
 * get the robot which now has the ball and go to Attack. The robot
 * that has the ball is driver, the other one is the defender.
 * The switch is only done when we're confident we should switch
 * (when we see the switch condition NORMAL_SWITCH_COUNT times)
 */
bool NormalGameStrategy::considerSwitchCreiteria()
{
    static int switchCounter = NORMAL_SWITCH_COUNT;

    GameModel* gm = GameModel::getModel();
    Robot* ballRobot = gm->getHasBall();

    if(ballRobot == NULL) {
        return true;
    }
    else if(ballRobot->isOnMyTeam() and not(isOnAttack)) {
        --switchCounter;
        if(switchCounter < 0) {
            /* We have seen the ball in our hands for long enough,
             * we will switch to attack. (retrun true)
             */
            switchCounter = NORMAL_SWITCH_COUNT;
            return true;
        }
    }
    else if(not(ballRobot->isOnMyTeam()) and isOnAttack){
        --switchCounter;
        if(switchCounter < 0) {
            /* We have seen the ball in our hands for long enough,
             * we will switch to defend. (retrun false)
             */
            switchCounter = NORMAL_SWITCH_COUNT;
            return false;
        }
    }

    return NormalGameStrategy::isOnAttack;
}


/* What TwoVOne did, where two robots are on the other side of the
 * field and drive as far as possible, and at the end of that drive,
 * pass the ball to the other and repeat until halfway to the goal and
 * then kick to the goal
 */
void NormalGameStrategy::assignAttackBehaviors()
{
    GameModel* gm = GameModel::getModel();
    Point ballPoint = gm->getBallPoint();
    Robot* driverBot = NULL, *recvBot = NULL;

    if(currentMainAttacker == NULL or currentSuppAttacker == NULL) {
        /* First run: We find the most valid robots for the job */
        findMostValidRobots(ballPoint, driverBot, recvBot);
    }
    else {
        /* Otherwise, we are coming from a previous attack, note here
         * that the driver/receiver are being swapped.
         */
        driverBot = currentSuppAttacker;
        recvBot = currentMainAttacker;
    }
    /**************/

    //*** Assign AttackMain (Passer) behavior
    BehaviorAssignment<AttackMain> mainAttacker(true);
    mainAttacker.setBehParam("recvBot", recvBot);
    mainAttacker.assignBeh(driverBot);

    //*** Assign AttackSupport (Reciever) behavior
    BehaviorAssignment<AttackSupport> suppAttacker(true);
    suppAttacker.setBehParam("passBot", driverBot);
    suppAttacker.assignBeh(recvBot);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});

    //Store information
    currentMainAttacker = driverBot;
    currentSuppAttacker = recvBot;
}


/* Goalie stays at goal. One robot sits at the mid line ready to change
 * to attack. One other one gets in between the enemy passing robot
 * and enemy receiving robot.
 */
void NormalGameStrategy::assignDefendBehaviors()
{
    GameModel* gm = GameModel::getModel();
    Robot* receiver = NULL, *middleSitter = NULL;
    Point wait_point = Point(gm->getOpponentGoal().x * 0.15, 250+500*TEAM);
    findMostValidRobots(wait_point, middleSitter, receiver);
    /**************/

    BehaviorAssignment<MiddleSitter> middleAssign(true);
    middleAssign.setBehParam("targetPoint", wait_point);
    middleAssign.assignBeh(middleSitter);

    BehaviorAssignment<OpBallBlocker> blockerAssign(true);
    blockerAssign.assignBeh(receiver);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});
}

/* Goalie tries to pass to teammate. One robot sits at the mid line
 * ready to change to attack. One other one goes to a point at which
 * it can receive a pass from a goalie
 */
void NormalGameStrategy::assignGoalKickBehaviors()
{
    GameModel* gm = GameModel::getModel();
    Robot* receiver = NULL, *middleSitter = NULL;
    Point wait_point = Point(gm->getMyGoal().x*0.7, 700);
    findMostValidRobots(wait_point, receiver, middleSitter);
    /**************/

    BehaviorAssignment<MiddleSitter> middleAssign(true);
    middleAssign.setBehParam("targetPoint", Point(0, -1500+3000*TEAM));
    middleAssign.assignBeh(middleSitter);

    BehaviorAssignment<GoalKickReciever> receiverAssign(true);
    receiverAssign.assignBeh(receiver);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});
}


/* Utility function that finds two robots `a_out` and `b_out` such that:
 * - a_out is closest to the `target`
 * - b_out is set to the other robot
 * - both a_out and b_out are not the goalie robot
 */
void NormalGameStrategy::findMostValidRobots(Point target, Robot*& a_out, Robot*& b_out)
{
    std::vector<Robot*> myTeam = GameModel::getModel()->getMyTeam(); //Copies
    Robot* a_found = NULL, *b_found = NULL;

    static auto id_equals_5 = [](Robot* rob) {
        return rob->getID() == 5;
    };
    auto closestToTarget = [&](Robot* rob1, Robot* rob2) {
        return Measurments::distance(rob1->getRobotPosition(), target) <
               Measurments::distance(rob2->getRobotPosition(), target);
    };

    myTeam.erase(std::find_if(myTeam.begin(), myTeam.end(), id_equals_5));

    a_found = *std::min_element(myTeam.begin(), myTeam.end(), closestToTarget);

    myTeam.erase(std::find(myTeam.begin(), myTeam.end(), a_found));

    b_found = myTeam.front();

    if((a_found == NULL) or (b_found == NULL))
        throw std::runtime_error("ERROR: Valid robots not found!");

    a_out = a_found;
    b_out = b_found;
}
