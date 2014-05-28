#include "closedloopcontrol.h"
#include "Utilities/measurments.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>


using namespace std;

wheelvelocities closed_loop_control(double x_current,double y_current, double theta_current, double x_goal, double y_goal, double theta_goal)
{
    //*******************************************************************************************
    //*******************************************************************************************
    // Robot Parameters
    // 1 meter == 1000
    double wheel_separation = 0.115 * 1000;
    double wheel_radius = 0.027 * 1000;
    double left_motor_velocity, right_motor_velocity;

    //*******************************************************************************************
    //*******************************************************************************************
    //kRho, kAlpha, kBeta

    double krho = 3;
    double kalpha = 8;
    double kbeta = -1.5;

    //*******************************************************************************************
    //*******************************************************************************************
    //Rho, Alpha, Beta

    double thetha_in_goal_coords = Measurments::angleDiff(-theta_current, theta_goal);

    double rho = sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2));

    double alpha = Measurments::angleDiff(atan2 ((y_goal-y_current) , (x_goal-x_current)),theta_current);
    cout << "alpha " << alpha << endl;

    if (rho < 100)
        alpha = 0;


    double beta = -Measurments::angleDiff(-thetha_in_goal_coords,alpha);
    cout << "beta " << beta << endl;

    double robot_xvel = krho * rho;

//    if (alpha > M_PI/2 || alpha < -M_PI/2)
//        robot_xvel = -robot_xvel;

    double robot_turnrate = kalpha * alpha + kbeta * (beta);


    left_motor_velocity = (robot_xvel / (2*M_PI*wheel_radius) + (wheel_separation*robot_turnrate/(2*M_PI*wheel_radius)))/2;
    right_motor_velocity = (robot_xvel / (2*M_PI*wheel_radius) - (wheel_separation*robot_turnrate/(2*M_PI*wheel_radius)))/2;

    cout << "LMV: " << left_motor_velocity << endl;
    cout << "RMV: " << right_motor_velocity << endl;

    if (left_motor_velocity > 100)
        left_motor_velocity = 100;
    if (right_motor_velocity > 100)
        right_motor_velocity = 100;
    if (left_motor_velocity < -100)
        left_motor_velocity = -100;
    if (right_motor_velocity < -100)
        right_motor_velocity = -100;

    wheelvelocities result = {left_motor_velocity,right_motor_velocity};
    return result;
}


