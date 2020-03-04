#ifndef DIFFERENTIAL_H
#define DIFFERENTIAL_H

#include "Eigen/Dense"

#define WHEEL_LEFT 0
#define WHEEL_RIGHT 1

class Differential
{
public:
    Differential(double wheel_separation, double wheel_radius);
    Eigen::VectorXd getWheelSpeeds(float vx, float vy, float w);

private:
    double half_wheel_separation;
    double wheel_radius;

};

#endif // DIFFERENTIAL_H
