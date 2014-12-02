#include <iostream>
#include <list>
#include "stopstrategy.h"
#include "include/config/team.h"
#include "utilities/measurments.h"
#include "behavior/behaviorassignment.h"
#include "behavior/stopbehavior.h"
#include "behavior/defendfarfromball.h"

#define RADIUS 1000
#define STOPSTRAT_DEBUG 1

Point StopStrategy::prevBallPoint = Point(9999,9999);
Point robTargetPoints[MAX_ROBOTS];

void StopStrategy::assignBeh()
{
    GameModel* model = GameModel::getModel();
    Point bp = model->getBallPoint();
    rebuildTargetPoints();

    for(Robot* robot : model->getMyTeam()) {
        if (robot->getID() != 5){
            Point robTarget = robTargetPoints[robot->getID()];
            float targetAngle = Measurments::angleBetween(robTarget, bp);
            BehaviorAssignment<GenericMovementBehavior> stopAssign(true);
            stopAssign.assignBeh(robot, robTarget, targetAngle, true, true);
        }
        else
        {
            BehaviorAssignment<DefendFarFromBall> golieAssign;
            golieAssign.setSingleAssignment(true);
            golieAssign.assignBeh(robot);
        }
    }
}

#if 1
bool StopStrategy::update()
{
    Point nowBallPoint = GameModel::getModel()->getBallPoint();
    if(Measurments::distance(nowBallPoint, prevBallPoint) > 50) {
        prevBallPoint = nowBallPoint;
        return true;
    }
    return false;
}
#endif

void StopStrategy::rebuildTargetPoints()
{
    std::list<Point> newPoints;

    GameModel* mod  = GameModel::getModel();
    Point ballPoint = mod->getBallPoint();

    int teamSize = mod->getMyTeam().size();

    /* Interesting thing:
     * If we're yellow team, then we we do is add an offset to the initial
     * theta, to allow the yellow robots to move to different points than
     * the blue robots. They both follow the same increment.
     */
    float theta_inc = (2*M_PI) / teamSize;
#if TEAM==TEAM_BLUE
    float theta = 0;
#else
    float theta = theta_inc/2;
#endif

    /* First create an evenly distributed number of points around the ball.
     * Later, I want to make this so all robots are placed on one side.
     */
    for(int i = 0; i != teamSize; ++i, theta += theta_inc)
    {
        float x_pos = (RADIUS * cos(theta)) + ballPoint.x;
        float y_pos = (RADIUS * sin(theta)) + ballPoint.y;
        newPoints.emplace_back(x_pos, y_pos);
    }

    /* Then for each robot, find the closest point around the ball to the robot. This
     * will be the robot's new target point stored in robTargetPoints
     */
    for(Robot* rob : mod->getMyTeam())
    {
        //rob->clearCurrentBeh();

        auto min_pos = Measurments::closestPoint(newPoints, rob->getRobotPosition());
    #if STOPSTRAT_DEBUG
        std::cout << rob->getID() << ": " << min_pos->toString() << std::endl;
    #endif
        robTargetPoints[rob->getID()] = *min_pos;	//Set a new target point for rob
        newPoints.erase(min_pos);	//Remove this point because it is now used
    }
}
