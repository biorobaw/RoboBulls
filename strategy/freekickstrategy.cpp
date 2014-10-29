#include "freekickstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"
#include "behavior/defendfarfromball.h"

FreeKickStrategy::FreeKickStrategy()
{
}

void FreeKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();

    vector <Robot*> myTeam;

    myTeam = gm->getMyTeam();

    BehaviorAssignment<KickToGoal> kickToGoalAssignment;
    kickToGoalAssignment.setSingleAssignment(true);

    BehaviorAssignment<SimpleBehaviors> simpleAssignment;
    simpleAssignment.setSingleAssignment(true);

    BehaviorAssignment<DefendFarFromBall> golieAssignment;
    golieAssignment.setSingleAssignment(true);
    for (Robot* rob: myTeam)
    {
        if (rob->getID() == 5)
            golieAssignment.assignBeh(rob);
    }

    Robot *closestRobot;
    int closestRobotID;
    Point ballPoint = gm->getBallPoint();

    //Finds the closest robot to the penalty point and its ID
    if (myTeam.size() > 1)
    {
        if (myTeam.at(0)->getID() != 5)
            closestRobot = myTeam.at(0);
        else
            closestRobot = myTeam.at(1);

        for (unsigned i = 1; i < myTeam.size(); i++)
        {
            Point iPos = myTeam.at(i)->getRobotPosition();
            Point closestPos = closestRobot->getRobotPosition();
            if (Measurments::distance(iPos, ballPoint) < Measurments::distance(closestPos, ballPoint))
                closestRobot = myTeam.at(i);
        }
        closestRobotID = closestRobot->getID();
    }

    kickToGoalAssignment.assignBeh(closestRobot);  //lets the closest robot to the ball to perform the free kick

    if (myTeam.size() > 1)  // assigns simple behavior to the rest of robots
    {
        for (unsigned i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != closestRobotID && myTeam.at(i)->getID() != 5)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
}
