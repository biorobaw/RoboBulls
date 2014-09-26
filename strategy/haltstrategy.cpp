#include "haltstrategy.h"
#include "skill/skill.h"
#include "model/gamemodel.h"
#include "behavior/behaviorassignment.h"
#include "behavior/simplebehaviors.h"

HaltStrategy::HaltStrategy()
{
}

void HaltStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();

    BehaviorAssignment<SimpleBehaviors> haltAssignment;
    haltAssignment.setSingleAssignment(true);
    haltAssignment.assignBeh();
}
