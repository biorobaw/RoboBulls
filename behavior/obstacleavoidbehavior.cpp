#include <assert.h>
#include <iostream>
#include <deque>
#include <utility>
#include "behavior/obstacleavoidbehavior.h"
#include "utilities/measurments.h"
#include "skill/gotoposition.h"
#include "skill/gotoposition2.h"
#include "behavior.h"
#include "include/globals.h"

#include "skill/pathfinding/fppa_pathfinding.hpp"

ObstacleAvoidBehavior::ObstacleAvoidBehavior(const ParameterList& list)
{
    this->startPoint  = list.getParam<Point>("startPoint");
	this->targetPoint = list.getParam<Point>("targetPoint");
	
	//This actually finds the path.
	std::vector<Point> foundPath = FPPA::findShortestPath(startPoint, targetPoint);	
	
#if FPPA_DEBUG
    std::cout << "Points:" <<std::endl;
    for(Point pt : foundPath) std::cout << pt.toString() << std::endl;
#endif

	//Create a queue of points to follow.
    std::deque<Point> pathDeque = std::deque<Point>
            (foundPath.begin(), foundPath.end());

#if FPPA_DEBUG
    std::cout << "DPoints:" <<std::endl;
    for(Point pt : pathDeque) std::cout << pt.toString() << std::endl;
#endif

    pathQueue = std::queue<Point> ( pathDeque );
    std::cout << pathQueue.size() << " " << pathQueue.front().toString() << std::endl;

    pathQueue.pop();
    nextPoint = pathQueue.front();

    //myGoto = new Skill::GoToPosition2(nextPoint);
}


void ObstacleAvoidBehavior::perform(Robot* robot)
{
    Skill::GoToPosition go(nextPoint);
    go.perform(robot);

    //if(myGoto != nullptr)
    // myGoto->perform(robot);

    if(!pathQueue.empty() &&
    Measurments::isClose(robot->getRobotPosition(), pathQueue.front(), DIST_TOLERANCE))
    {
        Point old = Point(nextPoint);
        pathQueue.pop();

        robot->clearCurrentBeh();
        return;

        #if FPPA_DEBUG
            std::cout << "Updating;" << " Left: " << pathQueue.size()
            << " Empty: " << pathQueue.empty() << std::endl;
            std::cout << "Old: " << old.toString()
            << "New: " << pathQueue.front().toString() << std::endl;
        #endif

        if(!pathQueue.empty()) {
            nextPoint = Point(pathQueue.front());
        } else {
            //robot->clearCurrentBeh();
            //Skill Finished
        }
    }
}
