#include "passballreceiver.h"
#include "utilities/measurments.h"
#include "behavior/passballsender.h"
#include "skill/stop.h"
#include "skill/kick.h"

#if SIMULATED
    #define ANGLE   (10 * M_PI/180)
    #define CLOSE_ENOUGH 110
    #define DIST   50
#else
    #define ANGLE   (15 * M_PI/180)
    #define CLOSE_ENOUGH 280
    #define DIST 220
#endif

GameModel *gm = GameModel::getModel();
PassBallReceiver::PassBallReceiver(const ParameterList& list)
    : GenericMovementBehavior(list), ballOrg(gm->getBallPoint()), hasKicked(0)
{
    UNUSED_PARAM(list);
    state = moving;
    ballLastSeen = gm->getBallPoint();
}

void PassBallReceiver::setBallOrigin()
{
    GameModel *gm = GameModel::getModel();
    ballOrg = gm->getBallPoint();
}

void PassBallReceiver::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();

    Point ballPos = gm->getBallPoint();
    Point robotPos = robot->getRobotPosition();
    float angle = Measurments::angleBetween(ballPos, gm->getOpponentGoal());
    float angleInv = Measurments::angleBetween(gm->getOpponentGoal(), ballPos);
    bool robotCloseToball = Measurments::isClose(robotPos, ballPos, CLOSE_ENOUGH);
    bool angleIsRight = abs(Measurments::angleDiff(robot->getOrientation(), angle)) < ANGLE;
    Point behindBall = Point(DIST*cos(angleInv)+ballPos.x, DIST*sin(angleInv)+ballPos.y);

    if (Measurments::distance(ballOrg, ballPos) < CLOSE_ENOUGH ) //when ball at original position
    {
        ballLastSeen = ballPos;
    }
    else if (Measurments::distance(ballLastSeen, ballPos) > CLOSE_ENOUGH) //when ball moving after pass
    {
        ballLastSeen = ballPos;
    }
    else if (Measurments::distance(ballOrg, ballPos) > CLOSE_ENOUGH &&
             Measurments::distance(ballLastSeen, ballPos) < CLOSE_ENOUGH ) //after pass and ball stopped
    {
        ballLastSeen = ballPos;

        switch (state)
        {
            case moving:
            {
                setMovementTargets(behindBall, angle, false);
                GenericMovementBehavior::perform(robot, Movement::Type::Default);
                if (robotCloseToball && angleIsRight)
                {
                    state = kicking;
                }
            }
                break;
            case kicking:
            {
                Skill::Kick kick;
                kick.perform(robot);
                hasKicked = 1;
                if (hasKicked)
                    state = idling;
            }
                break;
            case idling:
            {
                Skill::Stop stop;
                stop.perform(robot);
            }
                break;
        }
    }
}
