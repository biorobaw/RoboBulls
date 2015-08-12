#ifndef THREEWHEEL_VELCALCULATOR_H
#define THREEWHEEL_VELCALCULATOR_H
#include <cmath>
#include <vector>
#include "model/robot.h"
#include "movement/movetype.h"
#include "utilities/measurments.h"

#define THREE_WHEEL_DEBUG 0

namespace Movement
{

struct threeWheelVels
{
    int L, R, B;
};

class ThreeWheelCalculator
{
public: 
	threeWheelVels calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);
	threeWheelVels calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);

private:
    const double wheel_radius = 27;
    unsigned int max_mtr_spd = 100;
    double distance_to_goal, angle_to_goal;

    threeWheelVels defaultCalc
        (Robot* rob, float x_goal, float y_goal, float theta_goal);
    threeWheelVels facePointCalc
        (Robot* rob, float x_goal, float y_goal, float theta_goal);
    std::vector<double> calcBias(double x, double y);
};

}

#endif
