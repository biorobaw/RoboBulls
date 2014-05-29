#ifndef OBSTACLEAVOIDBEHAVIOR_H
#define OBSTACLEAVOIDBEHAVIOR_H

#include <vector>
#include <queue>
#include "utilities/point.h"
#include "skill/gotoposition2.h"
#include "behavior.h"
#include "utilities/paramlist.h"

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
    Skill::GoToPosition2* myGoto;
};

#endif // OBSTACLEAVOIDBEHAVIOR_H
