#include "freekickstrategy.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "model/game_state.h"
#include "behavior/goalie.h"
#include "strategy/normalgamestrategy.h"
#include "behavior/refstop.h"
#include "behavior/wall.h"
#include "behavior/markbot.h"

#include "skill/kicktopointomni.h"

#include "model/ball.h"
#include "model/field.h"

FreeKickStrategy::FreeKickStrategy(Team* _team)
    : Strategy(_team), initial_bp(Ball::getPosition())
{

}

void FreeKickStrategy::assignBeh()
{
    // Pointers to various robots
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

    // We are taking the free kick
    if ((gameState->getState() == 'F' && team->getColor() == TEAM_BLUE) ||
        (gameState->getState() == 'f' && team->getColor() == TEAM_YELLOW))
    {
        for(Robot* rob : team->getRobots())
            rob->clearBehavior();

        auto opponent_goal = Field::getGoalPosition(team->getOpponentSide());
        if(attack1)
        {
            attack1->assignSkill<Skill::KickToPointOmni>(opponent_goal);
            kicker = attack1;
        }
        else if(attack2)
        {
            attack2->assignSkill<Skill::KickToPointOmni>(opponent_goal);
            kicker = attack2;
        }
        else if(wall1)
        {
            wall1->assignSkill<Skill::KickToPointOmni>(opponent_goal);
            kicker = wall1;
        }
        else if(wall2)
        {
            wall2->assignSkill<Skill::KickToPointOmni>(opponent_goal);
            kicker = wall2;
        }

        if(wall1 && !wall1->hasBehavior())
            wall1->assignBeh<Wall>();
        if(wall2 && !wall2->hasBehavior())
            wall2->assignBeh<Wall>();
        if(attack1 && !attack1->hasBehavior())
            attack1->assignBeh<AttackMain>(attack1);
        if(attack2 && !attack2->hasBehavior())
            attack2->assignBeh<AttackSupport>(attack2);

        NormalGameStrategy::assignGoalieIfOk(team);
    }
    // We are defending against a free kick
    else if ((gameState->getState() == 'f' && team->getColor() == TEAM_BLUE)
          || (gameState->getState() == 'F' && team->getColor() == TEAM_YELLOW))
    {
        if(wall1)
            wall1->assignBeh<Wall>();
        if(wall2)
            wall2->assignBeh<Wall>();
        if(attack1)
            attack1->assignBeh<MarkBot>();
        if(attack2)
            attack2->assignBeh<MarkBot>();

        NormalGameStrategy::assignGoalieIfOk(team);
    }
}

char FreeKickStrategy::getNextStrategy()
{
    if ((kicker && kicker->getKick() > 0)
    || !Measurements::isClose(initial_bp, Ball::getPosition(), 70))
        return ' '; // Go to normal game strategy
    else
        return '\0';
}
