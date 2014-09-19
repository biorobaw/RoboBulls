#ifndef FOURWHEEL_VELCALCULATOR_H
#define FOURWHEEL_VELCALCULATOR_H

#include <math.h>
#include <deque>
#include <iostream>

#include "model/robot.h"
#include "movement/movetype.h"
#include "include/config/globals.h"
#include "utilities/measurments.h"
#define FOUR_WHEEL_DEBUG 0

namespace Movement
{

struct fourWheelVels
{
    double LB, LF, RB, RF;
};


class FourWheelCalculator
{
public:
    FourWheelCalculator();

	fourWheelVels calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);
	fourWheelVels calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);

private:
    fourWheelVels defaultCalc(Robot* rob, float x_goal, float y_goal, float theta_goal);
    fourWheelVels facePointCalc(Robot* rob, float x_goal, float y_goal, float theta_goal);

    //Robot Physical Properties
    const double frnt_axl_offset = M_PI/6;
    const double rear_axl_offset = M_PI/4;
    const double wheel_radius = 27;
    double distance_to_goal, angle_to_goal;

    //PID
    void calc_error();
    unsigned int dist_error_maxsize=20;
    unsigned int angle_error_maxsize=20;
    double dist_error_integral=0;
    double angle_error_integral=0;
    std::deque<double> dist_error_deque;
    std::deque<double> angle_error_deque;
};

}

#endif
