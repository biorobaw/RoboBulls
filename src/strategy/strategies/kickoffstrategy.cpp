#include "../behaviors/genericmovementbehavior.h"
#include "../behaviors/goalie.h"
#include "model/game_state.h"
#include "kickoffstrategy.h"
#include "robot/navigation/commands/CmdGoToPose.h"
#include "model/team.h"
#include "robot/robot.h"

/******************
 *             *
 *             *
 *    3  1    |-|
 | 5          |o|
 *    4  2    |-|
 *             *
 *             *
 ******************/

KickOffStrategy::KickOffStrategy(RobotTeam* _team) : Strategy(_team) {

}

void KickOffStrategy::assignBehaviors()
{
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

    if(wall1)
        wall1->assignBeh<GenericMovementBehavior>(Point(-650, 300), 0);
    if(wall2)
        wall2->assignBeh<GenericMovementBehavior>(Point(-1600, 0), 0);
    if(attack1)
        attack1->assignBeh<GenericMovementBehavior>(Point(-650, 0), 0);
    if(attack2)
        attack2->assignBeh<GenericMovementBehavior>(Point(-650, -300), 0);

    // Goalie is a special case
    Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
    if(goalie) goalie->assignBeh<Goalie>();
}
