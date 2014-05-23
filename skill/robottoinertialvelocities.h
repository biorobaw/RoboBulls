#ifndef RobotToInertialVelocities_h
#define RobotToInertialVelocities_h

#include "matrixcalculator.h"
#include "math.h"

struct results
{
    double inertial_x_velocity;
    double inertial_y_velocity;
    double inertial_turn_rate;
};

//Calculates the inertial velocities from reference velocities. Used in ClosedLoopControl.
results robot_to_inertial_velocities(double robot_x_velocity, double robot_y_velocity, double robot_turnrate);

#endif // RobotToInertialVelocities_h
