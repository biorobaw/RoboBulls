#include "model/gamemodel.h"
#include "movement/movetype.h"
#include "movement/go_to_pose.h"
#include "gui/guiinterface.h"
#include <time.h>
#include <assert.h>
#include "include/config/tolerances.h"     //ROT/DIST Tolerance
#include "include/config/globals.h"        //OVERALL_VELOCITY
#include "movement/move.h"
#include "movement/move_collisions.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"

#include <iostream>

namespace Move
{

GoToPose::GoToPose()
    : m_targetPoint(9999,9999)
    , m_targetAngle(UNUSED_ANGLE_VALUE)
    , velMultiplier(1)
    , is_initialized(false)
    , avoid_obstacles(true)
    , avoid_ball(true)
    , lastLineDrawnTime(0)
    { }

GoToPose::~GoToPose()
{ }

GoToPose::GoToPose(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
{
    updateGoal(targetPoint, targetAngle, withObstacleAvoid, avoidBall);
}

void GoToPose::updateGoal(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
{
    std::cout << "DEBUG: updateGoal()" << std::endl;
    /* In most cases, this is called each loop to track a possibly moving point.
     * But that is very inefficient in that basically the pathfinding is
     * remade each time, and other expensive things are done. So if the new point
     * is close enough to the last, let's not do anything
     */
    if(Measurements::distance(m_targetPoint, targetPoint) > recrDistTolerance) {
        m_targetPoint      = targetPoint;
        m_targetAngle      = targetAngle;
        nextTargetAngle    = UNUSED_ANGLE_VALUE;
        nextDistTolerance  = 20;
        pathQueue.clear();
        lastObstacles.clear();
        is_initialized = true;
    }

    m_targetAngle = targetAngle;
    avoid_obstacles = withObstacleAvoid;
    avoid_ball = avoidBall;
}


void GoToPose::setVelocityMultiplier(float newMultiplier)
{
    this->velMultiplier = newMultiplier;
}


void GoToPose::setRecreateTolerances(float distTolerance, float angleTolerance)
{
    this->recrDistTolerance  = distTolerance;
    this->recrAngleTolerance = angleTolerance;
}


void GoToPose::setMovementTolerances(float distTolerance, float angleTolerance)
{
    this->lastDistTolerance = distTolerance;
    this->lastAngTolerance  = angleTolerance;
}


bool GoToPose::perform(Robot *robot, MoveType moveType)
{
    if(!is_initialized)
        return false;

    if(m_targetAngle == UNUSED_ANGLE_VALUE)
        m_targetAngle = Measurements::angleBetween(robot, m_targetPoint);

    if(avoid_obstacles || avoid_ball)
        return calcObstacleAvoidance(robot, moveType);
    else
        return calcRegularMovement(robot, moveType);
}


/***********************************************************/
/********************* Private Methods *********************/
/***********************************************************/

bool GoToPose::calcRegularMovement(Robot* rob, MoveType moveType)
{
    Point robotPos = rob->getPosition();
    float robotAng = rob->getOrientation();

    calcAndSetVels(rob, m_targetPoint, m_targetAngle, moveType);

    if (Measurements::isClose(m_targetPoint, robotPos, lastDistTolerance)
    &&   Measurements::isClose(m_targetAngle, robotAng, lastAngTolerance))
        return true;
    return false;
}

bool GoToPose::pathIsClear(Robot* robot) const
{
    std::cout << "DEBUG: pathIsClear()" << std::endl;
    //Check to see if there is an obstalce in current path
    Point robotPoint = robot->getPosition();
    Point obsPoint;
    bool isNewObstacleInPath = FPPA::isObstacleInLine(robotPoint, nextPoint, &obsPoint, avoid_ball);

    //Checking the next line as well for an obstacle
    if(!isNewObstacleInPath && pathQueue.size() > 2) {
        const Point& nextNextPoint = pathQueue[1];
        isNewObstacleInPath = FPPA::isObstacleInLine(nextPoint, nextNextPoint, &obsPoint, avoid_ball);
    }

    //If there's an obstacle and it isn't near any recorded onces, the path is no longer clear
    if(isNewObstacleInPath && Comparisons::isDistanceToLess(obsPoint, 20).none_of(lastObstacles))
        return false;

    return true;
}

Point GoToPose::updatePathQueue(Robot* robot)
{
    std::cout << "DEBUG: UdpatePathQueue()" << std::endl;
    // Pops path queue if close to next point
    if(Measurements::isClose(robot, nextPoint, nextDistTolerance)) {
        pathQueue.pop_front();
        if(pathQueue.size() == 1) {
            nextDistTolerance = lastDistTolerance;
            nextTargetAngle = m_targetAngle;
        }
    }

    // If the queue is empty, we've finished the path. Otherwise return next waypoint
    if(pathQueue.empty())
        return m_targetPoint;
    else
        return pathQueue.front();
}

void GoToPose::getCollisionState(Robot* robot, bool& collided, bool& yielding) const
{
    // Just to get rid of build warning, doing random stuff
    if(robot == nullptr){int x=6; x = 7-x;}
    collided = yielding = false;
#if MOVEMENT_USE_ROB_COLLIDE
    Collisions::update();
    int status = Collisions::getMoveStatus(robot);
    collided = (status == MOVE_COLLIDED);   //When the robot has collided with another
    yielding = (status == MOVE_YIELDING);   //When the robot is yielding to another
#endif
}

bool GoToPose::calcObstacleAvoidance(Robot* robot, MoveType moveType)
{
    // If we haven't reached the target
    if(Measurements::distance(robot, m_targetPoint) > lastDistTolerance)
    {
        // Assign a new path if the current path is not clear
        if(!pathIsClear(robot))
            assignNewPath(robot->getPosition(), (robot->getID() != GOALIE_ID));

        // Get the next way-point
        nextPoint = updatePathQueue(robot);
    }

    // Move to next waypoint
    calcAndSetVels(robot, nextPoint, m_targetAngle, moveType);

    if (Measurements::isClose(m_targetPoint, robot, lastDistTolerance)
    &&  Measurements::isClose(m_targetAngle, robot->getOrientation(), lastAngTolerance))
        return true;
    return false;   // Motion not finished
}


void GoToPose::assignNewPath(const Point& robotPoint, bool use_def_areas)
{
    std::cout << "DEBUG: assignNewPath()";
    FPPA::Path path = FPPA::genPath(robotPoint, m_targetPoint, avoid_ball, use_def_areas);
    pathQueue.assign(path.begin(), path.end());
    lastObstacles = FPPA::getCurrentObstacles(); //Copies

    //Draws path lines on iterface. Uses clock() to avoid line spam.
    long now = clock();
    if((float)(now - lastLineDrawnTime) / CLOCKS_PER_SEC > 0.5)
    {
        lastLineDrawnTime = now;
        for (unsigned int i=1; i<pathQueue.size(); i++)
            GuiInterface::getGuiInterface()->drawLine(pathQueue[i-1], pathQueue[i], 0.25*i);
    }
}

void GoToPose::calcAndSetVels(Robot *rob, Point targetPoint, float targetAngle, MoveType moveType)
{
    switch(rob->type())
    {
    case differential:
        {
            if(!IS_DIFFERENTIAL(moveType))
                moveType = ::Move::MoveType::Default;
            diff_p.drive(rob, targetPoint, targetAngle, moveType);
        }
        break;
    case threeWheelOmni: 
        {
            if(!IS_OMNI(moveType))
                moveType = ::Move::MoveType::Default;
            three_wheel_omni_p.drive(rob, targetPoint, targetAngle, moveType);
        }
        break;
    case fourWheelOmni:
        {
            if(!IS_OMNI(moveType))
                moveType = ::Move::MoveType::Default;
            four_wheel_omni_p.drive(rob, targetPoint, targetAngle, moveType);
        }
        break;
    }
}

}
