#include "behavior/refstop.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "behavior/goalie.h"
#include "include/config/team.h"
#include "penaltystrategy.h"

void PenaltyStrategy::assignBeh()
{
    /* Penalty task will be assigned to the robot
     * that is closest to the penalty point.
     * The rest of the robots stand at a point 40 cm
     * behind the penalty point */
    //Robot going to be sent to kick the ball
     Robot* closestRobot = NULL;

    // We only do something special if it is *our* penalty kick.
    char gs = gameModel->getGameState();
    if ((gs == 'P' && OUR_TEAM == TEAM_BLUE) || (gs == 'p' && OUR_TEAM == TEAM_YELLOW))
    {
        //Finds the closest robot to the penalty point and its ID
        closestRobot = Comparisons::distanceOpGoal().minMyTeam();

        //Assigns closest robot to kick
        closestRobot->assignSkill<Skill::KickToPointOmni>(gameModel->getOppGoal(), -1, -1, true);
    }

    //Assign simple behaviors to entire team, except for kicker and goalie
    for(Robot* robot : gameModel->getMyTeam()) {
        if (robot->getID() != GOALIE_ID && robot != closestRobot)
            robot->assignBeh<RefStop>();
    }

    //Assign Goalie if he's there
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<Goalie>();
}
