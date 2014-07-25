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
	, currentPathIsClear(true)
    , targetPoint(target)
    , lastDirection(FPPA::PathDirection::None)
{
#if OBSTACLE_MOVE_DEBUG
	std::cout << "Making ObstacleAvoidMove" << std::endl;
#endif
}



static bool ptIsInFrontOfRob(Robot* rob, const Point& pt)
{
	Point robPos   = rob->getRobotPosition();
	float robAngle = rob->getOrientation();
	float angleBetween = Measurments::angleBetween(robPos, pt);

    return (Measurments::angleDiff(robAngle, angleBetween) < M_PI/6);
}



bool ObstacleAvoidMove::perform(Robot* robot)
{
#if 0
	if(pathQueue.empty() && hasFoundPath) 
	{
	#if OBSTACLE_MOVE_DEBUG
        std::cout << "End of path reached" << std::endl;
	#endif
		return true;
	}
#endif

	Point robotPoint = robot->getRobotPosition();

	/**********///Initial condition: Robot has no path
	if(!hasFoundPath) {
		hasFoundPath = true;
		this->assignNewPath(robotPoint);
	}
	/**********/

	Point nextPoint = this->pathQueue.front();
	
	/**********///Dynamic path updating
	if(currentPathIsClear)
	{
		//No known obstacles in path; test for new ones
		Point obsPoint;
		bool isNewObstacleInPath = FPPA::isObstacleInLine(robotPoint, nextPoint, &obsPoint);

        if(pathQueue.size() > 2) {
            const Point& nextNextPoint   = pathQueue[2];
			bool isNewObstacleInNextPath = FPPA::isObstacleInLine(nextPoint, nextNextPoint, &obsPoint);
            isNewObstacleInPath = isNewObstacleInPath || isNewObstacleInNextPath;
        }

        if(isNewObstacleInPath && !Measurments::isClose(obsPoint, lastObsPoint, 100))
        {
            /* We have a possible obstacle..
             * If it is NOT close any of the obstacles used to generate
             * the current path, it is a new obstacle
             */
            if(std::none_of(lastObstacles.begin(), lastObstacles.end(),
                            [&](const Point& pt) {
                                return Measurments::isClose(pt, obsPoint, 100);
                             }))
            {
                /* We've found a new obstacle in the clear path.
                 * We want to save the obstacle point to compare
                 * against it later
				 */
                lastObsPoint = obsPoint;
                currentPathIsClear = false;
            }
		}
	} else {
		/* There is a known obstacle in the current path
		 * We want to rebuild the path and set the (new)
		 * path to be clear again
		 */
		this->assignNewPath(robotPoint);
		currentPathIsClear = true;
	}

	
	/**********///Path Queue Updating
    if(Measurments::isClose(robotPoint, nextPoint, 250)) 
	{
		pathQueue.pop_front();
		if(!pathQueue.empty()) {
			nextPoint = pathQueue.front();
		} else {
			return true;	//All points followed
		}
	}
	
	
	/**********///Velocity sending
    ClosedLoopSharpTurns moveController;
    moveController.setVelMultiplier(20).closed_loop_control(robot, nextPoint).sendVels();

	
	return false;	//Still need to follow points
}



void ObstacleAvoidMove::assignNewPath(const Point& robotPoint)
{
    FPPA::getCurrentObstacles(&(this->lastObstacles), robotPoint, targetPoint);

    FPPA::PathInfo p = FPPA::findShortestPath(robotPoint, targetPoint, lastDirection);
    this->pathQueue.assign(p.first.begin(), p.first.end());
    this->lastDirection = p.second;
}


}	//namespace Skill
