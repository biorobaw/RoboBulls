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
    : final_target_point(9999,9999)
    , final_target_angle(UNUSED_ANGLE_VALUE)
    , vel_multiplier(1)
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
    /* In most cases, this is called each loop to track a possibly moving point.
     * But that is very inefficient in that basically the pathfinding is
     * remade each time, and other expensive things are done. So if the new point
     * is close enough to the last, let's not do anything
     */
    if(Measurements::distance(final_target_point, targetPoint) > recrDistTolerance) {
        final_target_point      = targetPoint;
        final_target_angle      = targetAngle;
        next_target_angle    = UNUSED_ANGLE_VALUE;
        next_dist_tolerance  = 20;
        path_queue.clear();
        is_initialized = true;
    }

    final_target_angle = targetAngle;
    avoid_obstacles = withObstacleAvoid;
    avoid_ball = avoidBall;
}


void GoToPose::setVelocityMultiplier(float newMultiplier)
{
    this->vel_multiplier = newMultiplier;
}


void GoToPose::setRecreateTolerances(float distTolerance, float angleTolerance)
{
    this->recrDistTolerance  = distTolerance;
    this->recrAngleTolerance = angleTolerance;
}


void GoToPose::setMovementTolerances(float distTolerance, float angleTolerance)
{
    this->last_dist_tolerance = distTolerance;
    this->lastAngTolerance  = angleTolerance;
}


bool GoToPose::perform(Robot *robot, MoveType moveType)
{
    if(!is_initialized)
        return false;

    if(final_target_angle == UNUSED_ANGLE_VALUE)
        final_target_angle = Measurements::angleBetween(robot, final_target_point);

    if(avoid_obstacles || avoid_ball)
        return performObstacleAvoidance(robot, moveType);
    else
        return performNonAvoidMovement(robot, moveType);
}

/***********************************************************/
/********************* Private Methods *********************/
/***********************************************************/

bool GoToPose::performNonAvoidMovement(Robot* rob, MoveType moveType)
{
    Point robotPos = rob->getPosition();
    float robotAng = rob->getOrientation();

    calcAndSetVels(rob, final_target_point, final_target_angle,
                   final_target_point, moveType);

    if (Measurements::isClose(final_target_point, robotPos, last_dist_tolerance)
    &&  Measurements::isClose(final_target_angle, robotAng, lastAngTolerance))
        return true;
    return false;
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

bool GoToPose::performObstacleAvoidance(Robot* robot, MoveType moveType)
{
    // Assign robots that are to be considered obstacles
    FPPA::updateRobotObstacles(robot);

    // If we haven't reached the target
    if(Measurements::distance(robot, final_target_point) > last_dist_tolerance)
        assignNewPath(robot->getPosition(), (robot->getID() != GOALIE_ID));

    // Move to next waypoint
    calcAndSetVels(robot, next_point, final_target_angle, next_next_point, moveType);

    if (Measurements::isClose(final_target_point, robot, last_dist_tolerance)
    &&  Measurements::isClose(final_target_angle, robot->getOrientation(), lastAngTolerance))
        return true;
    return false;   // Motion not finished
}

void GoToPose::assignNewPath(const Point& robotPoint, bool use_def_areas)
{
    FPPA::Path path = FPPA::genPath(robotPoint, final_target_point, avoid_ball, use_def_areas);
    path_queue.assign(path.begin(), path.end());

    if(path_queue.empty()){
        next_point = final_target_point;
        next_next_point = final_target_point;
    } else if(path_queue.size() == 1) {
        next_point = path_queue[0];
        next_next_point = path_queue[0];
        next_dist_tolerance = last_dist_tolerance;
        next_target_angle = final_target_angle;
    } else {
        next_point = path_queue[0];
        next_next_point = path_queue[1];
    }

    if(!path_queue.empty() && Measurements::isClose(robotPoint, next_point, next_dist_tolerance))
        path_queue.pop_front();
}

void GoToPose::calcAndSetVels(Robot *rob, Point targetPoint, float targetAngle, Point nextPoint, MoveType moveType)
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
            four_wheel_omni_p.drive(rob, targetPoint, targetAngle, nextPoint, moveType);
        }
        break;
    }
}

}
