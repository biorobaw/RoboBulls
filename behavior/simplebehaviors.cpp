#include "behavior/defendbehavior.h"
#include "skill/stop.h"
#include "include/config/team.h"
#include "model/gamemodel.h"
#include "strategy/normalgamestrategy.h"
#include "simplebehaviors.h"

void SimpleBehaviors::perform(Robot * r)
{
    char gs = gameModel->getGameState();

    if (gs == 'H' || gs == ' ' || gs == 'T' || gs == 't')
    {
        Skill::Stop s;
        s.perform(r);
    }
    else if(gs == 'P' || gs == 'F' || gs == 'I' )
    {

        NormalGameStrategy::moveRobotToIdleLine(r);

    }
    else if(gs == 'p' || gs == 'f' || gs == 'i')
    {
        NormalGameStrategy::moveRobotToIdleLine(r);

    }
}
