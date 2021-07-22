#include "freekickstrategy.h"
#include "../normal_game_roles.h"

#include "model/team/controllers/normal_game/behaviors/attackmain.h"
#include "model/team/controllers/normal_game/behaviors/attacksupport.h"
#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/team/controllers/normal_game/behaviors/wall.h"
#include "model/team/controllers/normal_game/behaviors/markbot.h"

#include "model/game_state.h"
#include "model/team/team.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"


FreeKickStrategy::FreeKickStrategy(RobotTeam* _team)
    : TeamStrategy(_team), initial_bp(*ball)
{
}

void FreeKickStrategy::assignBehaviors()
{
    // Pointers to various robots
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* defender1 = team->getRobotByRole(RobotRole::DEFEND3);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);
    Robot* attack3 = team->getRobotByRole(RobotRole::ATTACK3);
    Robot* attack4 = team->getRobotByRole(RobotRole::ATTACK4);

    // We are taking the free kick

    if ((game_state->getRefereeCommand() == 9 && team->getID() == ROBOT_TEAM_BLUE) ||
        (game_state->getRefereeCommand() == 8 && team->getID() == ROBOT_TEAM_YELLOW))
    {
        for(Robot* rob : team->getRobots())
            rob->clearBehavior();

        auto opponent_goal = Field::getGoalPosition(OPPONENT_SIDE);
        if(attack1)
        {
            attack1->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack1;
            std::cout << "attack1 is the kicker" << std::endl;
        }
        else if(attack2)
        {
            attack2->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack2;
             std::cout << "attack2 is the kicker" << std::endl;
        }
        else if(attack3)
        {
            attack3->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack3;
             std::cout << "attack3 is the kicker" << std::endl;
        }
        else if(attack4)
        {
            attack4->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = attack4;
             std::cout << "attack4 is the kicker" << std::endl;
        }
        else if(wall1)
        {
            wall1->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = wall1;
             std::cout << "wall1 is the kicker" << std::endl;
        }
        else if(wall2)
        {
            wall2->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = wall2;
             std::cout << "wall2 is the kicker" << std::endl;
        }
        else if(defender1)
        {
            defender1->setBehavior<KickToPointOmni>(opponent_goal);
            kicker = defender1;
             std::cout << "defender1 is the kicker" << std::endl;
        }

        if(wall1 && !wall1->hasBehavior())
            wall1->setBehavior<Wall>();
        if(wall2 && !wall2->hasBehavior())
            wall2->setBehavior<Wall>();
        if(attack1 && !attack1->hasBehavior())
            attack1->setBehavior<AttackMain>();
        if(attack2 && !attack2->hasBehavior())
            attack2->setBehavior<AttackSupport>();
        if(attack3 && !attack3->hasBehavior())
            attack3->setBehavior<AttackSupport>();
        if(attack4 && !attack4->hasBehavior())
            attack4->setBehavior<AttackSupport>();
        if(defender1 && !defender1->hasBehavior())
            defender1->setBehavior<MarkBot>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();

    }
    // We are defending against a free kick
    else if ((game_state->getRefereeCommand() == 8 && team->getID() == ROBOT_TEAM_BLUE)
          || (game_state->getRefereeCommand() == 9 && team->getID() == ROBOT_TEAM_YELLOW))
    {
        if(wall1)
            wall1->setBehavior<Wall>();
        if(wall2)
            wall2->setBehavior<Wall>();
        if(attack1)
            attack1->setBehavior<MarkBot>();
        if(attack2)
            attack2->setBehavior<MarkBot>();
        if(attack3)
            attack3->setBehavior<MarkBot>();
        if(attack4)
            attack4->setBehavior<MarkBot>();
        if(defender1)
            defender1->setBehavior<Wall>();

        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();
    }
}

int FreeKickStrategy::getStatus()
{
    if ((kicker && kicker->getKickSpeed() > 0)
    || !Measurements::isClose(initial_bp, *ball, 70))
        return KICKING; // Go to normal game strategy
    else
        return KICKED;
}

QString FreeKickStrategy::getName(){
    return "free kick";
}
