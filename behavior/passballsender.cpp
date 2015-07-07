#include "passballsender.h"
#include "utilities/region.h"
#include "utilities/measurments.h"
#include "skill/kick.h"
#include "skill/stop.h"
#include "include/config/team.h"

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
    , state(movingBehind)
{
    state = movingBehind;
}

Point PassBallSender::findPassPoint(Robot* sender)
{
    GameModel *gm = GameModel::getModel();

    Region PlayerRegion;
    vector<Robot*> myTeam = gm->getMyTeam();
    Point passPoint;
    Point oppGoal = gm->getOpponentGoal();

    struct sides {  Region up;
                    Region down;
                    Region left;
                    Region right;
                 };
    sides fieldSides;
    fieldSides.up = Region(-2500, -3000, 2000, -2000);
    fieldSides.down = Region(2500, 3000, 2000, -2000);
    fieldSides.left = Region(-3000, 3000, 2000, 1500);
    fieldSides.right = Region(-3000, 3000, -2000, -1500);

    Region riskRegion;
    if (TEAM == TEAM_BLUE)
        riskRegion = Region (0, -3000, -2000, 2000);
    else
        riskRegion = Region (0, 3000, -2000, 2000);

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

            if (fieldSides.up.contains(myTeam[it]->getRobotPosition()) ||
                fieldSides.down.contains(myTeam[it]->getRobotPosition()) ||
                fieldSides.left.contains(myTeam[it]->getRobotPosition()) ||
                fieldSides.right.contains(myTeam[it]->getRobotPosition()) ||
                riskRegion.contains(myTeam[it]->getRobotPosition()))
                pch.inBadArea = true;
            else
                pch.inBadArea = false;
            myTeamInfo.push_back(pch);
        }
    }

    int i;
    int lessSurroundings = -1;
    double distance;
    for (unsigned j = 0; j < myTeamInfo.size(); j++)
    {
        if (j == 0 && myTeamInfo[j].ID != GOALIE_ID)
        {
            if (!myTeamInfo[j].inBadArea)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = 0;
            }
        }
        else if (j == 0 && myTeamInfo[j].ID == GOALIE_ID)
        {
            if (!myTeamInfo[j].inBadArea)
            {
                lessSurroundings = myTeamInfo[1].surroundingAppNum;
                distance = myTeamInfo[1].distanceToRobot;
                i = 1;
            }
        }
        else
        {
            if (lessSurroundings == myTeamInfo[j].surroundingAppNum &&
                    myTeamInfo[j].distanceToRobot < distance &&
                    myTeamInfo[j].ID != GOALIE_ID)
            {
                if (!myTeamInfo[j].inBadArea)
                {
                    lessSurroundings = myTeamInfo[j].surroundingAppNum;
                    distance = myTeamInfo[j].distanceToRobot;
                    i = j;
                }
            }
            else if (lessSurroundings > myTeamInfo[j].surroundingAppNum &&
                     myTeamInfo[j].ID != GOALIE_ID)
            {
                if (!myTeamInfo[j].inBadArea)
                {
                    lessSurroundings = myTeamInfo[j].surroundingAppNum;
                    distance = myTeamInfo[j].distanceToRobot;
                    i = j;
                }
            }
        }
    }
    if (lessSurroundings >= 0)
    {
        float angle = Measurments::angleBetween(sender->getRobotPosition(), myTeamInfo[i].position);
        passPoint = Point(50*cos(angle)+myTeamInfo[i].position.x, 50*sin(angle)+myTeamInfo[i].position.y);
    }
    else
        passPoint = Point(oppGoal.x*0.5, 0);


    passingPoint = passPoint;

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
        case movingBehind:
        {
            setMovementTargets(behindBall, angle, true);
            GenericMovementBehavior::perform(robot, Movement::Type::Default);

            //Here we don't move until robots have gotten out of the way
            bool waiterDone = waitingRobot->getCurrentBeh()->isFinished();
            if (waiterDone && robotCloseToBehindBall && angleIsRight)
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
            if (!robotCloseToBall)
                state = idling;
        }
        break;
        case idling:
        {
            kicked = 1;
            Skill::Stop stop;
            stop.perform(robot);
        }
            break;
    }
}

bool PassBallSender::isFinished()
{
    return kicked;
}
