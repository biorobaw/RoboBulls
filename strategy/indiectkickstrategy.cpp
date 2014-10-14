#include "indiectkickstrategy.h"
#include "behavior/simplebehaviors.h"
#include "behavior/behaviorassignment.h"
#include "model/gamemodel.h"

IndiectKickStrategy::IndiectKickStrategy()
{
}

void IndiectKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();
    BehaviorAssignment<SimpleBehaviors> assignment;
    assignment.setSingleAssignment(true);
    assignment.assignBeh(gm->getMyTeam().at(0));

}
