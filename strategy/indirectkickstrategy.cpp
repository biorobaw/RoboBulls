#include "indirectkickstrategy.h"
#include "model/game_state.h"
#include "utilities/region/rectangle.h"
#include "utilities/comparisons.h"

#include "behavior/goalie.h"
#include "behavior/refstop.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/wall.h"
#include "behavior/markbot.h"
#include "normalgamestrategy.h"
#include "behavior/markbot.h"

#include "model/ball.h"
#include "model/field.h"

IndirectKickStrategy::IndirectKickStrategy(Team* _team)
    :Strategy(_team), initial_bp(Ball::getPosition())
{

}

void IndirectKickStrategy::assignBeh()
{

    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);


    // We are kicking
    if ((GameState::getState() == 'I' && team->getColor() == TEAM_BLUE) ||
        (GameState::getState() == 'i' && team->getColor() == TEAM_YELLOW))
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
            attack1->assignBeh<AttackSupport>(attack1);
        if(attack2 && !attack2->hasBehavior())
            attack2->assignBeh<MarkBot>();

        NormalGameStrategy::assignGoalieIfOk(team);
    }
    // We are defending against an indirect kick
    else if ((GameState::getState() == 'i' && team->getColor() == TEAM_BLUE) ||
             (GameState::getState() == 'I' && team->getColor() == TEAM_YELLOW))
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


char IndirectKickStrategy::getNextStrategy()
{
    if ((kicker && kicker->getKick() > 0)
    || !Measurements::isClose(initial_bp, Ball::getPosition(), 70))
        return ' '; // Go to normal game strategy
    else
        return '\0';
}
