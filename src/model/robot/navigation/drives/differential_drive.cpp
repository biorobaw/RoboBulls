#include "differential_drive.h"

DifferentialDrive::DifferentialDrive(double wheel_separation, double wheel_radius) :
    half_wheel_separation(wheel_separation/2), wheel_radius(wheel_radius)
{

}

Eigen::VectorXd DifferentialDrive::getWheelSpeeds(float vx, float vy, float w){
    Eigen::VectorXd result = Eigen::VectorXd(2);
    result << (vx - half_wheel_separation*w)/wheel_radius,
              (vx + half_wheel_separation*w)/wheel_radius;
    return result;
};
