#include <iostream>
#include <list>
#include "stopstrategy.h"
#include "utilities/measurments.h"
#include "behavior/behaviorassignment.h"
#include "behavior/stopbehavior.h"

#define RADIUS 1000
#define STOPSTRAT_DEBUG 0


/* A map of each robot to a target point around the ball */
std::unordered_map<int, Point> StopStrategy::robTargetPoints;

/* The previously-known position of the ball */
Point StopStrategy::prevBallPoint;



StopStrategy::StopStrategy()
{
    if(robTargetPoints.empty())
        this->rebuildTargetPoints();
}


void StopStrategy::assignBeh()
{
    GameModel* model = GameModel::getModel();
    Point ballPoint  = model->getBallPoint();

    /* Update prevBallPoint and the target point map
     * if the ball has moved
     */
    if(!Measurments::isClose(ballPoint, prevBallPoint, 50.0))
    {
    #if STOPSTRAT_DEBUG
        std::cout << "Ball Change; " << prevBallPoint.toString()
                  << " New: " << ballPoint.toString() << std::endl;
    #endif

        this->rebuildTargetPoints();
        prevBallPoint = ballPoint;
    }


    BehaviorAssignment<StopBehavior> stopAssign(true);

    for(Robot* rob : model->getMyTeam())
    {
         //There is a new robot added
        if(robTargetPoints.find(rob->getID()) == robTargetPoints.end()) {
            this->rebuildTargetPoints();
        }

        //Target angle to make robots face the ball
        float targetAngle
                = Measurments::angleBetween(rob->getRobotPosition(), ballPoint);

        stopAssign.setBehParam("targetPoint", robTargetPoints[rob->getID()]);
        stopAssign.setBehParam("targetAngle", targetAngle);
        stopAssign.assignBeh(rob);\
    }
}


void StopStrategy::rebuildTargetPoints()
{
    std::list<Point> newPoints;

    GameModel* mod  = GameModel::getModel();
    Point ballPoint = mod->getBallPoint();

    /* Remove all old points; safe for now */
    robTargetPoints.clear();
    int teamSize = mod->getMyTeam().size();
    float theta = 0;
    float theta_inc = (2*M_PI) / teamSize;


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
        rob->clearCurrentBeh();

        auto min_pos = Measurments::closestPoint(newPoints, rob->getRobotPosition());
    #if STOPSTRAT_DEBUG
        std::cout << rob->getID() << ": " << min_pos->toString() << std::endl;
    #endif
        robTargetPoints[rob->getID()] = *min_pos;	//Set a new target point for rob
        newPoints.erase(min_pos);	//Remove this point because it is now used
    }
}
