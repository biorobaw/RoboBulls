#include "kickoffstrategy.h"
#include "behavior/positionforkickoff.h"
#include "behavior/behaviorassignment.h"

KickOffStrategy::KickOffStrategy()
{
}

void KickOffStrategy::assignBeh()
{
    BehaviorAssignment<PositionForKickoff> kickOffAssignment;
    kickOffAssignment.setSingleAssignment(true);
    kickOffAssignment.assignBeh();
}
