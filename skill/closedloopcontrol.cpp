#include "closedloopcontrol.h"
#include <iostream>
using namespace std;

wheelvelocities closed_loop_control(double x_current,double y_current, double theta_current, double x_goal, double y_goal, double theta_goal)
{
    //*******************************************************************************************
    //*******************************************************************************************
    // Robot Parameters
    // 1 meter == 1200
    double wheel_separation = 0.115 * 1200;
    double wheel_radius = 0.027 * 1200;
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

    double rho = sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2));

    double alpha = theta_current - atan2 ((y_goal-y_current) , (x_goal-x_current)); //this might be wrong! check!

    if (rho < 100)
        alpha = 0;

    double beta = -alpha - theta_current;

    //*******************************************************************************************
    //*******************************************************************************************
    //Calculate Robot Frame Velocities
    double robot_xvel = krho * rho;

    int max_xvel = 10000;
    if ( robot_xvel > max_xvel )
            robot_xvel = 150;
    else
        robot_xvel = robot_xvel * 150 /max_xvel;

    double robot_turnrate = kalpha * alpha + kbeta * (beta+theta_goal);


    //*******************************************************************************************
    //*******************************************************************************************
    double k_turnrate = 2;
    if(rho < 50)
    {
        k_turnrate = 3;
        if(unsigned(theta_goal-theta_current) < 10 * M_PI / 180)
           k_turnrate = 0;
    }


    left_motor_velocity = (robot_xvel + (wheel_separation*robot_turnrate/wheel_radius)*k_turnrate)/2;
    right_motor_velocity = (robot_xvel - (wheel_separation*robot_turnrate/wheel_radius)*k_turnrate)/2;

//    cout << endl << "distance error: " << rho << endl;
//    cout << "direction error: " << alpha * 180 / M_PI << endl;
//    cout << "orientation error: " << (theta_goal-theta_current) * 180 / M_PI << endl;

//    cout << endl << "x-velocity: " << robot_xvel << endl;
//    cout << "Turnrate: " <<robot_turnrate << endl;
//    cout << "LMV: " << left_motor_velocity << endl;
//    cout << "RMV: " << right_motor_velocity << endl;

    if (left_motor_velocity > 100)
        left_motor_velocity = 100;
    if (right_motor_velocity > 100)
        right_motor_velocity = 100;

    wheelvelocities result = {left_motor_velocity,right_motor_velocity};
    return result;
}


