#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

#include <math.h>

//results container
struct wheelvelocities
{
    int left,right;
};

// Returns the x-velocity, y-velocity, and turn-rate, given the starting and ending coordinates using closed loop control.
wheelvelocities closed_loop_control(double x_current,double y_current, double theta_current, double x_goal, double y_goal, double theta_goal);

#endif // CLOSEDLOOPCONTROL_H
