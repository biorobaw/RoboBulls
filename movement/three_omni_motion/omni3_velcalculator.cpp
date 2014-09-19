#include <iostream>
#include "movement/three_omni_motion/omni3_velcalculator.h"

#include "include/config/tolerances.h"
#include "utilities/measurments.h"

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
    switch (moveType)
    {
        default:
            return defaultCalc(rob,x_goal,y_goal,theta_goal);
    }
}

threeWheelVels ThreeWheelCalculator::defaultCalc
    (Robot* rob, float x_goal, float y_goal, float theta_goal)
{
    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_to_goal = Measurments::distance(rp,gp);
    angle_to_goal = Measurments::angleBetween(rp,gp);

    //Inertial Frame Velocities
    double x_vel = (distance_to_goal)*cos(angle_to_goal);
    double y_vel = (distance_to_goal)*sin(angle_to_goal);
    double theta_vel = theta_goal-theta_current;
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 300)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
    }

    //Set tolerances
    if (Measurments::isClose(rp,gp,200)) x_vel = y_vel = 0;
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<ROT_TOLERANCE) theta_vel = 0;

    //x_vel=0;
    //y_vel=0;
    //theta_vel=-10;

    // Robot Frame Velocities
    double x_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double y_vel_robot = -sin(theta_current)*x_vel+cos(theta_current)*y_vel;

    //Wheel Velocity Calculations
    double R = -(-sin(M_PI/6)   * y_vel_robot + cos(M_PI/6)   *x_vel_robot + wheel_radius*theta_vel);
    double L = -(-sin(5*M_PI/6) * y_vel_robot + cos(5*M_PI/6) *x_vel_robot + wheel_radius*theta_vel);
    double B = -(-sin(9*M_PI/6) * y_vel_robot + cos(9*M_PI/6) *x_vel_robot + wheel_radius*theta_vel);

    //Normalize wheel velocities
    if (abs(R)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(R))*L;
        B=(max_mtr_spd/abs(R))*B;
        R=(max_mtr_spd/abs(R))*R;
    }
    if (abs(L)>max_mtr_spd)
    {
        R=(max_mtr_spd/abs(L))*R;
        B=(max_mtr_spd/abs(L))*B;
        L=(max_mtr_spd/abs(L))*L;
    }
    if (abs(B)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(B))*L;
        R=(max_mtr_spd/abs(B))*R;
        B=(max_mtr_spd/abs(B))*B;
    }


    //Create and return result container
    threeWheelVels vels;
    vels.L = L;
    vels.R = R;
    vels.B = B;
    return vels;
}
}
