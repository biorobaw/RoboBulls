#include "penaltystrategy.h"
#include "behavior/behaviorassignment.h"
#include <cstdlib>

PenaltyStrategy::PenaltyStrategy()
{
}

void PenaltyStrategy::assignBeh()
{
    GameModel* gamemodel = GameModel::getModel();

    /* This BeheaviorAssignment assigns only once
     * to myTeam[0]. setSingleAssignment is used to check
     * if the robot already has the behavior. If it does,
     * PenaltyBehavior is not assigned
     */
    BehaviorAssignment<PenaltyBehavior> penAssignment;
    penAssignment.setSingleAssignment(true);
    penAssignment.assignBeh(gamemodel->getMyTeam().at(0));
}
