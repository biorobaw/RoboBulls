#ifndef OBSTACLEAVOIDBEHAVIOR_H
#define OBSTACLEAVOIDBEHAVIOR_H

#include <vector>
#include <queue>
#include "utilities/point.h"
#include "behavior.h"
#include "utilities/paramlist.h"

/*
 * Obstacle Avoid Behavior
 * Behavior that implements using the FPPA pathfinding algorithm by
 * managing a queue of points and going to each one.
 * James W.
 */
 
class ObstacleAvoidBehavior : public Behavior
{
public: 
    ObstacleAvoidBehavior(const ParameterList& list);
    void perform(Robot* robot);

private:
	Point startPoint;
	Point targetPoint;
	Point nextPoint;
    std::queue<Point> pathQueue;
};

#endif // OBSTACLEAVOIDBEHAVIOR_H
