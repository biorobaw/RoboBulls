#ifndef FOURWHEEL_VELCALCULATOR_H
#define FOURWHEEL_VELCALCULATOR_H

#include <deque>
#include "model/robot.h"
#include "movement/movetype.h"
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
    double distance, angle_to_goal;
    void calc_error();
    unsigned int dist_error_maxsize=100;
    unsigned int angle_error_maxsize=50;

    double dist_error_integral=0;
    double angle_error_integral=0;

#if 0
    std::deque<double> dist_error_deque;
    std::deque<double> angle_error_deque;
#endif
};

}

#endif
