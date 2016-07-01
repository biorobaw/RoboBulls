#include "penaltystrategy.h"

void PenaltyStrategy::assignBeh()
{
    // If we are taking the penalty kick
    char gs = gameModel->getGameState();
    if ((gs == 'P' && OUR_TEAM == TEAM_BLUE) || (gs == 'p' && OUR_TEAM == TEAM_YELLOW))
    {
        // One of the defenders will take the penalty and move back
        Robot* kicker = gameModel->findMyTeam(DEFEND_1);
        if(kicker)
            kicker->assignBeh<GenericMovementBehavior>(Point(gameModel->getOppGoal() - Point(1300,0)), 0);

        // Position the usual attackers behind the 400 mm mark
        Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
        Robot* attack2 = gameModel->findMyTeam(ATTACK_2);
        if(attack1)
            attack1->assignBeh<GenericMovementBehavior>(Point(-500, 1000), 0);
        if(attack2)
            attack2->assignBeh<GenericMovementBehavior>(Point(-500,-1000), 0);

        // The other defender walls our goal
        Robot* def = gameModel->findMyTeam(DEFEND_2);
        if(def)
            def->assignBeh<Wall>();

        // Goalie
        NormalGameStrategy::assignGoalieIfOk();

    }
    // If we are on the receiving end of a penalty kick
    else
    {
        // All robots move behind the 400mm mark
        for(Robot* robot: gameModel->getMyTeam())
        {
            if(robot != nullptr)
                robot->assignBeh<GenericMovementBehavior>(gameModel->getMyGoal() + Point(2000,(robot->getID() - 3)*300), 0);

        }
        // Position Goalie
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        if(goalie)
        {
            goalie->clearBehavior();
            goalie->assignBeh<GenericMovementBehavior>(gameModel->getMyGoal() + Point(ROBOT_RADIUS,0),0);
        }


    }
}
