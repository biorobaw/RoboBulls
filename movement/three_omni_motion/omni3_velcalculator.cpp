#include <iostream>
#include "movement/three_omni_motion/omni3_velcalculator.h"

namespace Movement
{

threeWheelVels ThreeWheelCalculator::calculateVels
    (Robot* rob, Point goalPoint, float theta_goal, Type moveType)
{
	return calculateVels(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}


threeWheelVels ThreeWheelCalculator::calculateVels
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
	UNUSED_PARAM(moveType);

    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();
	
#if THREE_WHEEL_DEBUG
	std::cout << "ID: "    << rob->getID() << std::endl;
    std::cout << "X: "     << x_current    << std::endl;
    std::cout << "Y: "     << y_current    << std::endl;
    std::cout << "Theta: " << theta_current *180/M_PI << std::endl;
#endif

    //Robot Physical Properties
    double axle_length = 100;  //distance from center of mass to wheel
    double wheel_radius = 20;
    double wheel_offset = M_PI/6;

    //Transformation Matrix
    double t_matrix[3][3] = { { -sin(wheel_offset+theta_current),	  cos(wheel_offset+theta_current),	  axle_length },
                              { -sin(wheel_offset-theta_current),	 -cos(wheel_offset-theta_current),	  axle_length },
                              {               cos(theta_current),                  sin(theta_current),    axle_length }};

    //Cartesian Velocity Matrix
    double cartVels[3][1] = {{x_goal-x_current},
                             {y_goal-y_current},
                             {theta_current-theta_goal}};

    //Result Matrix
    double wheelVels[3][1] = {{0},
                              {0},
                              {0}};

    multiply_3x3_3x1(t_matrix,cartVels,wheelVels);

    threeWheelVels results;

    results.L = wheelVels[0][0]/wheel_radius;
    results.R = -wheelVels[1][0]/wheel_radius;
    results.B = wheelVels[2][0]/wheel_radius;

    //Normalize wheel velocities
    if (abs(results.L)>100)
    {
        results.B=(100/results.L)*results.B;
        results.R=(100/results.L)*results.R;
        results.L=100;
    }
    if (abs(results.R)>100)
    {
        results.B=(100/results.R)*results.B;
        results.L=(100/results.R)*results.L;
        results.R=100;
    }
    if (abs(results.B)>100)
    {
        results.L=(100/results.B)*results.L;
        results.R=(100/results.B)*results.R;
        results.B=100;
    }

#if THREE_WHEEL_DEBUG
	std::cout << "ID: " << rob->getID()     << std::endl;
    std::cout << "  WheelL: " << results.L  << std::endl;
    std::cout << "  WheelR: " << -results.R << std::endl;
    std::cout << "  WheelB: " << results.B  << std::endl;
#endif

    return results;
}

}
