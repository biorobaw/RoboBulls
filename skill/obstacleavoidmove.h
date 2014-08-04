#ifndef OBSTACLE_AVOID_MOVE_H
#define OBSTACLE_AVOID_MOVE_H

#include <deque>
#include "skill/skill.h"
#include "skill/pathfinding/fppa_pathfinding.h"

namespace Skill {

/* ObstacleAvoidMove : Goes to a point while avoiding obstacles
 * Constructor: Ending point to move robot to
 * This skill should not be remade each iteration.
 */
#define OBSTACLE_MOVE_DEBUG 0
 
class ObstacleAvoidMove : public Skill
{
public:
    ObstacleAvoidMove(Point target, double finalOrientation = -10);
	bool perform(Robot* robot);
	
private:
	void assignNewPath(const Point& robotPoint);
    double targetAngle;
    bool   hasFoundPath;
    bool   hasFoundPathEnd;
    bool   currentPathIsClear;
    Point  targetPoint;
    Point  lastObsPoint;
	std::deque<Point> pathQueue;
    std::vector<Point> lastObstacles;
    FPPA::PathDirection lastDirection;
};


}	//namespace Skill

#endif
