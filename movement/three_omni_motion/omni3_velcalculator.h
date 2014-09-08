#ifndef THREEWHEEL_VELCALCULATOR_H
#define THREEWHEEL_VELCALCULATOR_H

#include <math.h>
#include "matrixcalculator.h"
#include "model/robot.h"
#include "movement/movetype.h"

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
};

}

#endif
