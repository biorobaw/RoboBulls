#include <stdlib.h>
#include "behavior/genericskillbehavior.h"
#include "skill/stop.h"
#include "model/game_state.h"
#include "videostrategies.h"
#include "utilities/debug.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "gui/guiinterface.h"
#include "behavior/goalie.h"
#include "model/ball.h"
#include "model/field.h"

//! @cond
namespace Video
{

/************************************************************************/
/* USER CONFIGURATION */

/* ***************** OmniRandomKicker Configuration
 * X_DEV and Y_DEV define the maximum deviation in one direction that will be applied
 *      when kicking to the target. Thus the name "random." Cannot be 0.
 */
static float X_DEV = 500;
static float Y_DEV = 1000;

/* ***************** VideoStrategy2 Configuration
 * CLOSE_DIST Defines the distance the ball must be to the ball predicion point in state
 *      WAITING before the robot starts KickToPointOmni to kick it away.
 * WAIT_COUNT_MAX Defines the maximum for a counter in the WAITING state that, if hit,
 *        The robot will return to the waiting point.
 */
//#if SIMULATED
//static float CLOSE_DIST = 800;
//static float WAIT_COUNT_MAX = 500;
//#else
static float CLOSE_DIST = 800;
static float WAIT_COUNT_MAX = 2000;
//#endif

/************************************************************************/
/* BEHAVIORS */

/* KICKBEH */
class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
public:
    KickBeh(Point target) {
        ktpo = new Skill::KickToPointOmni(target);
    }
    void perform(Robot * robot) override
    {
        ktpo->perform(robot);
    }
};

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
        Point offset = Point(-X_DEV+rand()%(2*(int)X_DEV), -Y_DEV+rand()%(2*(int)Y_DEV));
        Point myGoal = Field::getPenaltyPosition(robot->getTeam()->getSide());
        Point opgoal = Field::getGoalPosition(robot->getTeam()->getOpponentSide());
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

    Point prediction = Ball::getStopPosition();
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

    float ang = Measurements::angleBetween(robot, Ball::getPosition());
    setMovementTargets(target, ang);
    GenericMovementBehavior::perform(robot);
}

/************************************************************************/
/* VIDEO STRATEGY 1 */

//Used to negate pass / receiver combo when strategy finishes
bool vs1_oppositeRobots = false;

VideoStrategy1::VideoStrategy1(Team* _team, int r1, int r2)
    : Strategy(_team)
    , r1ID(r1)
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
    Robot* r1 = team->getRobot(r1ID);
    Robot* r2 = team->getRobot(r2ID);

    if(r1 && r2)  {
        //This bool is swapped on destruction, which happens when the passer kicks.
        if(vs1_oppositeRobots) {
            this->currentPasser = r1;
            this->currentRecver = r2;
        } else {
            this->currentPasser = r2;
            this->currentRecver = r1;
        }
        
        //Behavior assign for receiver: move to side
        Point recvrTarget = getSideFor(currentRecver->getID());
        float ang = Measurements::angleBetween(recvrTarget, Point(0,0));
        currentRecver->assignBeh<GenericMovementBehavior>(recvrTarget, ang);
        
        //Assignment for passer: Kick to receiver.
        currentPasser->assignBeh<OmniRandomKicker>(currentRecver);
    }
}

bool VideoStrategy1::update()
{
    return currentPasser->getBehavior()->isFinished();
}

/************************************************************************/
/* VIDEO STRATEGY 2 */

VideoStrategy2::VideoStrategy2(Team* _team, int r0, Point pointToWaitAt)
    : Strategy(_team)
    , robot(_team->getRobot(r0))
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
    float ang = Measurements::angleBetween(waitPoint, Point(0,0));
    robot->assignBeh<GenericMovementBehavior>(waitPoint, ang, false, false);
}


bool VideoStrategy2::update()
{
    Point bp = Ball::getPosition();
    
    switch(state) 
    {
    case NONE:
        {
            //Advance if GameModel's ball prediciton is on the same side as the wait point
            Point prediction = Ball::getStopPosition();
            if((prediction.x < 0) == (waitPoint.x < 0)) 
            {
                float a = Measurements::angleBetween(robot, prediction);
                robot->assignBeh<GenericMovementBehavior>(prediction, a);
                state = WAITING;
            }
        }
        break;
    case WAITING:
        {
            //Is the ball comes near us, kick it away.
            if(Measurements::distance(robot, bp) < CLOSE_DIST) {
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
            if(robot->getBehavior()->isFinished())
                return true;
        }
        break;
    }
    return false;
}

/************************************************************************/
/* VIDEO STRATEGY 3 */

VideoStrategy3::VideoStrategy3(Team* _team,int who)
    : Strategy(_team), guy(_team->getRobot(who))
    { }

void VideoStrategy3::assignBeh()
{
    //Start out going to the penalty point
    float ang = Measurements::angleBetween(guy, Point(0,0));
    auto pp = Field::getPenaltyPosition(team->getOpponentSide());
    guy->assignBeh<GenericMovementBehavior>(pp, ang, false, false);
}

bool VideoStrategy3::update()
{
    Point bp = Ball::getPosition();
    switch(state)
    {
    case NONE:
        //Is the ball comes on our side, go for it.
        if(bp.x > 0) {
            auto gp = Field::getGoalPosition(team->getSide());
            guy->assignSkill<Skill::KickToPointOmni>(gp);
            state = KICKING;
        }
        break;
    case KICKING:
        //If the ball goes back on the other side, destroy strategy.
        //Or if we're done kicking, destroy strategy.
        if((bp.x < 0) || guy->getBehavior()->isFinished()) {
            return true;
        }
        break;
    }
    return false;
}

/************************************************************************/
/* VIDEO STRATEGY 4 */

VideoStrategy4::VideoStrategy4(Team* _team, int r1, int r2)
    : Strategy(_team)
    , r1ID(r1)
    , r2ID(r2)
    { }

void VideoStrategy4::assignBeh()
{

}

bool VideoStrategy4::update()
{
    r1 = team->getRobot(r2ID);
    r2 = team->getRobot(r1ID);

    bp = Ball::getPosition();
    bpPredict = Ball::getStopPosition();

    // For r1:
    if (bp.x>200)
        r1->assignBeh<KickBeh>(Point (-2500,0));
    else if (bpPredict.x>200)
        r1->assignBeh<GenericMovementBehavior>(Point(2500,bpPredict.y), M_PI, false, false);
    else
        r1->assignBeh<GenericMovementBehavior>(Point(2500,0), M_PI, false, false);

    // For r2:
    if (bp.x<-200)
        r2->assignBeh<KickBeh>(Point (2500,0));
    else if (bpPredict.x<-200)
        r2->assignBeh<GenericMovementBehavior>(Point(-2500,bpPredict.y), 0, false, false);
    else
        r2->assignBeh<GenericMovementBehavior>(Point(-2500,0), 0, false, false);

    return false;
}

/************************************************************************/
/* VIDEO STRATEGY 5 */

VideoStrategy5::VideoStrategy5(Team* _team,int keeper, int kicker)
    : Strategy(_team)
    , keeperID(keeper)
    , kickerID(kicker)
    { }

void VideoStrategy5::assignBeh()
{

}

bool VideoStrategy5::update()
{
    keeper = team->getRobot(keeperID);
    kicker = team->getRobot(kickerID);

    bp = Ball::getPosition();

    // GK assignments
    bp.x >-2800?
        keeper->assignBeh<Goalie>(keeper):
        keeper->assignBeh<GenericMovementBehavior>(Point(-2700,0),0,false,false);

    // Decide where the target for ATK should be based on GK position
    keeper->getPosition().y>0?
        target = Point(-3000,-350):
        target = Point(-3000,350);

    bp.x>-1800? ++kick_timer : kick_timer = 0;

    // ATK assignments
    kick_timer >100?
        kicker->assignSkill<Skill::KickToPointOmni>(target):
        kicker->assignBeh<GenericMovementBehavior>(Point(-1200,0),M_PI,false,false);

    return false;
}

}   //namespace Video

//! @endcond
