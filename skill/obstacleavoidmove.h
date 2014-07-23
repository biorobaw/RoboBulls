#include <deque>
#include "skill/skill.h"
#include "skill/pathfinding/fppa_pathfinding.h"

namespace Skill {

/* ObstacleAvoidMove : Goes to a point while avoiding obstacles
 * Constructor: Ending point to move robot to
 * This skill should not be remade each iteration.
 */
#define OBSTACLE_MOVE_DEBUG 1
 
class ObstacleAvoidMove : public Skill
{
public:
	ObstacleAvoidMove(Point target);
	bool perform(Robot* robot);
	
private:
	void assignNewPath(const Point& robotPoint);
	bool hasFoundPath;
	bool currentPathIsClear;
	Point targetPoint;
	Point lastObsPoint;
	std::deque<Point> pathQueue;
    std::vector<Point> lastObstacles;
    FPPA::PathDirection lastDirection;
};


}	//namespace Skill
