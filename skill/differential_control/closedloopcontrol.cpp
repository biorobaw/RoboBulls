#ifndef CLOSEDLOOPCONTROL_CPP
#define CLOSEDLOOPCONTROL_CPP

#include <cmath>
#include <algorithm>
#include <iostream>
#include "closedloopcontrol.h"
#include "utilities/measurments.h"
#include "include/globals.h"


/* The errorContainer contains three pairs (std::pair) --
 * the first of each is a deque containing a double
 * (rhoQ, alphaQ, betaQ). The second is the continuous sum 
 * of the each containers.
 */
struct errorContainer
{
	Point lastTargetPoint;
	std::pair<std::deque<double>, double> containers[3];
};


/* Static map of each Robot's ID to their respective error containers. These are structures
 * containing the robot's previous target point, and also pairs containing a deque and a double; 
 * the deque is the error container itself, and the double is the sum of the deque. We're keeping 
 * the sum because it is much more efficient.
 * deque is very similar  to queue with the ability to access all the elements (like an array).
 *
 * auto& robErrorContainer = errorContainerMap[rob->getID()];
 *
 * robErrorContainer.containers[0].first 	is rhoQ  for rob
 * robErrorContainer.containers[1].first 	is alphaQ
 * robErrorContainer.containers[2].first 	is betaQ
 *
 * errorContainer.first is the deque itself, and errorContainer.second is the continuous sum
 */
template<const double constants[3]>
std::unordered_map<int, errorContainer> ClosedLoopBase<constants>::errorContainerMap;

template<const double constants[3]>
wheelvelocities ClosedLoopBase<constants>::closed_loop_control(Robot* robot, double x_goal, double y_goal, double theta_goal)
{
	double left_motor_velocity, right_motor_velocity;
	
	//*******************************************************************************************
	//*******************************************************************************************
	/* Get initial information about the robot. Were previously parameters
	 * for this function.*/
	Point robotPos	   = robot->getRobotPosition();
	double x_current	 = robotPos.x;
	double y_current	 = robotPos.y;
	double theta_current = robot->getOrientation();
	double angleToGoal   = atan2 ((y_goal-y_current) , (x_goal-x_current));

	//Rho, Alpha, Beta
	double rho   = sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2));
	double alpha = Measurments::angleDiff(theta_current, angleToGoal);
	double beta  = Measurments::angleDiff(angleToGoal, theta_goal);


#if CLOOP_CONTROL_DEBUG
	std::cout << "rho "   << rho   			  << std::endl;
	std::cout << "alpha " << 180/M_PI * alpha << std::endl;
	
	std::cout << "current theta " << 180/M_PI * theta_current << std::endl;
	std::cout << "beta "  		  << 180/M_PI * beta 		  << std::endl;
	std::cout << "Angle diff " 	  << 180/M_PI * Measurments::angleDiff(theta_current, theta_goal)
			  << std::endl;
#endif


	//*******************************************************************************************
	//*******************************************************************************************
	/* Storing a defined number of most errors for rho, beta, and alpha in the queue and calculate the sum of errors.
	 * Instead of looping hundreds of times to add up the entire sum over and over, it can simply be adjusted on the 
	 * change of the error containers */
	
	double newValues[3]	 = {rho, alpha, beta};
	double summedErrors[3]  = {0, 0, 0};
	static unsigned errQsizes[3] = {sizeRhoQ, sizeAlphaQ, sizeBetaQ};

	//First retrieve the errorContainer for the robot
	errorContainer& robErrorCntr = errorContainerMap[robot->getID()];

	Point& lastTarget = robErrorCntr.lastTargetPoint;

	/* Check if the target point is same as a previous call. The error
	 * containers are no longer valid on moving to a different point
	 */
	if(!Measurments::isClose(lastTarget, Point(x_goal, y_goal), 25))
	{
		for(auto& pair : robErrorCntr.containers) {
			pair.first.clear();
			pair.second = 0;
		}
		lastTarget = Point(x_goal, y_goal);
	}


	for(int i = 0; i != 3; ++i)
	{
		std::deque<double>& errorQ
				= robErrorCntr.containers[i].first;

		double& sumErrOfQ
				= robErrorCntr.containers[i].second;
			
		summedErrors[i] = sumErrOfQ;
		
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
		else
			std::cout << i << " has too many elements!" <<endl;
	}

	
	//*******************************************************************************************
	//*******************************************************************************************
	/* Calculate and set left and right motor velocity. This is dependent on rho being > 100 or not. */
	
	double sumErrRho   = summedErrors[0];
	double sumErrAlpha = summedErrors[1];
	double sumErrBeta  = summedErrors[2];
	
	double robot_xvel 	  = OVERALL_VELOCITY * (krho*rho + kRhoI*sumErrRho);
	double robot_turnrate = OVERALL_VELOCITY * (kalpha*alpha + kbeta*beta + kAlphaI*sumErrAlpha + kBetaI*sumErrBeta);


	if (rho > 100)
	{
		float Pi2R = (2*M_PI)*wheel_radius;
		left_motor_velocity  = ((robot_xvel / Pi2R) - (wheel_separation * robot_turnrate/Pi2R))/2;
		right_motor_velocity = ((robot_xvel / Pi2R) + (wheel_separation * robot_turnrate/Pi2R))/2;
	}
	else
	{
		float angDiffDeg = (180 / M_PI) * Measurments::angleDiff(theta_current, theta_goal);
		left_motor_velocity  = copysign(OVERALL_VELOCITY * 0.5 * (fabs(angDiffDeg)>10), -angDiffDeg);
		right_motor_velocity = copysign(OVERALL_VELOCITY * 0.5 * (fabs(angDiffDeg)>10),  angDiffDeg);
	}


#if CLOOP_CONTROL_DEBUG
	std::cout << "v "	<< robot_xvel 		<< std::endl;
	std::cout << "w "	<< robot_turnrate  << std::endl;
	std::cout << "LMV: " << left_motor_velocity  << std::endl;
	std::cout << "RMV: " << right_motor_velocity << std::endl;
#endif


	//*******************************************************************************************
	//*******************************************************************************************
	/* Magnitude clamping in the case magnitide is >= 100. Then finally return a result */

	if (abs(left_motor_velocity) > 100 || abs(right_motor_velocity) > 100)
	{
		float maximum = max(abs(left_motor_velocity), abs(right_motor_velocity));
		float ratio = maximum / 100;

		left_motor_velocity = left_motor_velocity / ratio;
		right_motor_velocity = right_motor_velocity / ratio;
	}

	wheelvelocities result = {(int)left_motor_velocity, (int)right_motor_velocity};
	
	return result;
}

#endif

