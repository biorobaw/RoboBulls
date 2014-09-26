#include "freekickstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "model/gamemodel.h"

FreeKickStrategy::FreeKickStrategy()
{
}

void FreeKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();

    BehaviorAssignment<KickToGoal> kickToGoalAssignment;
    kickToGoalAssignment.setSingleAssignment(true);
    kickToGoalAssignment.assignBeh(gm->getMyTeam().at(0));
}
