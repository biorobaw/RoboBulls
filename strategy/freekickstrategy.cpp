#include "freekickstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"

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

    Robot *closestRobot = myTeam.at(0);
    int closestRobotID;
    Point ballPoint = gm->getBallPoint();

    //Finds the closest robot to the penalty point and its ID
    if (myTeam.size() > 1)
    {
        for (int i = 1; i < myTeam.size(); i++)
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
        for (int i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != closestRobotID)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
}
