/*******************************************************************/
/*** CLOSEDLOOPCONTROL.cpp ***/
/*******************************************************************/
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include "movement/differential/closedloopcontrol.h"
#include "include/config/simulated.h"
#include "utilities/measurments.h"

/* Multiplier for rotational velocity on CLC calculation.
 * It's probably better to put this here than in a include/config/tolerances.h
 * because it's only used in one file.
 */
#if SIMULATED
 #define CLC_ROTATONG_VEL 5
#else
 #define CLC_ROTATONG_VEL 1
#endif

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

#if 0
    /* We need to reset the error containers on movement to a different point,
     * because the accumulated error is no longer valid. Then update the last point
     */
	Point newTarget = Point(x_goal, y_goal);
    if( !Measurments::isClose(lastTargetPoint, newTarget, 25) )
    {
        for(auto& pair : this->errorContainers) {
            pair.first.clear();
            pair.second = 0;
        }
        lastTargetPoint = newTarget;
    }
#else
    UNUSED_PARAM(x_goal);
    UNUSED_PARAM(y_goal);
#endif

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


wheelvelocities ClosedLoopBase::closed_loop_control(Robot* robot, Point goal, double theta_goal)
{
	return closed_loop_control(robot, goal.x, goal.y, theta_goal);
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
	
	//If the theta_goal is the defult argument, use it as a signal for no specific angle
	theta_goal = (theta_goal == UNUSED_ANGLE_VALUE ? 
		Measurments::angleBetween(robotPos, Point(x_goal, y_goal)) : theta_goal);

    //Rho, Alpha, Beta
    newRho   = (sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2)))/1;
    newAlpha = Measurments::angleDiff(theta_current, angleToGoal);
    newBeta  = Measurments::angleDiff(angleToGoal, theta_goal);

#if CLOOP_CONTROL_DEBUG
	float angDiff = Measurments::angleDiff(theta_current, theta_goal);
    std::cout << "rho "   		  << newRho   				  << std::endl;
    std::cout << "alpha " 		  << 180/M_PI * newAlpha	  << std::endl;
    std::cout << "current theta " << 180/M_PI * theta_current << std::endl;
    std::cout << "beta "  		  << 180/M_PI * newBeta		  << std::endl;
    std::cout << "Angle diff " 	  << 180/M_PI * angDiff		  << std::endl;
#endif

    //*******************************************************************************************

    this->handleError(x_goal, y_goal);
    double newSumErrRho   = errorContainers[0].second;
    double newSumErrAlpha = errorContainers[1].second;
    double newSumErrBeta  = errorContainers[2].second;

    //*******************************************************************************************
    /* Calculate and set left and right motor velocity. This is dependant on rho being > 100 or not. */

    double robot_xvel = OVERALL_VELOCITY * 5 *
		(krho*newRho + kRhoI*newSumErrRho);

    double robot_turnrate = OVERALL_VELOCITY * 5 *
        (kalpha*newAlpha + kbeta*newBeta + kAlphaI*newSumErrAlpha + kBetaI*newSumErrBeta);
#if SIMULATED
    int dist_tol=70;
#else
    int dist_tol=100;
#endif
    if (newRho > dist_tol)
    {
        float Pi2R = 2*M_PI*wheel_radius;
        left_motor_velocity  = ((robot_xvel / Pi2R) - (wheel_separation * robot_turnrate/Pi2R))/2;
        right_motor_velocity = ((robot_xvel / Pi2R) + (wheel_separation * robot_turnrate/Pi2R))/2;
    }
    else
    {
        float angDiff = Measurments::angleDiff(theta_current, theta_goal);
        left_motor_velocity  = OVERALL_VELOCITY * CLC_ROTATONG_VEL*-angDiff;
        right_motor_velocity = OVERALL_VELOCITY * CLC_ROTATONG_VEL*angDiff;
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
    std::cout << "v "    << robot_xvel 			 << std::endl;
    std::cout << "w "    << robot_turnrate  	 << std::endl;
    std::cout << "LMV: " << left_motor_velocity  << std::endl;
    std::cout << "RMV: " << right_motor_velocity << std::endl;
#endif

    wheelvelocities result {(int)left_motor_velocity, (int)right_motor_velocity};
    return result;
}
