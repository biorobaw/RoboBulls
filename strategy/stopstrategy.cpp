#include <iostream>
#include "stopstrategy.h"
#include "behavior/behaviorassignment.h"

StopStrategy::StopStrategy()
{
}

void StopStrategy:: assignBeh()
{
    /* Assigns StopBehavior to all robots regardless
     * of what behavior they currently have.
     */
    BehaviorAssignment<StopBehavior> stopAssignment;
    stopAssignment.assignBeh();
}
