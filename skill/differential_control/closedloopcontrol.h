#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

/*******************************************************************/
/** CLOSEDLOOPCONTROL.h **/
/*******************************************************************/

#include <math.h>
#include <deque>
#include "model/robot.h"
#include "communication/robcomm.h"

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
	wheelvelocities(int l, int r, Robot* p = nullptr) 
		: left(l), right(r), robotPtr(p) 
		{}

	void sendVels() 
	{
		if(robotPtr != nullptr)
            robotPtr->setL(left);
            robotPtr->setR(right);
	}
	
    int left,right;
	Robot* robotPtr;
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

	ClosedLoopBase& setVelMultiplier(double);
		
    wheelvelocities closed_loop_control
        (Robot* robot, double x_goal, double y_goal, double theta_goal = -10);
		
	wheelvelocities closed_loop_control
        (Robot* robot, Point goal, double theta_goal = -10);

private:
    static const unsigned int sizeRhoQ   = 400;
    static const unsigned int sizeAlphaQ = 300;
    static const unsigned int sizeBetaQ  = 100;
    static constexpr double wheel_separation = 0.115 * 1000;
    static constexpr double wheel_radius     = 0.027 * 1000;
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

class ClosedLoopSharpTurns : public ClosedLoopBase {
public:
    ClosedLoopSharpTurns()
    : ClosedLoopBase(ClosedLoopConstants::sharpTurnConstants){}
};

/*************************************************************/


#endif // CLOSEDLOOPCONTROL_H
