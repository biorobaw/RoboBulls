#include "include/config/team.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/goaliebehavior.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/defendbehavior.h"
#include "behavior/simplebehaviors.h"
#include "behavior/challengeballbot.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "strategy/normalgamestrategy.h"


//! @brief Are we currently on attack?
bool NormalGameStrategy::isOnAttack = true;

//Rest of the code here is not in Doxygen
//! @cond

/*************************************************/
/** CONFIGURATION **/
/* Defines the number of calls to considerSwitchCreiteria, in which the
 * switching condition is true, that must be made until the attack/defend
 * switch is actually made
 */
#define ATT_TO_DEF_SWITCH_COUNT 30
#define DEF_TO_ATT_SWITCH_COUNT 30

/* Defines the number of times the ball must be seen outside of the goal
 * to have the robots start moving again. Used to prevent jerky movement
 * on the field, and causes a delay when the ball *does* come out of the goal.
 */
#define MIN_BALLINGOAL_COUNT 24


/*************************************************/
/** BEHAVIORS **/

/* Behavior RetreatAfterGoal
 * Sends the robot to a point at a fixed x and a given y (in constructor)
 * Used to sit still when the ball is in either goal
 */
class RetreatAfterGoal : public StaticMovementBehavior
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
        double wait_orientation = Measurements::angleBetween(robot->getPosition(),gm->getBallPoint());

        setMovementTargets(wait_point, wait_orientation);
        StaticMovementBehavior::perform(robot);
    }
};

/*************************************************/
/** PUBLIC FUNCTIONS **/

bool NormalGameStrategy::hasStoppedForThisKickoff = false;

NormalGameStrategy::NormalGameStrategy()
    : currentMainAttacker(nullptr)
    , currentSuppAttacker(nullptr)
    , ballOriginalPos(GameModel::getModel()->getBallPoint())
    , our_def_area(0)
    , opp_def_area(1)
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
    bool ballMoved = !Measurements::isClose(ballOriginalPos, ball);

    //Ball near goals, used as checks to not do anything first
    static bool ballInOpDefArea = false;
    static bool ballInMyDefArea = false;
    ballInOpDefArea = opp_def_area.contains(ball);
    ballInMyDefArea = our_def_area.contains(ball);

    isOnAttack = considerSwitchCreiteria();

    /* If the ball is close to our goal or the opponent goal,
     * the robots retreat back to their half, and no attack/defend
     * is run.
     */
    if(posedge(ballInOpDefArea or ballInMyDefArea))
        assignGoalKickBehaviors();

    if(ballInOpDefArea or ballInMyDefArea)
    {
        /* A count is kept to ensure robots don't move
         * until the ball is surely out of the goal
         */
        ballNotInGoalCount = 0;
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

        if(((prevGs == 'K' && OUR_TEAM == TEAM_BLUE) ||
            (prevGs == 'k' && OUR_TEAM == TEAM_YELLOW)) &&
            !ballMoved)
        {
            assignAttackBehaviors();
        }
        else if(((prevGs == 'k' && OUR_TEAM == TEAM_BLUE) ||
                 (prevGs == 'K' && OUR_TEAM == TEAM_YELLOW)) &&
                 !ballMoved &&
                 !hasStoppedForThisKickoff)
        {
            //Idling while the opponent has not kicked the ball
            for(Robot* robot : gameModel->getMyTeam())
                robot->assignBeh<SimpleBehaviors>();
        }
        else
        {
            //The ball has moved and we have stopped for *this* kickoff.
            hasStoppedForThisKickoff = true;

            // Prevent jerky movement
            if(ballNotInGoalCount < MIN_BALLINGOAL_COUNT)
            {
                ++ballNotInGoalCount;
                return false;
            }

            if(oldAttack != isOnAttack)
            {
                // If the attack status has switched, we stop and return true so that
                // StrategyController can assign this strategy again with the new mode
                return true;
            }
            else if(isOnAttack)
            {
                // Assign attack behaviors
                assignAttackBehaviors();

                // Set flag to re-assign attack behaviors if a pass has been
                // completed to switch main/support attackers
                if (currentSuppAttacker != nullptr
                &&  currentSuppAttacker->getCurrentBeh()->isFinished())
                    attack_beh_assigned = false;

                return false;
            }
            else
                assignDefendBehaviors();
        }
        return false;
    }

    return false;
}

// Utility function to not duplicate this code
void NormalGameStrategy::assignGoalieIfOk()
{
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<GoalieBehavior>();
}

//Static function to be used in other places.
//Was added here to deal with simplebehaviors.cpp
void NormalGameStrategy::moveRobotToIdleLine(Robot* robot, bool waiter)
{
    Point wait_point(0, -1500);

    //Closest guy to the wait point sits there instead, if requested
    if(waiter && Comparisons::distance(wait_point).minMyTeam() == robot) {
        robot->assignBeh<StaticMovementBehavior>( wait_point );
    } else {
        //Otherwise assigns the robot to sit along a line across the field
        int incSize = HALF_FIELD_WIDTH / (gameModel->getMyTeam().size() - 1);
        int y = -HALF_FIELD_WIDTH/2 + incSize * robot->getID();
        robot->assignBeh<RetreatAfterGoal>(y);
    }
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
      Robot* r = gameModel->findMyTeam(GOALIE_ID);
      if(r!=nullptr)
        r->assignBeh<GoalieBehavior>();

    static int switchCounter = 0;
    Robot* ballRobot = gameModel->getHasBall();


    if(ballRobot == NULL || (ballRobot->isOnMyTeam() and not(isOnAttack))) {
        ++switchCounter;
        if(switchCounter > DEF_TO_ATT_SWITCH_COUNT) {
            /* We have seen the ball in our hands for long enough,
             * we will switch to attack. (return true)
             */
            switchCounter = 0;
            return true;
        }
    }
    else if(ballRobot and not(ballRobot->isOnMyTeam()) and isOnAttack){
        ++switchCounter;
        if(switchCounter > ATT_TO_DEF_SWITCH_COUNT) {
            /* We have not seen the ball in our hands for long enough,
             * we will switch to defend. (return false)
             */
            switchCounter = 0;
            return false;
        }
    }
    else {
        //We default to defence if ball is in either defence area
        Point ball = gameModel->getBallPoint();
        if(our_def_area.contains(ball) || opp_def_area.contains(ball))
            return false;
    }

    //No change in attack
    return NormalGameStrategy::isOnAttack;
}

//To ingnore robots with no kicker in assigning them to drive the ball
static bool no_kicker(Robot* robot) {
    return not robot->hasKicker();
}

/* What TwoVOne did, where two robots are on the other side of the
 * field and drive as far as possible, and at the end of that drive,
 * pass the ball to the other and repeat until halfway to the goal and
 * then kick to the goal.
 * EXPO: The third robot does "OpBallBlocker"
 */
void NormalGameStrategy::assignAttackBehaviors()
{
    // Don't re-assign unnecessarily because it
    // breaks behaviors
    if(attack_beh_assigned)
        return;

//    std::cout << "Assigning Attack Behaviors" << std::endl;

    // Assign everyone as a defender for starters
    for(Robot* rob : gameModel->getMyTeam())
        rob->assignBeh<DefendBehavior>();

    // Robot closest to the ball, not the goalie, that has a kicker
    currentMainAttacker = Comparisons::distanceBall().ignore_if(no_kicker).ignoreID(GOALIE_ID).minMyTeam();
    if(currentMainAttacker)
        currentMainAttacker->assignBeh<AttackMain>();

    // Robot second closest to the ball, not driverBot nor the goalie, that has a kicker
    currentSuppAttacker = Comparisons::distanceBall().ignore_if(no_kicker).ignoreID(GOALIE_ID).ignoreID(currentMainAttacker).minMyTeam();
    if(currentSuppAttacker)
        currentSuppAttacker->assignBeh<AttackSupport>();

    //Finally assign goalie
    assignGoalieIfOk();

    defend_beh_assigned = false;
    attack_beh_assigned = true;
}


/* Goalie stays at goal. One robot sits at the mid line ready to change
 * to attack. One other one gets in between the enemy passing robot
 * and enemy receiving robot.
 */
void NormalGameStrategy::assignDefendBehaviors()
{
    // Don't re-assign unnecessarily because it
    // breaks behaviors
    if(defend_beh_assigned)
        return;

//    std::cout << "Assigning Defend Behaviors" << std::endl;

    // Assign everyone as a defender for starters
    for(Robot* rob : gameModel->getMyTeam())
            rob->assignBeh<DefendBehavior>();

    // Robot closest to the ball moves to block
    Robot* blocker = Comparisons::distanceBall().minMyTeam();
    if(blocker)
        blocker->assignBeh<ChallengeBallBot>();

    //Usual special case for 5
    assignGoalieIfOk();

    defend_beh_assigned = true;
    attack_beh_assigned = false;
}


/* This runs when the ball is near in either penalty area.
 * Goalie is normal, and all robots are made to go to the idle line.
 * And attacker is removed because we are not attacking.
 */
void NormalGameStrategy::assignGoalKickBehaviors()
{
    // Assign everyone as a defender for starters
    for(Robot* rob : gameModel->getMyTeam())
            rob->assignBeh<DefendBehavior>();

    //Usual special case for 5
    assignGoalieIfOk();

    defend_beh_assigned = false;
    attack_beh_assigned = false;
}

//! @endcond
