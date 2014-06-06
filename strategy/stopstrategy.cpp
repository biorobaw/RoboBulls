#include <iostream>
#include "stopstrategy.h"
#include "behavior/behaviorassignment.h"

StopStrategy::StopStrategy()
{
}

void StopStrategy:: assignBeh()
{
    BehaviorAssignment<StopBehavior> stopAssignment;
    stopAssignment.assignBeh();
}
