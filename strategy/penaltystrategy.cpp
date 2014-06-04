#include "penaltystrategy.h"
#include "behavior/behaviorassignment.h"
#include <cstdlib>

PenaltyStrategy::PenaltyStrategy()
{
}

void PenaltyStrategy::assignBeh()
{
    GameModel* gamemodel = GameModel::getModel();

    BehaviorAssignment<PenaltyBehavior> penAssignment;
    penAssignment.setSingleAssignment(true);
    penAssignment.assignBeh(gamemodel->getMyTeam()[0]);
}
