#include "movement/four_omni_motion/omni4_velcalculator.h"

namespace Movement
{


fourWheelVels FourWheelCalculator::calculateVels
    (Robot* rob, Point goalPoint, float theta_goal, Type moveType)
{
	return calculateVels(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}


fourWheelVels FourWheelCalculator::calculateVels
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
    fourWheelVels vels;

    UNUSED_PARAM(rob);
    UNUSED_PARAM(x_goal);
    UNUSED_PARAM(y_goal);
    UNUSED_PARAM(theta_goal);
    UNUSED_PARAM(moveType);

    return vels;
}


}
