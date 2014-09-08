
#include "behavior/stopbehavior.h"

StopBehavior::StopBehavior(const ParameterList& list)
	: GenericMovementBehavior(list)
{
	/* Here, the GenericMovementBehavior takes care
	 * of the "targetPoint" entry on the ParameterList.
	 */
}


/* StopBehavior is simply a wrapper for
 * GoToPositionWithOrientation It expects a point named targetPoint
 * on the ParameterList on construction.
 * Movement type is set to SharpTurns because it works
 * Much better with pathfinding.
 */
void StopBehavior::perform(Robot * myRobot)
{
    GenericMovementBehavior::perform(myRobot, Movement::Type::SharpTurns);
}
