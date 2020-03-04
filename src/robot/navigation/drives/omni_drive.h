#ifndef OMNI_DRIVE_H
#define OMNI_DRIVE_H

#include "Eigen/Dense"

class OmniDrive
{
public:
    // values expected in millimiters and radians
    OmniDrive(double robot_radius, double wheel_radius,
              double angle_LF, double angle_LB, double angle_RF, double angle_RB);

    // values expected in millimiters per second and radians per second
    Eigen::VectorXd getWheelSpeeds(float vx, float vy, float w) ;

private:
    Eigen::MatrixXd coefs;
    double wheel_radius;

};

#endif // FOUR_WHEEL_OMNI_H
