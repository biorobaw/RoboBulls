#include "indirectkickstrategy.h"
#include "../normal_game_roles.h"

#include "model/game_state.h"
#include "utilities/region/rectangle.h"
#include "utilities/comparisons.h"

#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/team/controllers/normal_game/behaviors/refstop.h"
#include "model/team/controllers/normal_game/behaviors/attackmain.h"
#include "model/team/controllers/normal_game/behaviors/attacksupport.h"
#include "model/team/controllers/normal_game/behaviors/wall.h"
#include "model/team/controllers/normal_game/behaviors/markbot.h"
#include "model/team/controllers/normal_game/behaviors/markbot.h"
#include "model/robot/robot.h"

#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"

IndirectKickStrategy::IndirectKickStrategy(RobotTeam* _team)
    :TeamStrategy(_team), initial_bp(*ball)
{

}

void IndirectKickStrategy::assignBehaviors()
{

    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);


    // We are kicking
    if ((game_state->getRefereeCommand() == 'I' && team->getID() == ROBOT_TEAM_BLUE) ||
        (game_state->getRefereeCommand() == 'i' && team->getID() == ROBOT_TEAM_YELLOW))
    {
        for(Robot* rob : team->getRobots())
            rob->clearBehavior();

        auto opponent_goal = Field::getGoalPosition(OPPONENT_SIDE);
        if(attack1)
        {
            attack1->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack1;
        }
        else if(attack2)
        {
            attack2->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack2;
        }
        else if(wall1)
        {
            wall1->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = wall1;
        }
        else if(wall2)
        {
            wall2->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = wall2;
        }

        if(wall1 && !wall1->hasBehavior())
            wall1->setBehavior<Wall>();
        if(wall2 && !wall2->hasBehavior())
            wall2->setBehavior<Wall>();
        if(attack1 && !attack1->hasBehavior())
            attack1->setBehavior<AttackSupport>();
        if(attack2 && !attack2->hasBehavior())
            attack2->setBehavior<MarkBot>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();
    }
    // We are defending against an indirect kick
    else if ((game_state->getRefereeCommand() == 'i' && team->getID() == ROBOT_TEAM_BLUE) ||
             (game_state->getRefereeCommand() == 'I' && team->getID() == ROBOT_TEAM_YELLOW))
    {
        if(wall1)
            wall1->setBehavior<Wall>();
        if(wall2)
            wall2->setBehavior<Wall>();
        if(attack1)
            attack1->setBehavior<MarkBot>();
        if(attack2)
            attack2->setBehavior<MarkBot>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();
    }
}


int IndirectKickStrategy::getStatus()
{
    if ((kicker && kicker->getKickSpeed() > 0)
    || !Measurements::isClose(initial_bp, *ball, 70))
        return KICKING; // Go to normal game strategy
    else
        return KICKED;
}

QString IndirectKickStrategy::getName(){
    return "indirect kick";
}
