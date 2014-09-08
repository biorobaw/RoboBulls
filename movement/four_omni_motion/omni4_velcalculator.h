#ifndef FOURWHEEL_VELCALCULATOR_H
#define FOURWHEEL_VELCALCULATOR_H

#include "model/robot.h"
#include "movement/movetype.h"
#define FOUR_WHEEL_DEBUG 0

namespace Movement
{

struct fourWheelVels
{
	int LB, LF, RB, RF;
};


class FourWheelCalculator
{
public:
	fourWheelVels calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);
	fourWheelVels calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);
};

}

#endif
