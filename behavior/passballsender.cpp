#include "utilities/region.h"
#include "utilities/measurments.h"
#include "skill/kick.h"
#include "skill/stop.h"
#include "skill/kicktopointomni.h"
#include "passballsender.h"

//TODO: Do we need this SIMULATED? It's not being used now
#if SIMULATED
    #define DIST 250
    #define DIST2 50
    #define ANGLE (9*M_PI/180)
    #define CLOSE_ENOUGH 110
    #define R 400
#else
    #define DIST 400
    #define DIST2 90
    #define ANGLE (15*M_PI/180)
    #define CLOSE_ENOUGH 210
    #define R 700
#endif

PassBallSender::PassBallSender(Robot* waiter)
    : kicked(0)
    , waitingRobot(waiter)
    , kickSkill(nullptr)
    , state(waiting)
{
}

PassBallSender::~PassBallSender()
{
    delete kickSkill;
}

Point PassBallSender::findPassPoint(Robot*)
{
    /* Check both halves of the opponent's side so see which
     * has the least number of enemies */
    Region topRegion = Region::topRightRegion;
    Region botRegion = Region::lowerRightRegion;
    int topEnemies = topRegion.numOfOpponents();
    int botEnemies = botRegion.numOfOpponents();
    Region best = (botEnemies < topEnemies) ? botRegion : topRegion;
    passingPoint = best.centre();
    return passingPoint;
}

void PassBallSender::setPassingPoint(Point passPoint)
{
    passingPoint = passPoint;
}

Point PassBallSender::getPassingPoint()
{
    return passingPoint;
}

void PassBallSender::perform(Robot * robot)
{
    Point passPoint = findPassPoint(robot);
    float angle = Measurements::angleBetween(robot->getPosition(), passPoint);
    float angleInv = Measurements::angleBetween(passPoint, robot->getPosition());
    Point ballPos = gameModel->getBallPoint();
    Point behindBall = Point(DIST*cos(angleInv)+ballPos.x, DIST*sin(angleInv)+ballPos.y);

    switch(state)
    {
    case waiting:
    {
        //Here we don't kick until robots have gotten out of the way
        setMovementTargets(behindBall, angle, true);
        GenericMovementBehavior::perform(robot);
        if (waitingRobot->getCurrentBeh()->isFinished()) {
            state = performing;
            target = ballPos;
            kickSkill = new Skill::KickToPointOmni(passPoint);
        }
        break;
    }
    case performing:
    {
        //Perform the KickToPointOmni. On completion, say we're completed and stop.
        if(kickSkill->perform(robot)) {
            kicked = true;
            state = stopped;
        }
        break;
    }
    case stopped:
    {
        Skill::Stop s;
        s.perform(robot);
    }
    default:
        break;
    }
}

bool PassBallSender::isFinished()
{
    return kicked;
}
