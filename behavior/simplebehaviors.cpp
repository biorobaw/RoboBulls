#include "simplebehaviors.h"
#include "model/gamemodel.h"
#include "skill/stop.h"

SimpleBehaviors::SimpleBehaviors(const ParameterList& list)
{
    UNUSED_PARAM(list);
}

void SimpleBehaviors::perform(Robot * r)
{
    GameModel *gm = GameModel::getModel();

    if (gm->getGameState() == 'H')
    {
        s = new Skill::Stop();
        for (int i = 0; i < gm->getMyTeam().size(); i++)
        {
            s->perform(gm->getMyTeam().at(i));
        }
    }
}
