#include "haltstrategy.h"
#include "model/gamemodel.h"
#include "behavior/simplebehaviors.h"

void HaltStrategy::assignBeh()
{
    for(Robot* robot : gameModel->getMyTeam())
        robot->assignBeh<SimpleBehaviors>();
}
