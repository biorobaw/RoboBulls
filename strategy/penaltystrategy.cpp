#include "penaltystrategy.h"
#include "include/game_constants.h"

void PenaltyStrategy::assignBeh()
{
    // If we are taking the penalty kick
    char gs = gameModel->getGameState();
    if ((gs == 'P' && GameModel::OUR_TEAM == TEAM_BLUE) || (gs == 'p' && GameModel::OUR_TEAM == TEAM_YELLOW))
    {
        // One of the defenders will take the penalty and move back
        auto my_team = gameModel->getMyTeam();
        Robot* kicker = my_team.getRobotByRole(RobotRole::DEFEND1);
        if(kicker)
            kicker->assignBeh<GenericMovementBehavior>(Point(gameModel->getOppGoal() - Point(1300,0)), 0);

        // Position the usual attackers behind the 400 mm mark
        Robot* attack1 = my_team.getRobotByRole(RobotRole::ATTACK1);
        Robot* attack2 = my_team.getRobotByRole(RobotRole::ATTACK2);
        if(attack1)
            attack1->assignBeh<GenericMovementBehavior>(Point(-500, 1000), 0);
        if(attack2)
            attack2->assignBeh<GenericMovementBehavior>(Point(-500,-1000), 0);

        // The other defender walls our goal
        Robot* def = my_team.getRobotByRole(RobotRole::DEFEND2);
        if(def)
            def->assignBeh<Wall>();

        // Goalie
        NormalGameStrategy::assignGoalieIfOk();

    }
    // If we are on the receiving end of a penalty kick
    else
    {
        // All robots move behind the 400mm mark
        for(Robot* robot: gameModel->getMyTeam().getRobots())
        {
            if(robot != nullptr)
                robot->assignBeh<GenericMovementBehavior>(gameModel->getMyGoal() + Point(2000,(robot->getID() - 3)*300), 0);

        }
        // Position Goalie
        Robot* goalie = gameModel->getMyTeam().getRobotByRole(RobotRole::GOALIE);
        if(goalie)
        {
            goalie->clearBehavior();
            goalie->assignBeh<GenericMovementBehavior>(gameModel->getMyGoal() + Point(ROBOT_RADIUS,0),0);
        }


    }
}
