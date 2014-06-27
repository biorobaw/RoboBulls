#include <stdexcept>
#include "utilities/measurments.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/pathfinding/fppa_pathfinding.h"
#include "skill/obstacleavoidmove.h"

namespace Skill 
{

ObstacleAvoidMove::ObstacleAvoidMove(Point target)
	: hasFoundPath(false)
	, hasFoundPathEnd(false)
{
    this->targetPoint = target;
}


void ObstacleAvoidMove::perform(Robot* robot)
{
	if(pathQueue.empty() && hasFoundPath) {
	#if OBSTACLE_MOVE_DEBUG
		if(!hasFoundPathEnd) {
			hasFoundPathEnd = true;
			std::cout << "End of path reached" << std::endl;
		}
	#endif
		return;
	}

	Point robotPoint = robot->getRobotPosition();
	Point nextPoint  = Point(this->pathQueue.front());
	

	/* If we have not found a path yet or there is a new obstacle in
	 * the current line, path needs to be rebult
	 */
	if(!hasFoundPath || FPPA::isObstacleInLine(robotPoint, nextPoint)) {
		hasFoundPath = true;
        std::cout << "Found obstacle in line or !HasPath" << std::endl;

		FPPA::Path found = FPPA::findShortestPath(robotPoint, targetPoint);

        pathQueue.assign(found.begin(), found.end());   //Build queue
	}
	
	
	/* If rob is close to the next point, pop from the queue and begin
	 * going to the next point in the path 
	 */
    if(Measurments::isClose(robotPoint, nextPoint)) {
	#if OBSTACLE_MOVE_DEBUG
		std::cout << "Updating; Old: " << nextPoint.toString();
		std::cout << " New: " << pathQueue.front().toString() << std::endl;
	#endif
        pathQueue.pop_front();
		
		if(!pathQueue.empty())
			nextPoint = pathQueue.front();
		else
			return true;	//All points followed
	}
	
    GoToPositionWithOrientation go
        (nextPoint, Measurments::angleBetween(robotPoint, nextPoint));
		
	go.perform(robot);
	
	return false;	//Still beed to follow points
}


}	//namespace Skill
