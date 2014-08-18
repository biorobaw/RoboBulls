#include "gotoposebasic.h"

#include <math.h>
#include "matrixcalculator.h"

GoToPoseBasic::GoToPoseBasic()
{
}

threeWheelVels GoToPoseBasic::calcWheelVels(double x_goal, double y_goal, double theta_goal)
{
    //Current Position
    double x_current = 0;
    double y_current = 0;
    double theta_current = 0;

    //Robot Physical Properties
    double axle_length = 100;  //distance from center of mass to wheel
    double wheel_radius = 20;
    double wheel_offset = M_PI/6;

    //Transformation Matrix
    double t_matrix[3][3] = { { -sin(wheel_offset+theta_current),	  cos(wheel_offset+theta_current),	  axle_length },
                              { -sin(wheel_offset-theta_current),	 -cos(wheel_offset-theta_current),	  axle_length },
                              {               cos(theta_current),                  sin(theta_current),    axle_length }};

    //Calculation
    double wheelVels[3][1] = {{0},
                              {0},
                              {0}};

    double cartVels[3][1] = {{x_goal-x_current},
                             {y_goal-y_current},
                             {theta_goal-theta_current}};

    multiply_3x3_3x1(t_matrix,cartVels,wheelVels);

    threeWheelVels results;
    results.L = wheelVels[0][0]/wheel_radius;
    results.R = wheelVels[1][0]/wheel_radius;
    results.B = wheelVels[2][0]/wheel_radius;

    return results;
}
