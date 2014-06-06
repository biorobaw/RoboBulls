#include "closedloopcontrol.h"
#include "utilities/measurments.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "include/globals.h"
#include "skill/rotate.h"
#include <cmath>

using namespace std;

deque <double> ClosedLoopControl::rhoQ;
deque <double> ClosedLoopControl::alphaQ;
deque <double> ClosedLoopControl::betaQ;

wheelvelocities ClosedLoopControl::closed_loop_control(double x_current,double y_current, double theta_current, double x_goal, double y_goal, double theta_goal)
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

    //*******************************************************************************************
    //*******************************************************************************************
    //kRhoI, kAlphaI, kBetaI
    //Constant

    unsigned int sizeRhoQ = 400;
    unsigned int sizeAlphaQ = 300;
    unsigned int sizeBetaQ = 100;

    const double kRhoI = krho/(4*sizeRhoQ);
    const double kAlphaI = kalpha/(1*sizeAlphaQ);
    double kBetaI = kbeta/(.2*sizeBetaQ);

    //*******************************************************************************************
    //*******************************************************************************************

    //Rho, Alpha, Beta

    double angleToGoal = atan2 ((y_goal-y_current) , (x_goal-x_current));


//    double thetha_in_goal_coords = Measurments::angleDiff(-theta_current, theta_goal);

    double rho = sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2));
    double alpha = Measurments::angleDiff(theta_current, angleToGoal);
    double beta = Measurments::angleDiff(angleToGoal, theta_goal);;

#if CLOOP_CONTROL_DEBUG
    cout << "rho " << rho << endl;
//    cout << x_current << " " << y_current << " " << theta_current << endl;

    cout << "alpha " << 180 / M_PI * alpha << endl;

    cout << "current theta " << 180 / M_PI * theta_current << endl;
    cout << "beta " << 180 / M_PI * beta << endl;
    cout << "Angle diff " << 180 / M_PI *  Measurments::angleDiff(theta_current, theta_goal) << endl;
#endif

    //*******************************************************************************************
    //*******************************************************************************************
    //Storing a defined number of most errors for rho, beta, and alpha in the  queue and calculate the sum of erros

    double sumErrRho = 0;
    double sumErrAlpha = 0;
    double sumErrBeta = 0;




    if (rhoQ.size()<sizeRhoQ)
    {
        rhoQ.push_back(rho);
    }
    else if (rhoQ.size()==sizeRhoQ)
    {
        rhoQ.pop_front();
        rhoQ.push_back(rho);
    }
    else
        cout << "rhoQ has too many elements!" <<endl;

    if (alphaQ.size()<sizeAlphaQ)
    {
        alphaQ.push_back(alpha);
    }
    else if (alphaQ.size()==sizeAlphaQ)
    {
        alphaQ.pop_front();
        alphaQ.push_back(alpha);
    }
    else
        cout << "alphaQ has too many elements!" <<endl;

    if (betaQ.size()<sizeBetaQ)
    {
        betaQ.push_back(beta);
    }
    else if (betaQ.size()==sizeBetaQ)
    {
        betaQ.pop_front();
        betaQ.push_back(beta);
    }
    else
        cout << "betaQ has too many elements!" <<endl;

    for (unsigned int i=0; i < rhoQ.size(); i++)
    {
        sumErrRho += rhoQ.at(i);
    }

    for (unsigned int i=0; i < alphaQ.size(); i++)
    {
        sumErrAlpha += alphaQ.at(i);
    }

    for (unsigned int i=0; i < betaQ.size(); i++)
    {
        sumErrBeta += betaQ.at(i);
    }


    //*******************************************************************************************
    //*******************************************************************************************


    double robot_xvel = OVERALL_VELOCITY * (krho * rho + kRhoI * sumErrRho);

    double robot_turnrate = OVERALL_VELOCITY * (kalpha * alpha  + kbeta * (beta) + kAlphaI * sumErrAlpha + kBetaI * sumErrBeta);

#if CLOOP_CONTROL_DEBUG
    cout << "v " << robot_xvel << endl;
    cout << "w " << robot_turnrate << endl;
#endif

    if (rho > 100)
    {
        left_motor_velocity = (robot_xvel / (2*M_PI*wheel_radius) - (wheel_separation*robot_turnrate/(2*M_PI*wheel_radius)))/2;
        right_motor_velocity = (robot_xvel / (2*M_PI*wheel_radius) + (wheel_separation*robot_turnrate/(2*M_PI*wheel_radius)))/2;
    }
    else
    {
        if (180 / M_PI *  Measurments::angleDiff(theta_current, theta_goal) < -10)
        {
            left_motor_velocity = 5;
            right_motor_velocity = -5;
        }
        else if (180 / M_PI *  Measurments::angleDiff(theta_current, theta_goal) > 10)
        {
            left_motor_velocity = -5;
            right_motor_velocity = 5;
        }
        else
        {
            left_motor_velocity = 0;
            right_motor_velocity = 0;
        }
    }

//    cout << "LMV: " << left_motor_velocity << endl;
//    cout << "RMV: " << right_motor_velocity << endl;

    if (left_motor_velocity > 100)
        left_motor_velocity = 100;
    if (right_motor_velocity > 100)
        right_motor_velocity = 100;
    if (left_motor_velocity < -100)
        left_motor_velocity = -100;
    if (right_motor_velocity < -100)
        right_motor_velocity = -100;

    wheelvelocities result = {(int)left_motor_velocity, (int)right_motor_velocity};
    return result;
}
