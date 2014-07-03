#include <stdexcept>
#include "utilities/measurments.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/pathfinding/fppa_pathfinding.h"
#include "skill/obstacleavoidmove.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "communication/robcomm.h"

namespace Skill 
{

ObstacleAvoidMove::ObstacleAvoidMove(Point target)
	: hasFoundPath(false)
    , hasFoundPathEnd(false)
    , isUpdatingPath(false)
    , obsInLine(false)
{
    this->targetPoint = target;
    std::cout <<"Making ObstacleAvoidMove" << std::endl;
}


static bool ptIsInFrontOfRob(Robot* rob, const Point& pt)
{
    Point robPos   = rob->getRobotPosition();
    float robAngle = rob->getOrientation();
    float angleBetween = Measurments::angleBetween(robPos, pt);

    return (Measurments::angleDiff(robAngle, angleBetween) < (M_PI/180)*25);
}


bool ObstacleAvoidMove::perform(Robot* robot)
{
    if(pathQueue.empty() && hasFoundPath)
    {
	#if OBSTACLE_MOVE_DEBUG
		if(!hasFoundPathEnd) {
			hasFoundPathEnd = true;
			std::cout << "End of path reached" << std::endl;
		}
	#endif
        return true;
	}

	Point robotPoint = robot->getRobotPosition();
    Point nextPoint  = this->pathQueue.front();
    Point obstaclePosInfo;
    obsInLine  = FPPA::isObstacleInLine(robotPoint, nextPoint, &obstaclePosInfo);

    if(!hasFoundPath || obsInLine)
    {
    #ifdef OBSTACLE_MOVE_DEBUG
        if(!hasFoundPath) {
            std::cout << "Did not have path for rob " << robot->getID() << ", Making" << std::endl;
        } else if(obsInLine && !isUpdatingPath) {
            std::cout << "New Obstacle for robot "    << robot->getID()
                      << "At " << obstaclePosInfo.toString() << std::endl;
        }
    #endif

        /* If we have not found a path yet or there is a new obstacle in
         * the current line, path needs to be rebult
         */
        if(!hasFoundPath ||
                (obsInLine && isUpdatingPath && ptIsInFrontOfRob(robot, obstaclePosInfo)))
        {
            isUpdatingPath = true;
            FPPA::Path found = FPPA::findShortestPath(robotPoint, targetPoint);
            pathQueue.assign(found.begin(), found.end());   //Build queue
        }

        hasFoundPath = true;
	}
	
	
	/* If rob is close to the next point, pop from the queue and begin
	 * going to the next point in the path 
	 */
    if(Measurments::isClose(robotPoint, nextPoint))
    {
	#if OBSTACLE_MOVE_DEBUG
        //std::cout << "Updating; Old: " << nextPoint.toString();
        //std::cout << " New: " << pathQueue.front().toString() << std::endl;
	#endif
        pathQueue.pop_front();
		
		if(!pathQueue.empty())
			nextPoint = pathQueue.front();
		else
			return true;	//All points followed
	}
	


    CloseLoopSharpTurns cnsd;
    cnsd.setVelMultiplier(20);

    wheelvelocities res = cnsd.closed_loop_control(robot, nextPoint.x, nextPoint.y,
                             Measurments::angleBetween(robotPoint, nextPoint));

    RobComm* rc = RobComm::getRobComm();

    rc->sendVels(res.left, res.right, robot->getID());
	

    return false;	//Still need to follow points
}


}	//namespace Skill
