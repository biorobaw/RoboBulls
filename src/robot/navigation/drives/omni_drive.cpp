#define _USE_MATH_DEFINES
//#include <cmath>
#include "omni_drive.h"
#include <iostream>

OmniDrive::OmniDrive(double robot_radius, double wheel_radius,
                     double angle_LF, double angle_LB,
                     double angle_RB, double angle_RF)
{
    coefs = Eigen::MatrixXd(4,3);
    coefs <<  -sin(angle_LF),  cos(angle_LF) , robot_radius,
              -sin(angle_LB),  cos(angle_LB) , robot_radius,
              -sin(angle_RB),  cos(angle_RB) , robot_radius,
              -sin(angle_RF),  cos(angle_RF) , robot_radius
            ;
    coefs = coefs / wheel_radius;

//    std::cout<< "coefs:\n" << coefs << "'\n";
}


Eigen::VectorXd OmniDrive::getWheelSpeeds(float vx, float vy, float w){
    return coefs * Eigen::Vector3d(vx,vy,w);
}
