#include "model/robot.h"
#include "parameters/field.h"
#include "kick.h"

namespace Skill
{

Kick::Kick(float kickDistance)
    : kickDistance(kickDistance)
    { }

bool Kick::perform(Robot * robot)
{
    robot->setKick(getVelocityforDistance());
	return true;
}

/* Function to translate a distance in meters into an initial velocity.
 * This was obtained from a third-order interpolation of
 * the following data, from an experiment on the simulator looking
 * at distance traveled based on initial kick velocity.
 *
 * Distance(m)  Initial Kick Velocity(m/s)
 * 0.04         0.5
 * 0.172        1
 * 0.352        1.5
 * 0.62         2
 * 0.952        2.5
 * 1.34         3
 * 1.836        3.5
 * 2.376        4
 * 3.000        4.5
 * 3.768        5
 * 4.512        5.5
 */
float Kick::getVelocityforDistance()
{
    float d = kickDistance / POINTS_PER_METER;
    return 0.083*(d*d*d) - 0.759*(d*d) + 2.8812*(d) + 0.3785;
}

}
