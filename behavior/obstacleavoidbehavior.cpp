#include <assert.h>
#include <iostream>
#include <deque>
#include <utility>
#include "behavior/obstacleavoidbehavior.h"
#include "utilities/measurments.h"
#include "skill/gotoposition.h"
#include "skill/gotoposition2.h"
#include "include/globals.h"

#include "skill/pathfinding/fppa_pathfinding.hpp"


ObstacleAvoidBehavior::ObstacleAvoidBehavior(const ParameterList& list)
{
    this->startPoint  = list.getParam<Point>("startPoint");
	this->targetPoint = list.getParam<Point>("targetPoint");
	
	//This actually finds the path.
	std::vector<Point> foundPath = FPPA::findShortestPath(startPoint, targetPoint);	
	
	
	//Create a queue of points to follow.
    pathQueue = std::queue<Point>(std::deque<Point>(foundPath.begin(), foundPath.end()));
    
	
#if FPPA_DEBUG
    std::cout << "Points:" <<std::endl;
    for(Point pt : foundPath) std::cout << pt.toString() << std::endl;
	std::cout << pathQueue.size() << " " << pathQueue.front().toString() << std::endl;
#endif


	//Remove first point - always the starting point, likely where the robot is.
    pathQueue.pop();
    nextPoint = pathQueue.front();
}


void ObstacleAvoidBehavior::perform(Robot* robot)
{
    Skill::GoToPosition go(nextPoint);
    go.perform(robot);


    if(!pathQueue.empty() &&
    Measurments::isClose(robot->getRobotPosition(), pathQueue.front(), DIST_TOLERANCE))
    {
		/* Currently, goes to the first point then clears behavior.
		 * This makes the BehaviorAssignment assign a new ObstacleAvoidBehavior,
		 * which calculates the path again. Basically, a first step in dynamic paths
		 * that change if something gets in the way.
		 * Very inefficient right now though.
		 */
        robot->clearCurrentBeh();
        return;
		

		Point old = Point(nextPoint);
        pathQueue.pop();

	#if FPPA_DEBUG
		std::cout << "Updating;" << " Left: " << pathQueue.size()
				  << " Empty: " << pathQueue.empty() << std::endl;
		std::cout << "Old: " << old.toString()
				  << "New: " << pathQueue.front().toString() << std::endl;
	#endif

        if(!pathQueue.empty()) {
            nextPoint = Point(pathQueue.front());
        } else {
            //Skill Finished
        }
    }
}
