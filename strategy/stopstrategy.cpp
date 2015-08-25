#include <iostream>
#include <list>
#include "include/config/team.h"
#include "utilities/measurments.h"
#include "behavior/defendfarfromball.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"
#define STOPSTRAT_DEBUG 0

StopStrategy::StopStrategy(float radius)
    : prevBallPoint(9999,9999)
    , radius(radius)
    { }

void StopStrategy::assignBeh()
{
    Point bp = gameModel->getBallPoint();
    rebuildTargetPoints();

    for(Robot* robot : gameModel->getMyTeam()) {
        if(robot->getID() == GOALIE_ID)
            continue;
        Point robTarget = robTargetPoints[robot->getID()];
        float targetAngle = Measurments::angleBetween(robTarget, bp);
        robot->assignBeh<GenericMovementBehavior>(robTarget, targetAngle);
    }

    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<DefendFarFromBall>();
}

bool StopStrategy::update()
{
    Point nowBallPoint = gameModel->getBallPoint();
    if(Measurments::distance(nowBallPoint, prevBallPoint) > 50) {
        prevBallPoint = nowBallPoint;
        return true;
    }
    return false;
}

void StopStrategy::rebuildTargetPoints()
{
    std::list<Point> newPoints;
    Point ballPoint = gameModel->getBallPoint();
    int teamSize = gameModel->getMyTeam().size();
    int maxTeamSize  = std::max(teamSize, (int)gameModel->getOponentTeam().size());

    /* If we're yellow team, then we add an offset to the initial
     * theta, to allow the yellow robots to move to different points than
     * the blue robots. They both follow the same increment. */
    float theta_inc = (2*M_PI) / maxTeamSize;
#if TEAM==TEAM_BLUE
    float theta = 0;
#else
    float theta = theta_inc/2;
#endif

    /* First create an evenly distributed number of points around the ball.
     * Later, I want to make this so all robots are placed on one side. */
    for(int i = 0; i != teamSize; ++i, theta += theta_inc)
    {
        float x_pos = (radius * cos(theta)) + ballPoint.x;
        float y_pos = (radius * sin(theta)) + ballPoint.y;
        newPoints.emplace_back(x_pos, y_pos);
    }

    /* Then for each robot, find the closest point around the ball to the robot. This
     * will be the robot's new target point stored in robTargetPoints */
    for(Robot* rob : gameModel->getMyTeam())
    {
        auto min_pos = Comparisons::distance(rob).min(newPoints);
        robTargetPoints[rob->getID()] = *min_pos;    //Set a new target point for rob
        newPoints.erase(min_pos);                    //Remove this point because it is now used
    #if STOPSTRAT_DEBUG
        std::cout << rob->getID() << ": " << min_pos->toString() << std::endl;
    #endif
    }
}
