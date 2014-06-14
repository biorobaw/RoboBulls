#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

#include <math.h>
#include <deque>

using namespace std;

#define CLOOP_CONTROL_DEBUG 1

//results container
struct wheelvelocities
{
    int left,right;
};

class ClosedLoopControl {
public:
    ClosedLoopControl(double x_goal, double y_goal, double theta_goal);
    // Returns the x-velocity, y-velocity, and turn-rate, given the starting and ending coordinates using closed loop control.
    static wheelvelocities closed_loop_control(double x_current,double y_current, double theta_current);
private:

    //deque is very similar to queue with the ability to access all the elements (like an array)
    //These deques are used to store the errors in rho, alpha, and beta
    deque <double> rhoQ;
    deque <double> alphaQ;
    deque <double> betaQ;

};

#endif // CLOSEDLOOPCONTROL_H
