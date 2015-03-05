#include <stdlib.h>
#include "behavior/genericskillbehavior.h"
#include "skill/stop.h"
#include "model/gamemodel.h"
#include "strategy/videostrategies.h"
#include "utilities/debug.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "gui/guiinterface.h"

/************************************************************************/
/* USER CONFIGURATION */

/* ***************** OmniRandomKicker Configuration
 * X_DEV and Y_DEV define the maximum deviation in one direction that will be applied
 *      when kicking to the target. Thus the name "random." Cannot be 0.
 */
static int X_DEV = 500;
static int Y_DEV = 1000;

/* ***************** VideoStrategy2 Configuration
 * CLOSE_DIST Defines the distance the ball must be to the ball predicion point in state
 *      WAITING before the robot starts KickToPointOmni to kick it away.
 * WAIT_COUNT_MAX Defines the maximum for a counter in the WAITING state that, if hit,
 *        The robot will return to the waiting point.
 */
#if SIMULATED
static int CLOSE_DIST = 800;
static int WAIT_COUNT_MAX = 500;
#else
static int CLOSE_DIST = 800;
static int WAIT_COUNT_MAX = 2000;
#endif

/************************************************************************/
/* BEHAVIORS */

/* OMNI RANDOM KICKER */
OmniRandomKicker::OmniRandomKicker(Robot* whoIsRecever)
    : receiver(whoIsRecever)
    , bhasKicked(false)
{
    debug::registerVariable("x_dev", &X_DEV);
    debug::registerVariable("y_dev", &Y_DEV);
}
    
OmniRandomKicker::~OmniRandomKicker()
{
    delete ktp;
}
    
bool OmniRandomKicker::isFinished()
{
    return bhasKicked;
}
    
void OmniRandomKicker::perform(Robot* robot)
{
    if(ktp == nullptr) {
        Point offset = Point(-X_DEV+rand()%(2*X_DEV), -Y_DEV+rand()%(2*Y_DEV));
        Point myGoal = gameModel->getMyGoal();
        Point opgoal = gameModel->getOpponentGoal();
        Point less = std::min(myGoal, opgoal, Comparisons::distance(receiver));
        ktp = new Skill::KickToPointOmni(less + offset);
    } else {
        if(ktp->perform(robot)) {
            bhasKicked = true;
        }
    }
}



BallReceiver::BallReceiver(Point pointToWaitAt)
    : waitPoint(pointToWaitAt)
    { }

void  BallReceiver::perform(Robot* robot)
{
    static bool onMySide = false;

    Point prediction = gameModel->getBallPrediction();
    onMySide = ((prediction.x<0) == (waitPoint.x<0));

    /* We only consider the prediction of it is the first time the prediction
     * is on our side. Otherwise, when the ball transitions back to the other side,
     * target is reset to waitPoint.
     */
    if( posedge(onMySide) ) {
        target = prediction;
    }
    else if(negedge(onMySide)) {
        target = waitPoint;
    }

    float ang = Measurments::angleBetween(robot, gameModel->getBallPoint());
    setMovementTargets(target, ang);
    GenericMovementBehavior::perform(robot);
}

/************************************************************************/
/* VIDEO STRATEGY 1 */

//Used to negate pass / receiver combo when strategy finishes
bool vs1_oppositeRobots = false;

VideoStrategy1::VideoStrategy1(int r1, int r2)
    : r1ID(r1)
    , r2ID(r2)
    { }

VideoStrategy1::~VideoStrategy1()
{
    vs1_oppositeRobots = !vs1_oppositeRobots;
}

Point VideoStrategy1::getSideFor(int robot)
{
    return (robot == r1ID) ? Point(-2500, 0) : Point(2500, 0);
}

void VideoStrategy1::assignBeh() 
{
    Robot* r1 = gameModel->findMyTeam(r1ID);
    Robot* r2 = gameModel->findMyTeam(r2ID);

    if(r1 and r2)  {
        if(vs1_oppositeRobots) {
            this->currentPasser = r1;
            this->currentRecver = r2;
        } else {
            this->currentPasser = r2;
            this->currentRecver = r1;
        }
        Point recvrTarget = getSideFor(currentRecver->getID());
        float ang = Measurments::angleBetween(recvrTarget, Point(0,0));
        currentPasser->assignBeh<OmniRandomKicker>(currentRecver);
        currentRecver->assignBeh<GenericMovementBehavior>(recvrTarget, ang);
    }
}

bool VideoStrategy1::update()
{
    return currentPasser->getCurrentBeh()->isFinished();
}

/************************************************************************/
/* VIDEO STRATEGY 2 */

VideoStrategy2::VideoStrategy2(int r0, Point pointToWaitAt)
    : robot(gameModel->findMyTeam(r0))
    , waitTimer(0)
    , waitPoint(pointToWaitAt)
    , state(NONE)
{ 
    debug::registerVariable("vs2_close_d", &CLOSE_DIST);
    debug::registerVariable("vs2_wcm", &WAIT_COUNT_MAX);
}

void VideoStrategy2::assignBeh()
{
    //Start out moving to the goal point and stopping
    float ang = Measurments::angleBetween(waitPoint, Point(0,0));
    robot->assignBeh<GenericMovementBehavior>(waitPoint, ang, false, false);
}


bool VideoStrategy2::update()
{
    Point bp = gameModel->getBallPoint();
    
    switch(state) 
    {
    case NONE:
        {
            //Advance if GameModel's ball prediciton is on the same side as the wait point
            Point prediction = gameModel->getBallPrediction();
            if((prediction.x < 0) == (waitPoint.x < 0)) 
            {
                float a = Measurments::angleBetween(robot, prediction);
                robot->assignBeh<GenericMovementBehavior>(prediction, a);
                state = WAITING;
            }
        }
        break;
    case WAITING:
        {
            //Is the ball comes near us, kick it away.
            if(Measurments::distance(robot, bp) < CLOSE_DIST) {
                Point kickTo = waitPoint;
                kickTo.x = -kickTo.x;
                robot->assignSkill<Skill::KickToPointOmni>(kickTo);
                state = KICKING;
            }
            //Abort strategy if ball does not arrive near us. Prediction wrong.
            else if (++waitTimer > WAIT_COUNT_MAX) {
                return true;
            }
        }
        break;
    case KICKING:
        {
            //Distroy strategy if done kicking
            if(robot->getCurrentBeh()->isFinished())
                return true;
        }
        break;
    }
    return false;
}

/************************************************************************/
/* VIDEO STRATEGY 3 */

VideoStrategy3::VideoStrategy3(int who)
    : guy(gameModel->findMyTeam(who))
    { }

void VideoStrategy3::assignBeh()
{
    //Start out going to the penalty point
    float ang = Measurments::angleBetween(guy, Point(0,0));
    guy->assignBeh<GenericMovementBehavior>(gameModel->getPenaltyPoint(), ang, false, false);
}

bool VideoStrategy3::update()
{
    Point bp = gameModel->getBallPoint();
    switch(state)
    {
    case NONE:
        //Is the ball comes on our side, go for it.
        if(bp.x > 0) {
            guy->assignSkill<Skill::KickToPointOmni>(gameModel->getMyGoal());
            state = KICKING;
        }
        break;
    case KICKING:
        //If the ball goes back on the other side, destroy strategy.
        //Or if we're done kicking, destroy strategy.
        if((bp.x < 0) or guy->getCurrentBeh()->isFinished()) {
            return true;
        }
        break;
    }
    return false;
}

/************************************************************************/
/* VIDEO STRATEGY 4 */

VideoStrategy4::VideoStrategy4(int who)
    : guy(gameModel->findMyTeam(who))
    { }

void VideoStrategy4::assignBeh()
{
    //Go to an default wait position
    guy->assignBeh<GenericMovementBehavior>(Point(-2000,0), 0, true, true);
}

bool VideoStrategy4::update()
{
    bp = gameModel->getBallPoint();
    ball_near_goal = bp.x>1500 and bp.y <1000 and bp.y>-1000;

    // Check if the ball is near the opponent goal
    // and if we have already kicked;
    if(ball_near_goal and !done_kicking)
    {
        guy->assignSkill<Skill::KickToPointOmni>(Point(3000,0));
        done_kicking = guy->getCurrentBeh()->isFinished();
    }
    else
    {
        guy->assignBeh<GenericMovementBehavior>(Point(-2000,0), 0, true, true);
        done_kicking = !guy->getCurrentBeh()->isFinished();
    }

    return false;
}



