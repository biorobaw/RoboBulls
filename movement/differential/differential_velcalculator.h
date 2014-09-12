#ifndef DIFFERENTIAL_VELCALCULATOR_H
#define DIFFERENTIAL_VELCALCULATOR_H

#include "movement/differential/closedloopcontrol.h"

namespace Movement
{

class DifferentialCalculator
{
public:
	wheelvelocities calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);
	wheelvelocities calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);
private:
    ClosedLoopControl    clc;
    ClosedLoopSharpTurns clst;
    ClosedLoopNoSlowdown clnsd;
};

}

#endif
