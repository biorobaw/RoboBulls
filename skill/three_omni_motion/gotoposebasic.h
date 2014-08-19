#ifndef GOTOPOSEBASIC_H
#define GOTOPOSEBASIC_H


#include <iostream>
#include <math.h>
#include "matrixcalculator.h"
#include "model/robot.h"

struct threeWheelVels{
    int L,R,B;
};

class GoToPoseBasic
{
public:
    GoToPoseBasic();
    threeWheelVels calcWheelVels(Robot * rob, double x_goal, double y_goal, double theta_goal);
};

#endif // GOTOPOSEBASIC_H
