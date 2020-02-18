#define _USE_MATH_DEFINES
#include <cmath>
#include "src/movement/differential/differential_pilot.h"
#include <stdexcept>

namespace Move {

void DifferentialPilot::drive
    (Robot* rob, Point goalPoint, float theta_goal, MoveType moveType)
{
    return drive(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}

void DifferentialPilot::drive
    (Robot* rob, float x_goal, float y_goal, float theta_goal, MoveType moveType)
{
    // Switch constants based on move type
    switch(moveType)
	{
    case MoveType::Default:
        krho = DifferentialConstants::defaultConstants[0];
        kalpha = DifferentialConstants::defaultConstants[1];
        kbeta = DifferentialConstants::defaultConstants[2];
		break;
    case MoveType::SharpTurns:
        krho = DifferentialConstants::sharpTurnConstants[0];
        kalpha = DifferentialConstants::sharpTurnConstants[1];
        kbeta = DifferentialConstants::sharpTurnConstants[2];
		break;
    case MoveType::NoSlowdown:
        krho = DifferentialConstants::noSlowdownConstants[0];
        kalpha = DifferentialConstants::noSlowdownConstants[1];
        kbeta = DifferentialConstants::noSlowdownConstants[2];
		break;
	default:
		std::string msg = "DifferentialCalculator called with bad moveType " + 
            std::to_string(moveType);
		throw std::runtime_error(msg.c_str());
	}

    double left_motor_velocity = 0, right_motor_velocity = 0;

    /* Get initial information about the robot. */
    Point robotPos       = rob->getPosition();
    double x_current     = robotPos.x;
    double y_current     = robotPos.y;
    double theta_current = rob->getOrientation();
    double angleToGoal   = atan2 ((y_goal-y_current) , (x_goal-x_current));
    float angDiff        = Measurements::angleDiff(theta_current, theta_goal);

    // Rho, Alpha, Beta
    newRho   = (sqrt(pow((y_current - y_goal),2) + pow((x_current-x_goal),2)))/1;
    newAlpha = Measurements::angleDiff(theta_current, angleToGoal);
    newBeta  = Measurements::angleDiff(angleToGoal, theta_goal);

    // Calculate Integral Error (the I in PID)
    updateIntegralError(x_goal, y_goal);
    double newSumErrRho   = errorContainers[0].second;
    double newSumErrAlpha = errorContainers[1].second;
    double newSumErrBeta  = errorContainers[2].second;

    //*******************************************************************************************
    // Calculate the left and right motor velocity.
    double robot_xvel = 15 * (krho*newRho + kRhoI*newSumErrRho);
    double robot_turnrate = 20 * (kalpha*newAlpha + kbeta*newBeta + kAlphaI*newSumErrAlpha + kBetaI*newSumErrBeta);

    /*************************************************************************************************
    / If distance < 40, turn on flag to rotate only
    / Once on, this flag does not get turned off until the distance > 70
    / providing a 30 distance window to compensate for noise and small
    / translations during rotation. This calculator guarantees that the
    / robot is within 70 distance of target.*/
    if (newRho < 30) // Below this is too slow, above this is not precise enough
        rotateOnly = true;
    else
    {
        float wheel_circum = 2*M_PI*wheel_radius;
        left_motor_velocity  = ((robot_xvel / wheel_circum) - (wheel_separation * robot_turnrate/wheel_circum))/2;
        right_motor_velocity = ((robot_xvel / wheel_circum) + (wheel_separation * robot_turnrate/wheel_circum))/2;
    }

    if (newRho > 70)
        rotateOnly = false;

    if (rotateOnly)
    {
        left_motor_velocity  = -DifferentialConstants::ROTATE_VEL * angDiff;
        right_motor_velocity =  DifferentialConstants::ROTATE_VEL * angDiff;
    }

    //*******************************************************************************************
    //Normalize wheel velocities between -100 and 100 *******************************************
    if (abs(left_motor_velocity) > 100 || abs(right_motor_velocity) > 100)
    {
        float maximum = std::max(abs(left_motor_velocity), abs(right_motor_velocity));
        float ratio = maximum / 100;

        left_motor_velocity = left_motor_velocity / ratio;
        right_motor_velocity = right_motor_velocity / ratio;
    }

    // Set velocities on robot object
    rob->setL(left_motor_velocity);
    rob->setR(right_motor_velocity);
}

void DifferentialPilot::updateIntegralError(double x_goal, double y_goal)
{
    /* Store a limited history of errors for rho, beta, and alpha
     * in the queue and calculate the sum of errors.
     * Instead of looping hundreds of times to add up the entire sum over and over,
     * it can simply be adjusted on the change of the error containers
     */
    static unsigned errQsizes[3] = {sizeRhoQ, sizeAlphaQ, sizeBetaQ};
    double newValues[3] = {newRho, newAlpha, newBeta};

    /* We need to reset the error containers on movement to a different point,
     * because the accumulated error is no longer valid. Then update the last point.
     */
    Point newTarget = Point(x_goal, y_goal);
    if( !Measurements::isClose(lastTargetPoint, newTarget, 25) )
    {
        for(auto& pair : this->errorContainers) {
            pair.first.clear();
            pair.second = 0;
        }
        lastTargetPoint = newTarget;
    }

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
            if (errorQ.empty())
                std::cout << "errorQ is empty" << std::endl;
            errorQ.pop_front();
            errorQ.push_back(newValues[i]);
        }
    }
}

}   // namespace move
