#include "robottoinertialvelocities.h"

results robot_to_inertial_velocities(double robot_x_velocity, double robot_y_velocity, double robot_turnrate)
{
    double robot_frame_velocities[3][1] = {{robot_x_velocity},
                                           {robot_y_velocity},
                                           {robot_turnrate}};

    double R_inverse_function_theta[3][3] = { { cos(robot_turnrate),	 -sin(robot_turnrate),	  0 },
                                              { sin(robot_turnrate),	  cos(robot_turnrate),	  0 },
                                              { 0,	                       0,	                  1 } };

    double inertial_frame_velocities[3][1] = {{0},
                                              {0},
                                              {0}};

    multiply_3x3_3x1(R_inverse_function_theta, robot_frame_velocities, inertial_frame_velocities);

    results results = {remove_rounding_error(inertial_frame_velocities[0][0]),
                       remove_rounding_error(inertial_frame_velocities[1][0]),
                       remove_rounding_error(inertial_frame_velocities[2][0])};
    return results;
}
