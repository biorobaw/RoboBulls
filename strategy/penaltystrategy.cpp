#include "penaltystrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/simplebehaviors.h"
#include "utilities/measurments.h"
#include "behavior/defendfarfromball.h"
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

    BehaviorAssignment<DefendFarFromBall> golieAssignment;
    golieAssignment.setSingleAssignment(true);
    for (Robot* rob: myTeam)
    {
        if (rob->getID() == 5)
            golieAssignment.assignBeh(rob);
    }


    Robot *closestRobot;
    int closestRobotID;

    //Finds the closest robot to the penalty point and its ID
    if (myTeam.size() > 1)
    {
        if (myTeam.at(0)->getID() != 5)
            closestRobot = myTeam.at(0);
        else
            closestRobot = myTeam.at(1);
        for (int i = 1; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != 5)
            {
                Point iPos = myTeam.at(i)->getRobotPosition();
                Point closestPos = closestRobot->getRobotPosition();
                if (Measurments::distance(iPos, penaltyPoint) < Measurments::distance(closestPos, penaltyPoint))
                    closestRobot = myTeam.at(i);
            }
        }
        closestRobotID = closestRobot->getID();
    }
    cout << closestRobotID << endl;

    penAssignment.assignBeh(closestRobot);  // assigns penalty behavior to closest robot to the penalty point

    if (myTeam.size() > 1)  // assigns simple behavior to the rest of robots
    {
        for (int i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != closestRobotID && myTeam.at(i)->getID() != 5)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
}
