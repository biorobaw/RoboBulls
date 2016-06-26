#include "include/config/team.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/goalie.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/defendbehavior.h"
#include "behavior/defend.h"
#include "behavior/refstop.h"
#include "behavior/wall.h"
#include "behavior/markbot.h"
#include "behavior/challengeballbot.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "strategy/normalgamestrategy.h"


//! @brief Are we currently on attack?
bool NormalGameStrategy::isOnAttack = true;
bool NormalGameStrategy::clearing_ball = false;

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
        double wait_orientation = Measurements::angleBetween(robot->getPosition(),gm->getBallPoint());

        setMovementTargets(wait_point, wait_orientation);
        GenericMovementBehavior::perform(robot);
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
    ballInOpDefArea = opp_def_area.contains(ball, 2*ROBOT_RADIUS);
    ballInMyDefArea = our_def_area.contains(ball, 2*ROBOT_RADIUS);

    isOnAttack = considerSwitchCreiteria();

    /* If the ball is close to our goal or the opponent goal,
     * the robots retreat back to their half, and no attack/defend
     * is run.
     */
    if(posedge(ballInOpDefArea || ballInMyDefArea))
        assignGoalKickBehaviors();

    // If a defender is clearing the ball, assign both the
    // attackers to support
    if(gameModel->findMyTeam(WALL_1)->hasBehavior()
    || gameModel->findMyTeam(WALL_2)->hasBehavior())
    {
        clearing_ball = gameModel->findMyTeam(WALL_1)->getBehavior()->isFinished()
                     || gameModel->findMyTeam(WALL_2)->getBehavior()->isFinished();
    }
    else if(gameModel->findMyTeam(WALL_1)->hasBehavior())
        clearing_ball = gameModel->findMyTeam(WALL_1)->getBehavior()->isFinished();
    else if(gameModel->findMyTeam(WALL_2)->hasBehavior())
        clearing_ball = gameModel->findMyTeam(WALL_2)->getBehavior()->isFinished();
    else
        clearing_ball = false;

    if(posedge(clearing_ball))
        assignClearBehaviors();

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
                robot->assignBeh<RefStop>();
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
            else if(isOnAttack && !clearing_ball)
            {
                // Assign attack behaviors
                assignAttackBehaviors();

                // Set flag to re-assign attack behaviors if a pass has been
                // completed to switch main/support attackers
                if (currentSuppAttacker != nullptr
                &&  currentSuppAttacker->getBehavior()->isFinished())
                    attack_beh_assigned = false;

                return false;
            }
            else if (!clearing_ball)
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
        goalie->assignBeh<Goalie>();
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
        r->assignBeh<Goalie>();

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

    std::cout << "Assigning Attack Behaviors" << std::endl;

    Robot* wall1 = gameModel->findMyTeam(WALL_1);
    Robot* wall2 = gameModel->findMyTeam(WALL_2);
    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);

    if(wall1)
        wall1->assignBeh<Wall>();
    if(wall2)
        wall2->assignBeh<Wall>();

    if(attack1 != nullptr && attack2 != nullptr)
    {
        if(Measurements::distance(attack1, gameModel->getBallPoint()) <
           Measurements::distance(attack2, gameModel->getBallPoint()))
        {
            attack1->assignBeh<AttackMain>();
            attack2->assignBeh<AttackSupport>();
        }
        else
        {
            attack1->assignBeh<AttackSupport>();
            attack2->assignBeh<AttackMain>();
        }
    }
    else if(attack1 != nullptr)
        attack1->assignBeh<AttackMain>();
    else if(attack2 != nullptr)
        attack2->assignBeh<AttackMain>();

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

    std::cout << "Assigning Defend Behaviors" << std::endl;

    Robot* wall1 = gameModel->findMyTeam(WALL_1);
    Robot* wall2 = gameModel->findMyTeam(WALL_2);
    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);

    if(wall1)
        wall1->assignBeh<Wall>();
    if(wall2)
        wall2->assignBeh<Wall>();
    if(attack1)
        attack1->assignBeh<MarkBot>();
    if(attack2)
        attack2->assignBeh<ChallengeBallBot>();

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
    std::cout << "Assigning Goal Kick Behaviors" << std::endl;

    Robot* wall1 = gameModel->findMyTeam(WALL_1);
    Robot* wall2 = gameModel->findMyTeam(WALL_2);
    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);

    if(wall1)
        wall1->assignBeh<Wall>();
    if(wall2)
        wall2->assignBeh<Wall>();
    if(attack1)
        attack1->assignBeh<MarkBot>();
    if(attack2)
        attack2->assignBeh<AttackSupport>();

    assignGoalieIfOk();

    defend_beh_assigned = false;
    attack_beh_assigned = false;
}


// Used when a defender is trying to clear the ball
void NormalGameStrategy::assignClearBehaviors()
{
    std::cout << "Assigning Clear Behaviors" << std::endl;

    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);

    if(attack1)
        attack1->assignBeh<AttackSupport>();
    if(attack2)
        attack2->assignBeh<AttackSupport>();

    defend_beh_assigned = false;
    attack_beh_assigned = false;
}
//! @endcond
