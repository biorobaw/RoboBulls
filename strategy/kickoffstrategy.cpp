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

KickOffStrategy::KickOffStrategy(Team* _team) : Strategy(_team) {

}

void KickOffStrategy::assignBeh()
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
    if(goalie)
        goalie->assignBeh<Goalie>();
}
