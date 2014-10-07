#include "penaltystrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/simplebehaviors.h"
#include "utilities/measurments.h"
#include <cstdlib>

PenaltyStrategy::PenaltyStrategy()
{
}

void PenaltyStrategy::assignBeh()
{
    /* Penalty task will be assigned to the robot
     * that is closest to the penalty point.
     * The rest of the robots stand at a point 40 cm
     * behind the penalty point
     */

    GameModel* gamemodel = GameModel::getModel();

    vector <Robot*> myTeam;

    myTeam = gamemodel->getMyTeam();

    Point penaltyPoint = gamemodel->getPenaltyPoint();


    BehaviorAssignment<PenaltyBehavior> penAssignment;
    penAssignment.setSingleAssignment(true);

    BehaviorAssignment<SimpleBehaviors> simpleAssignment;
    simpleAssignment.setSingleAssignment(true);

    Robot *closestRobot = myTeam.at(0);
    int closestRobotID;

    //Finds the closest robot to the penalty point and its ID
    if (myTeam.size() > 1)
    {
        for (int i = 1; i < myTeam.size(); i++)
        {
            Point iPos = myTeam.at(i)->getRobotPosition();
            Point closestPos = closestRobot->getRobotPosition();
            if (Measurments::distance(iPos, penaltyPoint) < Measurments::distance(closestPos, penaltyPoint))
                closestRobot = myTeam.at(i);
        }
        closestRobotID = closestRobot->getID();
    }

    penAssignment.assignBeh(closestRobot);  // assigns penalty behavior to closest robot to the penalty point

    if (gamemodel->getMyTeam().size() > 1)  // assigns simple behavior to the rest of robots
    {
        for (int i = 0; i < gamemodel->getMyTeam().size(); i++)
        {
            if (gamemodel->getMyTeam().at(i)->getID() != closestRobotID)
                simpleAssignment.assignBeh(gamemodel->getMyTeam().at(i));
        }
    }
}
