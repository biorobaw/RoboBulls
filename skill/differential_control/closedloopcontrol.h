#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

#include <math.h>
#include <deque>
#include <unordered_map>
#include "model/robot.h"

#define CLOOP_CONTROL_DEBUG 0


namespace ClosedLoopConstants
{
    constexpr double
    defaultConstants[3] = {3, 8, -1.5};

    constexpr double
    noSlowdownConstants[3] = {0, 1, -0.5};	//Placeholders
}


//Container to hold error containers
struct errorContainer;

//results container
struct wheelvelocities
{
    int left,right;
};


/* ~ ClosedLoopBase~
 * Closed-Loop control in our case is defined by three constants:
 * krho, kalpha, and kbeta. Each of these have a specific effect on
 * the wheelvelocities returned. Each may affect the width of the curves taken, 
 * how quickly each is taken, and etc.
 * The ClosedLoopBase is a general template class, taking three constants.
 */
template<const double constants[3]>
class ClosedLoopBase 
{
public:
    static wheelvelocities 
	closed_loop_control(Robot* robot, double x_goal, double y_goal, double theta_goal);

private:
	/* Block O' Constants */
	static constexpr double krho   = constants[0];
    static constexpr double kalpha = constants[1];
    static constexpr double kbeta  = constants[2];
	
	static constexpr unsigned int sizeRhoQ = 400;
    static constexpr unsigned int sizeAlphaQ = 300;
    static constexpr unsigned int sizeBetaQ = 100;
	
    static constexpr double kRhoI   = krho/(4*sizeRhoQ);
    static constexpr double kAlphaI = kalpha/(1*sizeAlphaQ);
    static constexpr double kBetaI  = kbeta/(.2*sizeBetaQ);

	static constexpr double wheel_separation = 0.115 * 1000;
	static constexpr double wheel_radius = 0.027 * 1000; 
	

	/* Map of each robot's id to its respective error container */
    static std::unordered_map<int, errorContainer> errorContainerMap;
};

/***********************************/

typedef ClosedLoopBase< ClosedLoopConstants::defaultConstants >
    ClosedLoopControl;

typedef ClosedLoopBase< ClosedLoopConstants::noSlowdownConstants >
    ClosedLoopNoSlowdown;

	
#include "closedloopcontrol.cpp"


#endif // CLOSEDLOOPCONTROL_H

