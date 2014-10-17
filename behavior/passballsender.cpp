#include "passballsender.h"
#include "utilities/region.h"
#include "utilities/measurments.h"
#include "skill/kick.h"
#include "skill/stop.h"

#if SIMULATED
    #define DIST 250
    #define ANGLE (9*M_PI/180)
    #define CLOSE_ENOUGH 110
    #define R 400
#else
    #define DIST 350
    #define ANGLE (15*M_PI/180)
    #define CLOSE_ENOUGH 210
    #define R 700
#endif

PassBallSender::PassBallSender(const ParameterList& list) :
                                kicked(0), GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
    state = movingBehind;
}

Point PassBallSender::findPassPoint(Robot* sender)
{
    GameModel *gm = GameModel::getModel();

    Region *PlayerRegion;
    vector<Robot*> myTeam = gm->getMyTeam();
    Point passPoint;

    for (int it = 0 ; it < myTeam.size(); it++)
    {
        if (sender->getID() != myTeam[it]->getID())
        {
            PlayerRegion = new Region(myTeam[it]->getRobotPosition().x + R,
                                      myTeam[it]->getRobotPosition().x - R,
                                      myTeam[it]->getRobotPosition().y + R,
                                      myTeam[it]->getRobotPosition().y - R);
            playersCharactristics pch;
            pch.position = myTeam[it]->getRobotPosition();
            pch.ID = myTeam[it]->getID();
            pch.distanceToGoal = Measurments::distance(myTeam[it]->getRobotPosition(), gm->getOpponentGoal());
            pch.surroundingAppNum = PlayerRegion->numOfOpponents();
            pch.distanceToRobot = Measurments::distance(myTeam[it]->getRobotPosition(), sender->getRobotPosition());
            myTeamInfo.push_back(pch);
        }
    }

    int i;
    int lessSurroundings = 0;
    double distance;
    for (int j = 0; j < myTeamInfo.size(); j++)
    {
        if (j == 0)
        {
            lessSurroundings = myTeamInfo[j].surroundingAppNum;
            distance = myTeamInfo[j].distanceToRobot;
            i = 0;
        }
        else
        {
            if (lessSurroundings == myTeamInfo[j].surroundingAppNum &&
                    myTeamInfo[j].distanceToRobot < distance)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = j;
            }
            else if (lessSurroundings > myTeamInfo[j].surroundingAppNum)
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
            setMovementTargets(behindBall, angle, false);
            GenericMovementBehavior::perform(robot, Movement::Type::Default);
            if (Measurments::distance(robot->getRobotPosition(), behindBall) < CLOSE_ENOUGH &&
                    abs(Measurments::angleDiff(robot->getOrientation(), angle)) < ANGLE)
            {
                state = approaching;
                target = ballPos;
            }
        }
            break;
        case approaching:
        {
            setMovementTargets(ballPos, angle, false);
            GenericMovementBehavior::perform(robot, Movement::Type::Default);
            if (Measurments::distance(robot->getRobotPosition(), ballPos) < CLOSE_ENOUGH &&
                    abs(Measurments::angleDiff(robot->getOrientation(), angle)) < ANGLE)
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
            Skill::Kick kick;
            kick.perform(robot);
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
