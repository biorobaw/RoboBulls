#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H
#include <math.h>
#include <deque>
#include "include/config/globals.h"
#include "model/robot.h"

/* Code in this file is adapted from
 * "Introduction to Autonomous Mobile Robots"
 * by R.SIEGWART and I. NOURBAKHSH P.50-62 */

/*! @brief Constants for then Closed-loop control algorithm
 * @details Frequently used constant sets for closed loop control motion can be defined here.
 * Refer to the motion control folder on Robobulls Google Drive for papers on what
 * exactly the constants do. The diagram below shows how modifying the constants can
 * change the robots’ paths. The constants actually used focus on reducing the
 * distance/rho error more than the orientation error because straighter paths make
 * obstacle avoidance calculations easier. */
namespace ClosedLoopConstants
{
    const double defaultConstants[3] = {3, 12, -1.3};
    const double noSlowdownConstants[3] = {0, 8, -1.5};
    const double sharpTurnConstants[3] = {3, 25, 0};
}

//! @brief results container from CLC Algorithm
struct wheelvelocities
{
    int left, right;
};

/*! @brief Velocity calculation base for differential-type robots
 * @author Muhaimen Shamsi
 *
 * Closed-Loop control in our case is defined by three constants:
 * krho, kalpha, and kbeta. Each of these have a specific effect on
 * the wheelvelocities returned. ClosedLoopBase implements the movement algorithm. */
class ClosedLoopBase
{
public:
    ClosedLoopBase();

    ClosedLoopBase(double rho, double alpha, double beta)
        : krho(rho), kalpha(alpha), kbeta(beta)
        { ClosedLoopBase(); }

    ClosedLoopBase(const double constants[3])
        : krho(constants[0]), kalpha(constants[1]), kbeta(constants[2])
        { ClosedLoopBase(); }
		
    wheelvelocities closed_loop_control
        (Robot* robot, double x_goal, double y_goal, double theta_goal = UNUSED_ANGLE_VALUE);
		
	wheelvelocities closed_loop_control
        (Robot* robot, Point goal, double theta_goal = UNUSED_ANGLE_VALUE);

private:
    static const unsigned int sizeRhoQ   = 100;/*original value 400*/
    static const unsigned int sizeAlphaQ = 300;/*original value 300*/
    static const unsigned int sizeBetaQ  = 50;/*original value 100*/
    static constexpr double wheel_separation = 0.115 * 1000;
    static constexpr double wheel_radius     = 0.027 * 1000;
    double newRho  , krho;
    double newAlpha, kalpha;
    double newBeta , kbeta;
    double kRhoI   = krho/(5.0 * sizeRhoQ);
    double kAlphaI = kalpha/(2.0 * sizeAlphaQ);
    double kBetaI  = kbeta/(1.2 * sizeBetaQ);
    double rotateOnly = false;

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
