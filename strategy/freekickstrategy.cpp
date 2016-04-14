#include "freekickstrategy.h"
#include "behavior/kicktogoal.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"
#include "behavior/goaliebehavior.h"
#include "strategy/normalgamestrategy.h"
#include "include/config/team.h"

FreeKickStrategy::FreeKickStrategy()
    : kickerRobot(NULL)
{
}

void FreeKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();

    std::vector<Robot*> myTeam;

    myTeam = gm->getMyTeam();

    if ((gm->getGameState() == 'F' && TEAM == TEAM_BLUE) ||
        (gm->getGameState() == 'f' && TEAM == TEAM_YELLOW))
    {
        //Assign the goalie if he is there
        NormalGameStrategy::assignGoalieIfOk();

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
            if (myTeam.at(0)->getID() != GOALIE_ID)
                kickerRobot = myTeam.at(0);
            else
                kickerRobot = myTeam.at(1);

            for (unsigned i = 1; i < myTeam.size(); i++)
            {
                if (myTeam.at(i)->getID() != GOALIE_ID)
                {
                    Point iPos = myTeam.at(i)->getPosition();
                    Point closestPos = kickerRobot->getPosition();
                    if (Measurements::distance(iPos, ballPoint) < Measurements::distance(closestPos, ballPoint))
                        kickerRobot = myTeam.at(i);
                }
            }
            closestRobotID = kickerRobot->getID();
        }

        kickerRobot->assignBeh<KickToGoal>();   //lets the closest robot to the ball to perform the free kick

        if (myTeam.size() > 1)  // assigns simple behavior to the rest of robots
        {
            for(Robot* robot : gameModel->getMyTeam()) {
                if(robot->getID() != GOALIE_ID && robot->getID() != closestRobotID)
                    robot->assignBeh<SimpleBehaviors>();
            }
        }
    }
    else if ((gm->getGameState() == 'f' && TEAM == TEAM_BLUE)
          || (gm->getGameState() == 'F' && TEAM == TEAM_YELLOW))
    {
        //Everyone is simple
        for(Robot* robot : gameModel->getMyTeam())
            robot->assignBeh<SimpleBehaviors>();

        //Assign goalie if he is there
        NormalGameStrategy::assignGoalieIfOk();
    }
}


char FreeKickStrategy::getNextStrategy()
{
    /* Here we check to see if the robot has kicked (KickToGoal's
     * "isFinished") and go to NormalGame if so */
#if 0
    if(kickerRobot != NULL) {
        return kickerRobot->getCurrentBeh()->isFinished() ? ' ' : '\0';
    } else {
        method to stop on opponent free kicks
    }
#endif

    return '\0';
}
