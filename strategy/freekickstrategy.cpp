#include "freekickstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"
#include "behavior/defendfarfromball.h"
#include "include/config/team.h"

FreeKickStrategy::FreeKickStrategy()
    : kickerRobot(NULL)
{
}

void FreeKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();

    vector <Robot*> myTeam;

    myTeam = gm->getMyTeam();

    if ((gm->getGameState() == 'F' && TEAM == TEAM_BLUE)
            || (gm->getGameState() == 'f' && TEAM == TEAM_YELLOW))
    {
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


        int closestRobotID;
        Point ballPoint = gm->getBallPoint();

        /*Finds the closest robot to the ball point (assuming the
         * ball is where the robots should perform free kick)
         * and its ID
         * If there is only one robot on the field, that one robot
         * will perform the free kick
         * */
        if (myTeam.size() == 1)
            kickerRobot = myTeam.at(0);
        else if (myTeam.size() > 1)
        {
            if (myTeam.at(0)->getID() != 5)
                kickerRobot = myTeam.at(0);
            else
                kickerRobot = myTeam.at(1);

            for (unsigned i = 1; i < myTeam.size(); i++)
            {
                if (myTeam.at(i)->getID() != 5)
                {
                    Point iPos = myTeam.at(i)->getRobotPosition();
                    Point closestPos = kickerRobot->getRobotPosition();
                    if (Measurments::distance(iPos, ballPoint) < Measurments::distance(closestPos, ballPoint))
                        kickerRobot = myTeam.at(i);
                }
            }
            closestRobotID = kickerRobot->getID();
        }

        kickToGoalAssignment.assignBeh(kickerRobot);  //lets the closest robot to the ball to perform the free kick

        if (myTeam.size() > 1)  // assigns simple behavior to the rest of robots
        {
            for (unsigned i = 0; i < myTeam.size(); i++)
            {
                if (myTeam.at(i)->getID() != closestRobotID && myTeam.at(i)->getID() != 5)
                    simpleAssignment.assignBeh(myTeam.at(i));
            }
        }
    }
    else if ((gm->getGameState() == 'f' && TEAM == TEAM_BLUE)
            || (gm->getGameState() == 'F' && TEAM == TEAM_YELLOW))
    {
        BehaviorAssignment<DefendFarFromBall> golieAssignment;
        golieAssignment.setSingleAssignment(true);
        for (Robot* rob: myTeam)
        {
            if (rob->getID() == 5)
                golieAssignment.assignBeh(rob);
        }
        BehaviorAssignment<SimpleBehaviors> simpleAssignment;
        simpleAssignment.setSingleAssignment(true);
        for (unsigned i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != 5)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
}


char FreeKickStrategy::getNextStrategy()
{
    /* Here we check to see if the robot has kicked (KickToGoal's
     * "isFinished") and go to NormalGame if so
     */
    if(kickerRobot != NULL) {
        return kickerRobot->getCurrentBeh()->isFinished() ? ' ' : '\0';
    } else {
        //method to stop on opponent free kicks
    }

    return '\0';
}
