#ifndef CLOSEDLOOPCONTROL_H
#define CLOSEDLOOPCONTROL_H

#include <math.h>
#include <deque>
#include <unordered_map>
#include <array>
#include <utility>
#include "model/robot.h"

#define CLOOP_CONTROL_DEBUG 0

//results container
struct wheelvelocities
{
    int left,right;
};

class ClosedLoopControl 
{
public:
    /* Returns the x-velocity, y-velocity, and turn-rate, given the starting and ending 
	 * coordinates using closed loop control.
	 */
    static wheelvelocities 
	closed_loop_control(Robot* robot, double x_goal, double y_goal, double theta_goal);

private:
	/*************************************************************/
	/* ~Closed Loop Control Constants ~
	 * Defines the constants for the control system
	 */

	// Robot Parameters (1 meter == 1000)
    static constexpr double wheel_separation = 0.115 * 1000;
    static constexpr double wheel_radius = 0.027 * 1000; 
	
    //kRho, kAlpha, kBeta
    static constexpr double krho = 3;
    static constexpr double kalpha = 8;
    static constexpr double kbeta = -1.5;
	
	//kRhoI, kAlphaI, kBetaI; Constant
	static constexpr unsigned int sizeRhoQ = 400;
    static constexpr unsigned int sizeAlphaQ = 300;
    static constexpr unsigned int sizeBetaQ = 100;
	
    static constexpr double kRhoI   = krho/(4*sizeRhoQ);
    static constexpr double kAlphaI = kalpha/(1*sizeAlphaQ);
    static constexpr double kBetaI  = kbeta/(.2*sizeBetaQ);

	
	/*************************************************************/
	
	/* Static map of each Robot's ID to their respective three error containers. These are pairs
	 * containing a deque and a double; the deque is the error container itself, and the double
	 * is the sum of the deque. We're keeping the sum because it is much more efficient.
	 * deque is very similar  to queue with the ability to access all the elements (like an array).
	 *
	 * errorContainers[myRobot->getID()][0].first 	is rhoQ for myRobot
	 * errorContainers[myRobot->getID()][1].first 	is alphaQ
	 * errorContainers[myRobot->getID()][2].first 	is betaQ
	 *
	 * errorContainer.first is the deque itself, and errorContainer.second is the continuous sum
	 */
    struct errorContainer
    {
        Point lastTargetPoint;
        std::pair<std::deque<double>, double> containers[3];
    };

    static std::unordered_map<int, errorContainer> errorContainerMap;
};


#endif // CLOSEDLOOPCONTROL_H
