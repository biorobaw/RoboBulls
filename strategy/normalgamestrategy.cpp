#include <vector>
#include "include/config/team.h"
#include "behavior/behavior.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"
#include "utilities/comparisons.h"
#include "strategy/normalgamestrategy.h"

bool NormalGameStrategy::isOnAttack = true;

/*************************************************/
/** CONFIGURATION **/
/* Defines the number of calls to considerSwitchCreiteria, in which the
 * switching condition is true, that must be made until the attack/defend
 * switch is actually made
 */
#define ATT_TO_DEF_SWITCH_COUNT 16
#define DEF_TO_ATT_SWITCH_COUNT 16

/* Defines the number of times the ball must be seen outside of the goal
 * to have the robots start moving again. Used to prevent jerkey movement
 * on the field, and causes a delay when the ball *does* come out of the goal.
 */
#define MIN_BALLINGOAL_COUNT 24


/*************************************************/
/** BEHAVIORS **/

/* OpBallBlocker
 * A behavior that should be nessecery that, if applicable, places the robot
 * in the middle of the enemy passer/reciever team. Since there are only three
 * robots, this is an easier task.
 */
class OpBallBlocker : public GenericMovementBehavior
{
public:
    OpBallBlocker()
        : one(NULL), two(NULL)
    {
		//Get the two robots that are not ID 5 on OpTeam
        one = Comparisons::idNot(5).anyMyTeam();
        two = Comparisons::idNot(5).ignoreID(one->getID()).anyMyTeam();
		if(!one or !two)
			throw std::runtime_error("OpBallBlocker: Could not find two robots!");
    }
	
    void perform(Robot *robot)
    {
        //Midpoint between 1 and 2 and angle to ball
        Point midPoint = Measurments::midPoint(one, two);
        float myAngle  = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(midPoint, myAngle, true, true);
        GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
    }
private:
    Robot* one, *two;   //Robots to get inbetween
};


/* Behavior GoalKickReceiver
 * Sends the robot to a point where it can receive the ball from
 * the goalkeeper.
 */
class GoalKickReciever : public GenericMovementBehavior
{
public:
    void perform(Robot* robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point wait_point = Point(gameModel->getMyGoal().x*0.5, 1000);
        double wait_orientation = Measurments::angleBetween(robot->getRobotPosition(),gm->getBallPoint());

        setMovementTargets(wait_point, wait_orientation);
        GenericMovementBehavior::perform(robot);
    }
};

/* Behavior Retreat Left
 * Sends the robot to a point at which to wait for the
 * opponent goalie to kick the ball out
 */
class RetreatLeft : public GenericMovementBehavior
{
public:
    void perform(Robot* robot) override
    {
        Point wait_point = Point(gameModel->getMyGoal().x*0.5, 1000);
        double wait_orientation = Measurments::angleBetween(robot->getRobotPosition(),
                                                            gameModel->getBallPoint());
		
        setMovementTargets(wait_point, wait_orientation);
        GenericMovementBehavior::perform(robot);
    }
};

/* Behavior Retreat Right
 * Sends the robot to a point at which to wait for the
 * opponent goalie to kick the ball out
 */
class RetreatRight : public GenericMovementBehavior
{
public:
    void perform(Robot* robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point wait_point = Point(gm->getMyGoal().x*0.5, -1000);
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
    , ballOriginalPos(GameModel::getModel()->getBallPoint())
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
    bool ballMoved = !Measurments::isClose(ballOriginalPos, ball);

    /* This strategy is designed for the Nov.26 presentation
     * and must have three robots to function
     */
    if(gm->getMyTeam().size() != 3) {
        return false;
    }

    isOnAttack = considerSwitchCreiteria();

    if(Measurments::isClose(ball, opGoal, 999))
    {
        /* If the ball is close to opponent goal, the robots
         * retreate back to their half
         */
        ballNotInGoalCount = 0;
        assignRetreatBehaviors();
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
        /*
         * If we go to normal strategy after our kickoff
         *      we have attack behavior
         * if we go to normal stratgey after opponent kickoff
         *      we wait until opponent players move the ball
         * else
         *      we follow the general normal game strategy
         * */
        if(((gm->getPreviousGameState() == 'K' && TEAM == TEAM_BLUE) ||
            (gm->getPreviousGameState() == 'k' && TEAM == TEAM_YELLOW)) &&
            !ballMoved)
        {
            assignAttackBehaviors();
        }
        else if(((gm->getPreviousGameState() == 'k' && TEAM == TEAM_BLUE) ||
                 (gm->getPreviousGameState() == 'K' && TEAM == TEAM_YELLOW)) &&
                 !ballMoved)
        {
            BehaviorAssignment<SimpleBehaviors> haltAssignment;
            haltAssignment.setSingleAssignment(true);
            haltAssignment.assignBeh();
        }
        else
        {
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
    static int switchCounter = 0;

    GameModel* gm = GameModel::getModel();
    Robot* ballRobot = gm->getHasBall();

    if(ballRobot == NULL) {
        if(not(isOnAttack)) {
            ++switchCounter;
            return (switchCounter > DEF_TO_ATT_SWITCH_COUNT);
        }
        return true;
    }
    else if(ballRobot->isOnMyTeam() and not(isOnAttack)) {
        ++switchCounter;
        if(switchCounter > ATT_TO_DEF_SWITCH_COUNT) {
            /* We have seen the ball in our hands for long enough,
             * we will switch to attack. (retrun true)
             */
            switchCounter = 0;
            return true;
        }
    }
    else if(not(ballRobot->isOnMyTeam()) and isOnAttack){
        ++switchCounter;
        if(switchCounter > DEF_TO_ATT_SWITCH_COUNT) {
            /* We have not seen the ball in our hands for long enough,
             * we will switch to defend. (retrun false)
             */
            switchCounter = 0;
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

    findMostValidRobots(ballPoint, driverBot, recvBot);

//    if(currentMainAttacker == NULL or currentSuppAttacker == NULL) {
//        /* First run: We find the most valid robots for the job */

//    }
//    else {
//        /* Otherwise, we are coming from a previous attack, note here
//         * that the driver/receiver are being swapped.
//         */
//        recvBot = currentMainAttacker;
//        driverBot = currentSuppAttacker;
//    }
    /**************/

    //*** Assign AttackMain (Passer) behavior
    BehaviorAssignment<AttackMain> mainAttacker(true);
    mainAttacker.assignBeh(driverBot, recvBot);

    //*** Assign AttackSupport (Reciever) behavior
    BehaviorAssignment<AttackSupport> suppAttacker(true);
    suppAttacker.assignBeh(recvBot, driverBot);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});

    //Store information
    //currentMainAttacker = driverBot;
    //currentSuppAttacker = recvBot;
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

    BehaviorAssignment<GenericMovementBehavior> middleAssign(true);
    middleAssign.assignBeh(middleSitter, wait_point);

    BehaviorAssignment<OpBallBlocker> blockerAssign(true);
    blockerAssign.assignBeh(receiver);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});
}

/* This runs when the ball is near in friendly penalty area.
 * Goalie tries to pass to teammate. One robot sits at the mid line
 * ready to change to attack. One other one goes to a point at which
 * it can receive a pass from a goalie.
 */
void NormalGameStrategy::assignGoalKickBehaviors()
{
    GameModel* gm = GameModel::getModel();
    Robot* receiver = NULL, *middleSitter = NULL;
    Point wait_point = Point(gm->getMyGoal().x*0.5, 1000);
    findMostValidRobots(wait_point, receiver, middleSitter);
    /**************/

    BehaviorAssignment<GenericMovementBehavior> middleAssign(true);
    middleAssign.assignBeh(middleSitter, Point(0, -1500+3000*TEAM));

    BehaviorAssignment<GoalKickReciever> receiverAssign(true);
    receiverAssign.assignBeh(receiver);

    //*** Assign goalie to ID 5
    BehaviorAssignment<DefendFarFromBall> goalie_5(true);
    goalie_5.assignBeh({5});
}

/* This runs when the ball is in the opponent penalty area
 * Goalie tries to pass to teammate. One robot sits at the mid line
 * ready to change to attack. One other one goes to a point at which
 * it can receive a pass from a goalie.
 */
void NormalGameStrategy::assignRetreatBehaviors()
{
    GameModel* gm = GameModel::getModel();
    Robot* right_rob = NULL, *left_rob = NULL;
    Point right_point = Point(gm->getMyGoal().x*0.5, -1000);
    findMostValidRobots(right_point, right_rob, left_rob);
    /**************/

    BehaviorAssignment<RetreatRight> rightRetreat(true);
    rightRetreat.assignBeh(right_rob);

    BehaviorAssignment<GoalKickReciever> leftRetreat(true);
    leftRetreat.assignBeh(left_rob);

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
    std::vector<Robot*>& myTeam = gameModel->getMyTeam();
    Robot* a_found = NULL, *b_found = NULL;
	
	//Find robot closest to `target`
    a_found = *Comparisons::distance(target).ignoreID(5).min(myTeam);

	//b_found is now the remaining robot (3-robot teams)
    b_found = Comparisons::idNot(5).ignoreID(a_found->getID()).anyMyTeam();

    if((a_found == NULL) or (b_found == NULL))
        throw std::runtime_error("ERROR: Valid robots not found!");

    a_out = a_found;
    b_out = b_found;
}
