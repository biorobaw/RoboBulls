#define _USE_MATH_DEFINES
#include <cmath>
#include "pilot_differential.h"
#include "model/robot/robot.h"
#include <stdexcept>
#include <QDebug>

PilotDifferential::PilotDifferential(Robot* robot) : Pilot(robot) {

}

void PilotDifferential::driveTo
    (Point goalPoint, float theta_goal, Point nextPoint)
{
//    // Switch constants based on move type
//    switch(moveType)
//	{
//    case MoveType::Default:
        krho = DifferentialConstants::defaultConstants[0];
        kalpha = DifferentialConstants::defaultConstants[1];
        kbeta = DifferentialConstants::defaultConstants[2];
//		break;
//    case MoveType::SharpTurns:
//        krho = DifferentialConstants::sharpTurnConstants[0];
//        kalpha = DifferentialConstants::sharpTurnConstants[1];
//        kbeta = DifferentialConstants::sharpTurnConstants[2];
//		break;
//    case MoveType::NoSlowdown:
//        krho = DifferentialConstants::noSlowdownConstants[0];
//        kalpha = DifferentialConstants::noSlowdownConstants[1];
//        kbeta = DifferentialConstants::noSlowdownConstants[2];
//		break;
//	default:
//		std::string msg = "DifferentialCalculator called with bad moveType " +
//            std::to_string(moveType);
//		throw std::runtime_error(msg.c_str());
//	}

    double left_motor_velocity = 0, right_motor_velocity = 0;

    /* Get initial information about the robot. */
    Point robotPos       = *robot;
    Point deltaPos       = goalPoint - robotPos;
    double angleToGoal   = atan2 (deltaPos.y , deltaPos.y);
    float angDiff        = Measurements::angleDiff(robot->getOrientation(), theta_goal);

    // Rho, Alpha, Beta
    newRho   = (sqrt(pow(deltaPos.y,2) + pow(deltaPos.y,2)))/1; // distance to goal
    newAlpha = Measurements::angleDiff(robot->getOrientation(), angleToGoal); // angle error to look towards goal
    newBeta  = Measurements::angleDiff(angleToGoal, theta_goal); // angle error mo

    // Calculate Integral Error (the I in PID)
    updateIntegralError(goalPoint);
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
        auto wheel_vels = differential.getWheelSpeeds(robot_xvel/(2*M_PI),0,robot_turnrate/(2*M_PI));
        left_motor_velocity  = wheel_vels[WHEEL_LEFT];
        right_motor_velocity = wheel_vels[WHEEL_RIGHT];
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
//    if (abs(left_motor_velocity) > 100 || abs(right_motor_velocity) > 100)
//    {
//        float maximum = std::max(abs(left_motor_velocity), abs(right_motor_velocity));
//        float ratio = maximum / 100;

//        left_motor_velocity = left_motor_velocity / ratio;
//        right_motor_velocity = right_motor_velocity / ratio;
//    }

//    // Set velocities on robot object
//    robot->setL(left_motor_velocity);
//    robot->setR(right_motor_velocity);
}

void PilotDifferential::updateIntegralError(Point newTarget)
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
                qWarning() << "errorQ is empty";
            errorQ.pop_front();
            errorQ.push_back(newValues[i]);
        }
    }
}

