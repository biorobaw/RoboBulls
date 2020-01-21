#include "behavior/genericmovementbehavior.h"
#include "behavior/goalie.h"
#include "model/gamemodel.h"
#include "kickoffstrategy.h"

/******************
 *             *
 *             *
 *    3  1    |-|
 | 5          |o|
 *    4  2    |-|
 *             *
 *             *
 ******************/

void KickOffStrategy::assignBeh()
{
    auto my_team = gameModel->getMyTeam();
    Robot* wall1 = my_team.getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = my_team.getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = my_team.getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = my_team.getRobotByRole(RobotRole::ATTACK2);

    if(wall1)
        wall1->assignBeh<GenericMovementBehavior>(Point(-650, 300), 0);
    if(wall2)
        wall2->assignBeh<GenericMovementBehavior>(Point(-1600, 0), 0);
    if(attack1)
        attack1->assignBeh<GenericMovementBehavior>(Point(-650, 0), 0);
    if(attack2)
        attack2->assignBeh<GenericMovementBehavior>(Point(-650, -300), 0);

    // Goalie is a special case
    Robot* goalie = gameModel->getMyTeam().getRobotByRole(RobotRole::GOALIE);
    if(goalie)
        goalie->assignBeh<Goalie>();
}
