#include <deque>
#include "skill/skill.h"

namespace Skill {

/* ObstacleAvoidMove : Goes to a point while avoiding obstacles
 * Constructor: Takes a list with a "targetPoint" entry, corrisponding
 * to the targeted point of the move.
 *
 * This skill should not be remade each iteration.
 */
#define OBSTACLE_MOVE_DEBUG 0
 
class ObstacleAvoidMove : public Skill
{
public:
    ObstacleAvoidMove(Point target);
	bool perform(Robot* robot);
	
private:
	bool hasFoundPath;
	bool hasFoundPathEnd;
	Point targetPoint;
    std::deque<Point> pathQueue;
};


}	//namespace Skill
