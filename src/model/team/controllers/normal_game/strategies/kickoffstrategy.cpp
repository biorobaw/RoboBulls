#include "kickoffstrategy.h"
#include "../normal_game_roles.h"

#include "model/robot/behaviors/genericmovementbehavior.h"
#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/game_state.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"
#include "model/team/team.h"
#include "model/robot/robot.h"

/******************
 *             *
 *             *
 *    3  1    |-|
 | 5          |o|
 *    4  2    |-|
 *             *
 *             *
 ******************/

KickOffStrategy::KickOffStrategy(RobotTeam* _team) : TeamStrategy(_team) {

}

void KickOffStrategy::assignBehaviors()
{
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

    if(wall1)
        wall1->setBehavior<GenericMovementBehavior>(Point(-650, 300), 0);
    if(wall2)
        wall2->setBehavior<GenericMovementBehavior>(Point(-1600, 0), 0);
    if(attack1)
        attack1->setBehavior<GenericMovementBehavior>(Point(-650, 0), 0);
    if(attack2)
        attack2->setBehavior<GenericMovementBehavior>(Point(-650, -300), 0);

    // Goalie is a special case
    Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
    if(goalie) goalie->setBehavior<Goalie>();
}

QString KickOffStrategy::getName(){
    return "kick off";
}
