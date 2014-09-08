#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

/*************************************************************/
/* CLOSEDLOOPCONTROL.h **/
/* Velocity calculation base for differential-type robots */
/* Adapted from "Introduction to Autonomous Mobile Robots" by */
/* R.SIEGWART and I. NOURBAKHSH P.50-62 */
/*************************************************************/

#include <math.h>
#include <deque>
#include "include/config/globals.h"
#include "model/robot.h"

#define CLOOP_CONTROL_DEBUG 0

namespace ClosedLoopConstants
{
    const double defaultConstants[3] = {3, 16, -1.5};
    const double noSlowdownConstants[3] = {0, 8, -1.5};
    const double sharpTurnConstants[3] = {3, 25, 0};
}

//results container
struct wheelvelocities
{
    int left, right;
};

/**************************************************************/
/* ClosedLoopBase
 * Closed-Loop control in our case is defined by three constants:
 * krho, kalpha, and kbeta. Each of these have a specific effect on
 * the wheelvelocities returned.
 */
class ClosedLoopBase
{
public:
    ClosedLoopBase(double rho, double alpha, double beta)
        : krho(rho), kalpha(alpha), kbeta(beta) 
		{}

    ClosedLoopBase(const double constants[3])
        : krho(constants[0]), kalpha(constants[1]), kbeta(constants[2]) 
		{}
		
    wheelvelocities closed_loop_control
        (Robot* robot, double x_goal, double y_goal, double theta_goal = UNUSED_ANGLE_VALUE);
		
	wheelvelocities closed_loop_control
        (Robot* robot, Point goal, double theta_goal = UNUSED_ANGLE_VALUE);

private:
    static const unsigned int sizeRhoQ   = 800;/*original value 400*/
    static const unsigned int sizeAlphaQ = 600;/*original value 300*/
    static const unsigned int sizeBetaQ  = 1000;/*original value 100*/
    static constexpr double wheel_separation = 0.115 * 1000;
    static constexpr double wheel_radius     = 0.027 * 1000;
    double newRho  , krho;
    double newAlpha, kalpha;
    double newBeta , kbeta;
    double kRhoI   = krho/(4.0 * sizeRhoQ);
    double kAlphaI = kalpha/(1.0 * sizeAlphaQ);
    double kBetaI  = kbeta/(0.2 * sizeBetaQ);

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

class ClosedLoopSharpTurns : public ClosedLoopBase {
public:
    ClosedLoopSharpTurns()
    : ClosedLoopBase(ClosedLoopConstants::sharpTurnConstants){}
};

/*************************************************************/


#endif // CLOSEDLOOPCONTROL_H
