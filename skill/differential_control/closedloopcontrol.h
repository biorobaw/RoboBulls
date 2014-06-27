#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

/*******************************************************************/
/** CLOSEDLOOPCONTROL.h **/
/*******************************************************************/

#include <math.h>
#include <deque>
#include "model/robot.h"

#define CLOOP_CONTROL_DEBUG 1

namespace ClosedLoopConstants
{
    const double defaultConstants[3] = {3, 8, -1.5};
    const double noSlowdownConstants[3] = {0, 8, -1.5};
}

//results container
struct wheelvelocities
{
    int left,right;
};

/*************************************************************/
/* ClosedLoopBase
 * Closed-Loop control in our case is defined by three constants:
 * krho, kalpha, and kbeta. Each of these have a specific effect on
 * the wheelvelocities returned.
 */
class ClosedLoopBase
{
public:
    ClosedLoopBase(double rho, double alpha, double beta)
        : krho(rho), kalpha(alpha), kbeta(beta) {}

    ClosedLoopBase(const double constants[3])
        : krho(constants[0]), kalpha(constants[1]), kbeta(constants[2]) {}

	void setVelMultiplier(double);
		
    wheelvelocities closed_loop_control
        (Robot* robot, double x_goal, double y_goal, double theta_goal);

private:
    static const unsigned int sizeRhoQ   = 400;
    static const unsigned int sizeAlphaQ = 300;
    static const unsigned int sizeBetaQ  = 100;
    static const double wheel_separation = 0.115 * 1000;
    static const double wheel_radius     = 0.027 * 1000;
    double newRho  , krho;
    double newAlpha, kalpha;
    double newBeta , kbeta;
    double kRhoI   = krho/(4.0 * sizeRhoQ);
    double kAlphaI = kalpha/(1.0 * sizeAlphaQ);
    double kBetaI  = kbeta/(0.2 * sizeBetaQ);
	double velMultiplier = 5;

    void handleError(double x_goal, double y_goal);
	
    std::pair<std::deque<double>, double> errorContainers[3];
	
    Point lastTargetPoint;
};

/*************************************************************/


class ClosedLoopControl : public ClosedLoopBase {
public:
    ClosedLoopControl()
    : ClosedLoopBase(ClosedLoopConstants::defaultConstants){}
};

class ClosedLoopNoSlowdown : public ClosedLoopBase {
public:
    ClosedLoopNoSlowdown()
    : ClosedLoopBase(ClosedLoopConstants::noSlowdownConstants){}
};


/*************************************************************/

#endif // CLOSEDLOOPCONTROL_H
