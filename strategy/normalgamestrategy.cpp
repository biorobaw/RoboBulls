#include <vector>
#include "include/config/team.h"
#include "behavior/behaviorassignment.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/defendbehavior.h"
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
        /* Get the two robots on OpTeam; One is the one with the ball, the other
         * is closest to our goal. We plan to block inbetween them.
         */
        one = Comparisons::distanceBall().ignoreID(5).minOpTeam();
        two = Comparisons::distanceMyGoal().ignoreIDs({5, one->getID()}).minOpTeam();

		if(!one or !two)
			throw std::runtime_error("OpBallBlocker: Could not find two robots!");
    }
	
    void perform(Robot *robot)
    {
        //Midpoint between 1 and 2 and angle to ball
        Point midPoint = Measurments::midPoint(one, two);
        float myAngle  = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(midPoint, myAngle);
        GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
    }
private:
    Robot* one, *two;   //Robots to get inbetween
};


/* Behavior RetreatAfterGoal
 * Sends the robot to a point at which to wait for the
 * opponent goalie to kick the ball out, or to recieve the ball from the
 * friendly goalkeeper.
 */
class RetreatAfterGoal : public GenericMovementBehavior
{
    int myYPos = 0;
public:
    RetreatAfterGoal(int whichYPosition)
        : myYPos(whichYPosition)
        { }
    void perform(Robot* robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point wait_point = Point(gm->getMyGoal().x*0.5, myYPos);
        double wait_orientation = Measurments::angleBetween(robot->getRobotPosition(),gm->getBallPoint());

        setMovementTargets(wait_point, wait_orientation);
        GenericMovementBehavior::perform(robot);
    }
};

/*************************************************/
/** PUBLIC FUNCTIONS **/

bool NormalGameStrategy::hasStoppedForThisKickoff = false;

NormalGameStrategy::NormalGameStrategy()
    : currentMainAttacker(NULL)
    , currentSuppAttacker(NULL)
    , ballOriginalPos(GameModel::getModel()->getBallPoint())
    { }

/* So, here, the only way to get back into NGS is through a kickoff.
 * If StrategyController is deleting this because of a new gameModel
 * state, we make sure the NEXT kickoff is stopped for. This is needed
 * because StrategyController does delete this upon switching from attack
 * and defend, so this seperates those two deletion cases
 */
NormalGameStrategy::~NormalGameStrategy()
{
    if(gameModel->isNewCommand())
        hasStoppedForThisKickoff = false;
}

void NormalGameStrategy::assignBeh()
{
    if(gameModel->getMyTeam().size() >= 3) {
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

    /* This strategy is designed for the Nov.26 presentation /
     * Feb 13-14 Engineering Expo
     * and must have <= four robots to function
     */
    if(gm->getMyTeam().size() < 3) {
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
    else
    {
        /*
         * If we go to normal strategy after our kickoff
         *      we have attack behavior
         * if we go to normal stratgey after opponent kickoff
         *      we wait until opponent players move the ball
         * else
         *      we follow the general normal game strategy
         * */
        char prevGs = gm->getPreviousGameState();

        if(((prevGs == 'K' && TEAM == TEAM_BLUE) ||
            (prevGs == 'k' && TEAM == TEAM_YELLOW)) &&
            !ballMoved)
        {
            assignAttackBehaviors();
        }
        else if(((prevGs == 'k' && TEAM == TEAM_BLUE) ||
                 (prevGs == 'K' && TEAM == TEAM_YELLOW)) &&
                 !ballMoved &&
                 !hasStoppedForThisKickoff)
        {
            BehaviorAssignment<SimpleBehaviors> haltAssignment;
            haltAssignment.setSingleAssignment(true);
            haltAssignment.assignBeh();

        }
        else
        {
            //The ball has moved and we have stopped for *this* kickoff.
            hasStoppedForThisKickoff = true;

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
                 * and the robots should directly go into defend. This allows swithcing
                 * between modes to always work.
                 */
                //assignDefendBehaviors();
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
 * then kick to the goal.
 *
 * EXPO: The third robot does "OpBallBlocker"
 */
void NormalGameStrategy::assignAttackBehaviors()
{
    Robot* driverBot, *recvBot, *otherBot;
    findMostValidRobots(gameModel->getBallPoint(), driverBot, recvBot, otherBot);
    /**************/

    driverBot->assignBeh<AttackMain>(recvBot);
      recvBot->assignBeh<AttackSupport>(driverBot);
    if(otherBot)
     otherBot->assignBeh<OpBallBlocker>();
    gameModel->findMyTeam(5)->assignBeh<DefendFarFromBall>();
}


/* Goalie stays at goal. One robot sits at the mid line ready to change
 * to attack. One other one gets in between the enemy passing robot
 * and enemy receiving robot.
 */
void NormalGameStrategy::assignDefendBehaviors()
{
//    Robot* blocker, *receiver, *other;
//    Point wait_point = Point(gameModel->getOpponentGoal().x * 0.15, 250+500*TEAM);
//    findMostValidRobots(gameModel->getBallPoint(), blocker, receiver, other);
    /**************/

    for(Robot* rob : gameModel->getMyTeam()) {
        if(rob->getID() == 5)
            continue;
        rob->assignBeh<DefendBehavior>();
    }

    gameModel->findMyTeam(5)->assignBeh<DefendFarFromBall>();
}

/* This runs when the ball is near in friendly penalty area.
 * Goalie tries to pass to teammate. One robot sits at the mid line
 * ready to change to attack. One other one goes to a point at which
 * it can receive a pass from a goalie.
 */
void NormalGameStrategy::assignGoalKickBehaviors()
{
    Robot* receiver, *middler, *other;
    Point wait_point = Point(gameModel->getMyGoal().x*0.5, 1000);
    findMostValidRobots(wait_point, receiver, middler, other);
    /**************/

     middler->assignBeh<GenericMovementBehavior>( Point(0, -1500+3000*TEAM) );
    receiver->assignBeh<RetreatAfterGoal>(1000);
    if(other)
       other->assignBeh<RetreatAfterGoal>(-1000);
    gameModel->findMyTeam(5)->assignBeh<DefendFarFromBall>();
}

/* This runs when the ball is in the opponent penalty area
 * Goalie tries to pass to teammate. One robot sits at the mid line
 * ready to change to attack. One other one goes to a point at which
 * it can receive a pass from a goalie.
 */
void NormalGameStrategy::assignRetreatBehaviors()
{
    Robot* right_rob, *left_rob, *othr_rob;
    Point right_point = Point(gameModel->getMyGoal().x*0.5, -1000);
    findMostValidRobots(right_point, right_rob, left_rob, othr_rob);
    /**************/

    right_rob->assignBeh<RetreatAfterGoal>(-1000);
     left_rob->assignBeh<RetreatAfterGoal>( 1000);
     if(othr_rob)
        othr_rob->assignBeh<GenericMovementBehavior>(Point(gameModel->getMyGoal().x*0.7, 0));
    gameModel->findMyTeam(5)->assignBeh<DefendFarFromBall>();
}


/* Utility function that finds two robots `a_out` and `b_out` such that:
 * - a_out is closest to the `target`
 * - b_out is set to the other robot
 * - both a_out and b_out are not the goalie robot
 */
void NormalGameStrategy::findMostValidRobots(Point target, Robot*& a_out, Robot*& b_out, Robot*& c_out)
{
    std::vector<Robot*>& myTeam = gameModel->getMyTeam();
    Robot* a_found = NULL, *b_found = NULL, *c_found = NULL;
	
	//Find robot closest to `target`
    a_found = *Comparisons::distance(target).ignoreID(5).min(myTeam);

    //b_found is the otherBotremaining robot (3-robot teams)
    b_found = Comparisons::idNot(5).ignoreID(a_found).anyMyTeam();

    //c_found is also the remaining robot
    c_found = Comparisons::idNot(5).ignoreIDs({a_found, b_found}).anyMyTeam();

    if(!a_found || !b_found)
        throw std::runtime_error("ERROR: Valid robots not found!");

    a_out = a_found;
    b_out = b_found;
    c_out = c_found;
}
