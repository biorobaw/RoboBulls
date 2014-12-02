#include "passballsender.h"
#include "utilities/region.h"
#include "utilities/measurments.h"
#include "skill/kick.h"
#include "skill/stop.h"

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

PassBallSender::PassBallSender() :
    GenericMovementBehavior(), kicked(0)
{
    state = movingBehind;
}

Point PassBallSender::findPassPoint(Robot* sender)
{
    GameModel *gm = GameModel::getModel();

    Region PlayerRegion;
    vector<Robot*> myTeam = gm->getMyTeam();
    Point passPoint;

    myTeamInfo.clear(); //Prevents old values from accumulating

    for (unsigned it = 0 ; it < myTeam.size(); it++)
    {
        if (sender->getID() != myTeam[it]->getID())
        {
            PlayerRegion = Region(myTeam[it]->getRobotPosition().x + R,
                                  myTeam[it]->getRobotPosition().x - R,
                                  myTeam[it]->getRobotPosition().y + R,
                                  myTeam[it]->getRobotPosition().y - R);
            playersCharactristics pch;
            pch.position = myTeam[it]->getRobotPosition();
            pch.ID = myTeam[it]->getID();
            pch.distanceToGoal = Measurments::distance(myTeam[it]->getRobotPosition(), gm->getOpponentGoal());
            pch.surroundingAppNum = PlayerRegion.numOfOpponents();
            pch.distanceToRobot = Measurments::distance(myTeam[it]->getRobotPosition(), sender->getRobotPosition());
            myTeamInfo.push_back(pch);
        }
    }

    int i;
    int lessSurroundings = 0;
    double distance;
    for (unsigned j = 0; j < myTeamInfo.size(); j++)
    {
        if (j == 0 && myTeamInfo[j].ID != 5)
        {
            lessSurroundings = myTeamInfo[j].surroundingAppNum;
            distance = myTeamInfo[j].distanceToRobot;
            i = 0;
        }
        else if (j == 0 && myTeamInfo[j].ID == 5)
        {
            lessSurroundings = myTeamInfo[1].surroundingAppNum;
            distance = myTeamInfo[1].distanceToRobot;
            i = 1;
        }
        else
        {
            if (lessSurroundings == myTeamInfo[j].surroundingAppNum &&
                    myTeamInfo[j].distanceToRobot < distance &&
                    myTeamInfo[j].ID != 5)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = j;
            }
            else if (lessSurroundings > myTeamInfo[j].surroundingAppNum &&
                     myTeamInfo[j].ID != 5)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = j;
            }
        }
    }
    float angle = Measurments::angleBetween(sender->getRobotPosition(), myTeamInfo[i].position);
    passPoint = Point(50*cos(angle)+myTeamInfo[i].position.x, 50*sin(angle)+myTeamInfo[i].position.y);

    passingPoint = passPoint;
    receiverID = myTeamInfo[i].ID;

    return passPoint;
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
    GameModel *gm = GameModel::getModel();

    Point passPoint = findPassPoint(robot);
    float angle = Measurments::angleBetween(robot->getRobotPosition(), passPoint);
    float angleInv = Measurments::angleBetween(passPoint, robot->getRobotPosition());
    Point ballPos = gm->getBallPoint();
    Point behindBall = Point(DIST*cos(angleInv)+ballPos.x, DIST*sin(angleInv)+ballPos.y);
    Point closeToBall = Point(DIST2*cos(angleInv)+ballPos.x, DIST2*sin(angleInv)+ballPos.y);

    bool angleIsRight = abs(Measurments::angleDiff(robot->getOrientation(), angle)) < ANGLE;
    bool robotCloseToBall = Measurments::distance(robot->getRobotPosition(), ballPos) < CLOSE_ENOUGH;
    bool robotCloseToBehindBall = Measurments::distance(robot->getRobotPosition(), behindBall) < CLOSE_ENOUGH;

    int lVel, rVel;
    int distance = Measurments::distance(ballPos, passPoint);

    switch(state)
    {
        case initial:
        {
            target = behindBall;
            state = movingBehind;
        }
            break;
        case movingBehind:
        {
            setMovementTargets(behindBall, angle, true);
            GenericMovementBehavior::perform(robot, Movement::Type::Default);
            if (robotCloseToBehindBall && angleIsRight)
            {
                state = approaching;
                target = ballPos;
            }
        }
            break;
        case approaching:
        {
            setMovementTargets(closeToBall, angle, false);
            GenericMovementBehavior::perform(robot, Movement::Type::Default);
//            cout << "obotCloseToBall && angleIsRight\t" <<
//                    Measurments::distance(robot->getRobotPosition(), ballPos)
//                    << "\t" << abs(Measurments::angleDiff(robot->getOrientation(), angle))/M_PI*180 << endl;
            if (robotCloseToBall && angleIsRight)
                state = kicking;
            else if (Measurments::distance(target, ballPos) > CLOSE_ENOUGH)
            {
                state = movingBehind;
                target = behindBall;
            }
        }
            break;
        case kicking:
        {
            if (distance <= 1900)
            {
                lVel = 1;
                rVel = 1;
            }
            else if (distance > 1900 && distance <= 3800)
            {
                lVel = 10;
                rVel = 10;
            }
            else if (distance > 3800)
            {
                lVel = 100;
                rVel = 100;
            }
            Skill::Kick kick(lVel, rVel);
            kick.perform(robot);
            kicked = 1;
            if (!robotCloseToBall)
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
