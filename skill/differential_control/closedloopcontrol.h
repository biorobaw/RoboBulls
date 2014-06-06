#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

#include <math.h>
#include <deque>

using namespace std;

#define CLOOP_CONTROL_DEBUG 0

//results container
struct wheelvelocities
{
    int left,right;
};

class ClosedLoopControl {
public:
    // Returns the x-velocity, y-velocity, and turn-rate, given the starting and ending coordinates using closed loop control.
    static wheelvelocities closed_loop_control(double x_current,double y_current, double theta_current, double x_goal, double y_goal, double theta_goal);
private:

    //deque is very similar to queue with the ability to access all the elements (like an array)
    //These deques are used to store the errors in rho, alpha, and beta
    static deque <double> rhoQ;
    static deque <double> alphaQ;
    static deque <double> betaQ;

};

#endif // CLOSEDLOOPCONTROL_H
