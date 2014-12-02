
#include "behavior/stopbehavior.h"

StopBehavior::StopBehavior
    (Point targetPoint, float targetAngle, bool obsAvoid)
    : GenericMovementBehavior(targetPoint, targetAngle, obsAvoid)
{
}


/* StopBehavior is simply a wrapper for
 * GenericMovementBehavior 
 * Movement type is set to SharpTurns because it works
 * Much better with pathfinding.
 */
void StopBehavior::perform(Robot * myRobot)
{
    GenericMovementBehavior::perform(myRobot, Movement::Type::SharpTurns);
}
