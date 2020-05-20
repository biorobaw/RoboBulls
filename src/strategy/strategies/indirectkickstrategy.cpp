#include "indirectkickstrategy.h"
#include "model/game_state.h"
#include "utilities/region/rectangle.h"
#include "utilities/comparisons.h"

#include "../behaviors/goalie.h"
#include "../behaviors/refstop.h"
#include "../behaviors/attackmain.h"
#include "../behaviors/attacksupport.h"
#include "../behaviors/wall.h"
#include "../behaviors/markbot.h"
#include "../behaviors/markbot.h"
#include "robot/robot.h"

#include "model/ball.h"
#include "model/field.h"
#include "model/team.h"

IndirectKickStrategy::IndirectKickStrategy(RobotTeam* _team)
    :Strategy(_team), initial_bp(Ball::getPosition())
{

}

void IndirectKickStrategy::assignBehaviors()
{

    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);


    // We are kicking
    if ((GameState::getRefereeCommand() == 'I' && team->getColor() == ROBOT_TEAM_BLUE) ||
        (GameState::getRefereeCommand() == 'i' && team->getColor() == ROBOT_TEAM_YELLOW))
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
            attack1->assignBeh<AttackSupport>();
        if(attack2 && !attack2->hasBehavior())
            attack2->assignBeh<MarkBot>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->assignBeh<Goalie>();
    }
    // We are defending against an indirect kick
    else if ((GameState::getRefereeCommand() == 'i' && team->getColor() == ROBOT_TEAM_BLUE) ||
             (GameState::getRefereeCommand() == 'I' && team->getColor() == ROBOT_TEAM_YELLOW))
    {
        if(wall1)
            wall1->assignBeh<Wall>();
        if(wall2)
            wall2->assignBeh<Wall>();
        if(attack1)
            attack1->assignBeh<MarkBot>();
        if(attack2)
            attack2->assignBeh<MarkBot>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->assignBeh<Goalie>();
    }
}


int IndirectKickStrategy::getStatus()
{
    if ((kicker && kicker->getKick() > 0)
    || !Measurements::isClose(initial_bp, Ball::getPosition(), 70))
        return KICKING; // Go to normal game strategy
    else
        return KICKED;
}

std::string IndirectKickStrategy::getName(){
    return "indirect kick";
}
