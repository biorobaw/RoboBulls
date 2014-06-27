/*******************************************************************/
/*** CLOSEDLOOPCONTROL.cpp ***/
/*******************************************************************/
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include "closedloopcontrol.h"
#include "utilities/measurments.h"
#include "include/globals.h"

void ClosedLoopBase::setVelMultiplier(double newMultiplier)
{
	this->velMultiplier = newMultiplier;
}

void ClosedLoopBase::handleError(double x_goal, double y_goal)
{
    /* Storing a defined number of most errors for rho, beta, and alpha 
	 * in the queue and calculate the sum of errors.
     * Instead of looping hundreds of times to add up the entire sum over and over, 
	 * it can simply be adjusted on the change of the error containers
     */
    static unsigned errQsizes[3]
        = {sizeRhoQ, sizeAlphaQ, sizeBetaQ};
    double newValues[3]
        = {newRho, newAlpha, newBeta};

//    Point newTarget = Point(x_goal, y_goal);

    /* We need to reset the error containers on movement to a different point,
     * because the accumulated error is no longer valid. Then update the last point
     */
//    if( !Measurments::isClose(lastTargetPoint, newTarget, 25) )
//    {
//        for(auto& pair : this->errorContainers) {
//            pair.first.clear();
//            pair.second = 0;
//        }
//        lastTargetPoint = newTarget;
//    }

    for(int i = 0; i != 3; ++i)
    {
        std::deque<double>& errorQ = this->errorContainers[i].first;
        double& sumErrOfQ  = this->errorContainers[i].second;

        if (errorQ.size() < errQsizes[i])
        {
            sumErrOfQ += newValues[i];

            errorQ.push_back(newValues[i]);
        }
        else if(errorQ.size() == errQsizes[i])
        {
            sumErrOfQ -= errorQ.front();
            sumErrOfQ += newValues[i];

            errorQ.pop_front();
            errorQ.push_back(newValues[i]);
        }
    }
}


wheelvelocities ClosedLoopBase::closed_loop_control(Robot* robot, double x_goal, double y_goal, double theta_goal)
{
    double left_motor_velocity, right_motor_velocity;

    //*******************************************************************************************
    /* Get initial information about the robot. Were previously parameters for this function.*/
    Point robotPos       = robot->getRobotPosition();
    double x_current     = robotPos.x;
    double y_current     = robotPos.y;
    double theta_current = robot->getOrientation();
    double angleToGoal   = atan2 ((y_goal-y_current) , (x_goal-x_current));

    //Rho, Alpha, Beta
    newRho   = sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2));
    newAlpha = Measurments::angleDiff(theta_current, angleToGoal);
    newBeta  = Measurments::angleDiff(angleToGoal, theta_goal);

#if CLOOP_CONTROL_DEBUG
//    std::cout << "rho "   << newRho   			<< std::endl;
//    std::cout << "alpha " << 180/M_PI * newAlpha << std::endl;
//    std::cout << "current theta " << 180/M_PI * theta_current << std::endl;
//    std::cout << "beta "  		  << 180/M_PI * newBeta		  << std::endl;
//    std::cout << "Angle diff " 	  << 180/M_PI * Measurments::angleDiff(theta_current, theta_goal)
//              << std::endl;
#endif

    //*******************************************************************************************

    this->handleError(x_goal, y_goal);
    double newSumErrRho   = errorContainers[0].second;
    double newSumErrAlpha = errorContainers[1].second;
    double newSumErrBeta  = errorContainers[2].second;

    //*******************************************************************************************
    /* Calculate and set left and right motor velocity. This is dependant on rho being > 100 or not. */

    double robot_xvel = OVERALL_VELOCITY * velMultiplier *
        (krho*newRho + kRhoI*newSumErrRho);

    double robot_turnrate = OVERALL_VELOCITY * velMultiplier *
        (kalpha*newAlpha + kbeta*newBeta + kAlphaI*newSumErrAlpha + kBetaI*newSumErrBeta);

    if (newRho > 100)
    {
        float Pi2R = 2*M_PI*wheel_radius;
        left_motor_velocity  = ((robot_xvel / Pi2R) - (wheel_separation * robot_turnrate/Pi2R))/2;
        right_motor_velocity = ((robot_xvel / Pi2R) + (wheel_separation * robot_turnrate/Pi2R))/2;
    }
    else
    {
        float angDiffDeg = (180 / M_PI)*Measurments::angleDiff(theta_current, theta_goal);
		
		if(fabs(angDiffDeg) > 10) {
			left_motor_velocity  = copysign(OVERALL_VELOCITY * velMultiplier, -angDiffDeg);
			right_motor_velocity = copysign(OVERALL_VELOCITY * velMultiplier,  angDiffDeg);
		} else {
			left_motor_velocity = right_motor_velocity = 0;
		}
    }

    //*******************************************************************************************
    //*******************************************************************************************

    if (abs(left_motor_velocity) > 100 || abs(right_motor_velocity) > 100)
    {
        float maximum = max(abs(left_motor_velocity), abs(right_motor_velocity));
        float ratio = maximum / 100;

        left_motor_velocity = left_motor_velocity / ratio;
        right_motor_velocity = right_motor_velocity / ratio;
    }

#if CLOOP_CONTROL_DEBUG
//    std::cout << "v "    << robot_xvel 		<< std::endl;
//    std::cout << "w "    << robot_turnrate  << std::endl;
//    std::cout << "LMV: " << left_motor_velocity  << std::endl;
//    std::cout << "RMV: " << right_motor_velocity << std::endl;
#endif

    wheelvelocities result = {(int)left_motor_velocity, (int)right_motor_velocity};

    return result;
}
