#include <iostream>
#include "stopstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/stopbehavior.h"

using namespace std;


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
