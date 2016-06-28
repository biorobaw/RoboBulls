#include "behavior/genericmovementbehavior.h"
#include "behavior/goalie.h"
#include "model/gamemodel.h"
#include "kickoffstrategy.h"

/******************
 *             *
 *       0     *
 *    3       |-|
 | 5     1    |o|
 *    4       |-|
 *       2     *
 *             *
 ******************/

void KickOffStrategy::assignBeh()
{
    Robot* wall1 = gameModel->findMyTeam(DEFEND_1);
    Robot* wall2 = gameModel->findMyTeam(DEFEND_2);
    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);

    if(wall1)
        wall1->assignBeh<GenericMovementBehavior>(Point(-650, 500), 0);
    if(wall2)
        wall2->assignBeh<GenericMovementBehavior>(Point(-1600, 0), 0);
    if(attack1)
        attack1->assignBeh<GenericMovementBehavior>(Point(-650, 0), 0);
    if(attack2)
        attack2->assignBeh<GenericMovementBehavior>(Point(-650, -500), 0);

    //Goalie is a special case
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<Goalie>();
}
