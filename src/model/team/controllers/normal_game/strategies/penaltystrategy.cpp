#include "penaltystrategy.h"
#include "../normal_game_roles.h"
#include "model/team/controllers/normal_game/behaviors/wall.h"
#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/game_state.h"
#include "model/field.h"
#include "model/team/team.h"




PenaltyStrategy::PenaltyStrategy(RobotTeam* _team) : TeamStrategy(_team) {

}

void PenaltyStrategy::assignBehaviors()
{
    // If we are taking the penalty kick
    char gs = game_state->getRefereeCommand();
    if ((gs == 7 && team->getID() == ROBOT_TEAM_BLUE) || (gs == 6 && team->getID() == ROBOT_TEAM_YELLOW))
    {
        // One of the defenders will take the penalty and move back
        Robot* kicker = team->getRobotByRole(RobotRole::DEFEND1);
        if(kicker){
            auto opponent_goal = Field::getGoalPosition(OPPONENT_SIDE);
            kicker->setBehavior<GenericMovementBehavior>(Point(opponent_goal - Point(1300,0)), 0);
            Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
            if(attack1)
                attack1->setBehavior<GenericMovementBehavior>(Point(-500, 1000), 0);
        }else {
            auto opponent_goal = Field::getGoalPosition(OPPONENT_SIDE);
            Robot* kicker = team->getRobotByRole(RobotRole::ATTACK1);
            kicker->setBehavior<GenericMovementBehavior>(Point(opponent_goal - Point(1300,0)), 0);
        }

        // Position the usual attackers behind the 400 mm mark

        Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

        if(attack2)
            attack2->setBehavior<GenericMovementBehavior>(Point(-500,-1000), 0);

        // The other defender walls our goal
        Robot* def = team->getRobotByRole(RobotRole::DEFEND2);
        Robot* attack3 = team->getRobotByRole(RobotRole::ATTACK3);
        Robot* attack4 = team->getRobotByRole(RobotRole::ATTACK4);
        Robot* defender1 = team->getRobotByRole(RobotRole::DEFEND3);
        if(def)
            def->setBehavior<Wall>();
        if(attack3)
            attack3->setBehavior<Wall>();
        if(attack4)
            attack4->setBehavior<Wall>();
        if(defender1)
            defender1->setBehavior<Wall>();

        // Goalie
        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();

    }
    // If we are on the receiving end of a penalty kick
    else
    {
        auto gp = Field::getGoalPosition(OUR_SIDE);
        // All robots move behind the 400mm mark
        for(Robot* robot: team->getRobots())
        {
            if(robot != nullptr)
                robot->setBehavior<GenericMovementBehavior>(gp + Point(2000,(robot->getId() - 3)*300), 0);

        }
        // Position Goalie
        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie)
        {
            goalie->clearBehavior();
            goalie->setBehavior<GenericMovementBehavior>(gp + Point(ROBOT_RADIUS,0),0);
        }


    }
}

QString PenaltyStrategy::getName(){
    return "Penalty";
}
