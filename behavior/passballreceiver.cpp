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
    #define ANGLE   (5 * M_PI/180)
    #define CLOSE_ENOUGH 210
    #define DIST 400
#endif

GameModel *gm = GameModel::getModel();
PassBallReceiver::PassBallReceiver(const ParameterList& list)
    : GenericMovementBehavior(list), hasKicked(0), detectedBall(0)
{
    UNUSED_PARAM(list);
    state = initial;
    ballLastSeen = gm->getBallPoint();
    count = 170;
    ballOrg = gm->getBallPoint();
}

void PassBallReceiver::setBallOrigin()
{
    GameModel *gm = GameModel::getModel();
    if (gm->getBallPoint().x !=0 && gm->getBallPoint().y != 0)
    {
        ballOrg = gm->getBallPoint();
        detectedBall = 1;
    }
}

void PassBallReceiver::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();

    Point ballPos = gm->getBallPoint();
    Point robotPos = robot->getRobotPosition();
    float robotOrientation = robot->getOrientation();
    float angle = Measurments::angleBetween(ballPos, gm->getOpponentGoal());
    float angleInv = Measurments::angleBetween(gm->getOpponentGoal(), ballPos);
    bool robotCloseToball = Measurments::isClose(robotPos, ballPos, CLOSE_ENOUGH);
    bool angleIsRight = abs(Measurments::angleDiff(robot->getOrientation(), angle)) < ANGLE;
    bool ballInFront = abs(Measurments::angleDiff(robotOrientation, Measurments::angleBetween(robotPos, ballPos))) < ANGLE*3;
    Point behindBall = Point(DIST*cos(angleInv)+ballPos.x, DIST*sin(angleInv)+ballPos.y);
    bool robotBehindBall = Measurments::isClose(robotPos, behindBall, CLOSE_ENOUGH);
    cout << "ballPos\t" << ballPos.toString() << endl;

//    while (!detectedBall)
//        setBallOrigin();

    if (Measurments::isClose(ballPos, robotPos, CLOSE_ENOUGH*2) || count == 0)
    {
        switch (state)
        {
            case initial:
            {
                if (robotCloseToball && angleIsRight && ballInFront)
                {
                    state = kicking;
                }
                else if (robotCloseToball && !angleIsRight && ballInFront)
                {
                    state = approaching;
                    target = ballPos;
                }
                else
                {
                    state = moving;
                    target = behindBall;
                }
            }
                break;
            case moving:
            {cout << "moving" << endl;
                setMovementTargets(behindBall, angle, true);
                GenericMovementBehavior::perform(robot, Movement::Type::Default);
                cout << "robotBehindBall && angleIsRight\t" <<
                Measurments::distance(robotPos, ballPos)
                << "\t" <<
                abs(Measurments::angleDiff(robot->getOrientation(), angle))/M_PI*180 << endl;
                if (robotBehindBall && angleIsRight)
                {
                    state = approaching;
                    target = ballPos;
                }

            }
                break;
            case approaching:
            {cout << "approaching" << endl;
                setMovementTargets(ballPos, angle, false);
                GenericMovementBehavior::perform(robot, Movement::Type::Default);
                cout << "robotCloseToball\t" << Measurments::distance(robotPos, ballPos)
                    << "\tangleIsRight\t" << abs(Measurments::angleDiff(robot->getOrientation(), angle))/M_PI*180
                    << "\tballInFront\t" << abs(Measurments::angleDiff(robotOrientation, Measurments::angleBetween(robotPos, ballPos)))/M_PI*180<< endl;
                if (robotCloseToball && angleIsRight && ballInFront)
                {
                    state = kicking;
                }
                else if (!Measurments::isClose(target, ballPos, CLOSE_ENOUGH))
                {
                    state = moving;
                    target = behindBall;
                }
            }
                break;
            case kicking:
            {cout << "kicking" << endl;
                Skill::Kick kick(100, 100);
                kick.perform(robot);
                hasKicked = 1;
//                if (hasKicked)
                if (!robotCloseToball)
                    state = idling;
            }
                break;
            case idling:
            {cout << "idling" << endl;
                Skill::Stop stop;
                stop.perform(robot);
            }
                break;
        }
    }
    if (Measurments::distance(ballOrg, ballPos) > CLOSE_ENOUGH && count != 0)
        count--;
    cout << "count\t" << count << endl;
//    cout << "distance(ballOrg, ballPos)\t" << Measurments::distance(ballOrg, ballPos) << endl;
//    cout<< "ballPos\t" << ballPos.toString() << endl;
}
