#include "freekickstrategy.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "model/gamemodel.h"
#include "behavior/goalie.h"
#include "strategy/normalgamestrategy.h"
#include "behavior/refstop.h"
#include "behavior/wall.h"
#include "behavior/markbot.h"

#include "skill/kicktopointomni.h"
#include "include/game_constants.h"

FreeKickStrategy::FreeKickStrategy()
    :initial_bp(gameModel->getBallPoint())
{
}

void FreeKickStrategy::assignBeh()
{
    // Pointers to various robots
    auto my_team = gameModel->getMyTeam();
    Robot* wall1 = my_team.getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = my_team.getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = my_team.getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = my_team.getRobotByRole(RobotRole::ATTACK2);

    // We are taking the free kick
    if ((gameModel->getGameState() == 'F' && GameModel::OUR_TEAM == TEAM_BLUE) ||
        (gameModel->getGameState() == 'f' && GameModel::OUR_TEAM == TEAM_YELLOW))
    {
        for(Robot* rob : gameModel->getMyTeam().getRobots())
            rob->clearBehavior();

        if(attack1)
        {
            attack1->assignSkill<Skill::KickToPointOmni>(gameModel->getOppGoal());
            kicker = attack1;
        }
        else if(attack2)
        {
            attack2->assignSkill<Skill::KickToPointOmni>(gameModel->getOppGoal());
            kicker = attack2;
        }
        else if(wall1)
        {
            wall1->assignSkill<Skill::KickToPointOmni>(gameModel->getOppGoal());
            kicker = wall1;
        }
        else if(wall2)
        {
            wall2->assignSkill<Skill::KickToPointOmni>(gameModel->getOppGoal());
            kicker = wall2;
        }

        if(wall1 && !wall1->hasBehavior())
            wall1->assignBeh<Wall>();
        if(wall2 && !wall2->hasBehavior())
            wall2->assignBeh<Wall>();
        if(attack1 && !attack1->hasBehavior())
            attack1->assignBeh<AttackMain>();
        if(attack2 && !attack2->hasBehavior())
            attack2->assignBeh<AttackSupport>();

        NormalGameStrategy::assignGoalieIfOk();
    }
    // We are defending against a free kick
    else if ((gameModel->getGameState() == 'f' && GameModel::OUR_TEAM == TEAM_BLUE)
          || (gameModel->getGameState() == 'F' && GameModel::OUR_TEAM == TEAM_YELLOW))
    {
        if(wall1)
            wall1->assignBeh<Wall>();
        if(wall2)
            wall2->assignBeh<Wall>();
        if(attack1)
            attack1->assignBeh<MarkBot>();
        if(attack2)
            attack2->assignBeh<MarkBot>();

        NormalGameStrategy::assignGoalieIfOk();
    }
}

char FreeKickStrategy::getNextStrategy()
{
    if ((kicker && kicker->getKick() > 0)
    || !Measurements::isClose(initial_bp, gameModel->getBallPoint(), 70))
        return ' '; // Go to normal game strategy
    else
        return '\0';
}
