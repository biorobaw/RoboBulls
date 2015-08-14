#include <time.h>
#include <assert.h>
#include "include/config/tolerances.h"     //ROT/DIST Tolerance
#include "include/config/globals.h"        //OVERALL_VELOCITY
#include "movement/move.h"
#include "movement/move_collisions.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "gui/guiinterface.h"              //Ryan

/************************************************************************/
/* USER CONFIGURATION */

/* Set this to 1 to use the robot-robot obstacle avoidance routines.
 * Without this there's little point in trying an actual game
 * [See movement/move_collisions.h]
 */
#define MOVEMENT_USE_ROB_COLLIDE    1

//Debug enable constant
#define MOVEMENT_MOVE_DEBUG 0

/************************************************************************/


namespace Movement
{

Move::Move()
{ }

Move::~Move()
{ }

Move::Move(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
{
    recreate(targetPoint, targetAngle, withObstacleAvoid, avoidBall);
}


void Move::recreate(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
{
    /* In most cases, this is called each loop to track a possibly moving point.
     * But that is very inefficient in that basically the pathfinding is 
     * remade each time, and other expensive things are done. So if the new point
     * is close enough to the last, let's not do anything
     */
    if(Measurments::distance(m_targetPoint, targetPoint) > recrDistTolerance) {
        m_targetPoint      = targetPoint;
        m_targetAngle      = targetAngle;
        useObstacleAvoid   = withObstacleAvoid;
        useAvoidBall       = avoidBall;
        currentPathIsClear = false;
        nextTargetAngle    = UNUSED_ANGLE_VALUE;
        nextDistTolerance  = 250;
        pathInfo.first.clear();
        pathQueue.clear();
        lastObstacles.clear();
        lastDirection = FPPA::PathDirection::None;
        isInitialized = true;
    } 
    else if(!Measurments::isClose(m_targetAngle, targetAngle, recrAngleTolerance)) {
        m_targetAngle = targetAngle;
    }
    else if(withObstacleAvoid != useObstacleAvoid) {
        useObstacleAvoid = withObstacleAvoid;
    }
}


void Move::setVelocityMultiplier(float newMultiplier)
{
    this->velMultiplier = newMultiplier;
}


void Move::setRecreateTolerances(float distTolerance, float angleTolerance)
{
    this->recrDistTolerance  = distTolerance;
    this->recrAngleTolerance = angleTolerance;
}


void Move::setMovementTolerances(float distTolerance, float angleTolerance)
{
    this->lastDistTolerance = distTolerance;
    this->lastAngTolerance  = angleTolerance;
}


bool Move::perform(Robot *robot, Movement::Type moveType)
{
    bool finished = true;

    if(!isInitialized)
        return false;
    
    /* This is a correction  factor; since all movement classes derive from
     * this class, the angle is corrected if the special flag is used. Likewise,
     * the movementType corrections to Movement::Default are done in
     * GoToPositon. The lower levels don't see this.
     */
    if(m_targetAngle == UNUSED_ANGLE_VALUE)
        m_targetAngle = Measurments::angleBetween(robot, m_targetPoint);

    if(useObstacleAvoid) {
        /* Check to see if we've found the end of the path. If we have and we're not close
         * to the ending point, assign a path to get back to it */
        if(hasFoundPathEnd && Measurments::distance(robot, m_targetPoint) > ROBOT_SIZE) {
            hasFoundPathEnd = false;
            assignNewPath(robot->getRobotPosition());
        }
        finished = this->calcObstacleAvoidance(robot, moveType);
    } else {
        finished = this->calcRegularMovement(robot, moveType);
    }

    this->setVels(robot);
    return finished;
}


/***********************************************************/
/********************* Private Methods *********************/
/***********************************************************/

bool Move::calcRegularMovement(Robot* robot, Type moveType)
{
    Point robotPos = robot->getRobotPosition();
    float robotAng = robot->getOrientation();

    //Check to see if movement is not necessary; close in position and angle
    if (Measurments::isClose(m_targetPoint, robotPos, lastDistTolerance) &&
        Measurments::isClose(m_targetAngle, robotAng, lastAngTolerance)) {
        lfront=lback=rfront=rback=left=right=back=0;
        return true;
    } else {
        //In regular movement we go directly to the target point as seen here
        calculateVels(robot, m_targetPoint, m_targetAngle, moveType);
        return false;
    }
}

bool Move::determinePathClear(Robot* robot) const
{
    //Check to see if there is an obstalce in current path
    Point robotPoint = robot->getRobotPosition();
    Point obsPoint;
    bool isNewObstacleInPath = FPPA::isObstacleInLine(robotPoint, nextPoint, &obsPoint, useAvoidBall);

    //Checking the next line as well for an obstacle
    if(!isNewObstacleInPath && pathQueue.size() > 2) {
        const Point& nextNextPoint = pathQueue[1];
        isNewObstacleInPath = FPPA::isObstacleInLine(nextPoint, nextNextPoint, &obsPoint, useAvoidBall);
    }

    //If there's an obstcle and it isn't near any recorded onces, the path is no longer clear
    if(isNewObstacleInPath && Comparisons::isDistanceToLess(obsPoint, 100).none_of(lastObstacles)) {
        return false;
    }

    return true;
}

Point Move::updatePathQueue(Robot* robot)
{
    //Pops path queue if close to next point
    if(Measurments::isClose(robot, nextPoint, nextDistTolerance)) {
        pathQueue.pop_front();
        if(pathQueue.size() == 1) {
            nextDistTolerance = lastDistTolerance;
            nextTargetAngle = m_targetAngle;
        }
    }

    //If the queue is empty, we've finished the path. Otherwise return next waypoint
    if(pathQueue.empty()) {
        hasFoundPathEnd = true;
        return m_targetPoint;
    } else {
        return pathQueue.front();
    }
}

void Move::getCollisionState(Robot* robot, bool& collided, bool& yielding) const
{
    collided = yielding = false;
#if MOVEMENT_USE_ROB_COLLIDE
    Collisions::update();
    auto status = Collisions::getMoveStatus(robot);
    collided = (status == MOVE_COLLIDED);   //When the robot has collided with another
    yielding = (status == MOVE_YIELDING);   //When the robot is yielding to another
#endif
}

bool Move::calcObstacleAvoidance(Robot* robot, Type moveType)
{
    bool isCollided, isYielding;
    getCollisionState(robot, isCollided, isYielding);

    if(!hasFoundPathEnd) {
        //Pathfinding waypoint mode.
        if(currentPathIsClear) {
            /* If path clear, nextPoint is decided on if the robot is collided,
             * yielding, or in the clear. see getCollisionState function */
            if(isYielding) {
                nextPoint = robot->getRobotPosition();
            } else if(isCollided) {
                nextPoint = robot->getRobotPosition() + Collisions::getBackupDirection(robot) * 600;
            } else {
                nextPoint = updatePathQueue(robot);
                currentPathIsClear = determinePathClear(robot);
            }
            calculateVels(robot, nextPoint, m_targetAngle, moveType);
        } else {
            //A new path needs to be assigned. Do no movement
            assignNewPath(robot->getRobotPosition());
            currentPathIsClear = true;
        }
    } else {
        //Rotating mode. If robot did not reach target angle, rotate only
        calculateVels(robot, m_targetPoint, m_targetAngle, moveType);
        if (Measurments::isClose(m_targetPoint, robot, lastDistTolerance) &&
            Measurments::isClose(m_targetAngle, robot->getOrientation(), lastAngTolerance*1.5)) {
            lfront=lback=rfront=rback=left=right=back=0;
            return true;
        }
    }

    return false;   //Skill not finished
}


void Move::assignNewPath(const Point& robotPoint)
{
    FPPA::PathInfo p = FPPA::findShortestPath(robotPoint, m_targetPoint, useAvoidBall, lastDirection, 0.50);
    pathQueue.assign(p.first.begin(), p.first.end());
    lastDirection = p.second;
    lastObstacles = FPPA::getCurrentObstacles(); //Copies

    //Draws path lines on iterface
    //Uses clock() to avoid line spam
    long now = clock();
    if((float)(now - lastLineDrawnTime) / CLOCKS_PER_SEC > 0.5) {
        lastLineDrawnTime = now;
        for (unsigned int i=1; i<pathQueue.size(); i++)
            GuiInterface::getGuiInterface()->drawPath(pathQueue[i-1], pathQueue[i], i*2);
    }
}


void Move::setVels(Robot *robot)
{
    /* This is the function that, after velocity is calculated above,
     * sets the velocities on the actual robot. This then funnels down
     * to sendVelsLarge. Thus, OVERALL_VELOCITY allows the slowing down
     * of the entire program since all movement comes through here.
     */

    /* Ryan has perpetrated this boolean check; if overrided, ignore movement signals */
    if (!GuiInterface::getGuiInterface()->isOverriddenBot()[robot->getID()]) {
        switch(robot->type()) {
        case differential:
            robot->setL(left  * velMultiplier * OVERALL_VELOCITY);
            robot->setR(right * velMultiplier * OVERALL_VELOCITY);
            break;
        case threeWheelOmni:
            robot->setL(left  * velMultiplier * OVERALL_VELOCITY);
            robot->setR(right * velMultiplier * OVERALL_VELOCITY);
            robot->setB(back  * velMultiplier * OVERALL_VELOCITY);
            break;
        case fourWheelOmni:
            robot->setLF(lfront * velMultiplier * OVERALL_VELOCITY);
            robot->setRF(rfront * velMultiplier * OVERALL_VELOCITY);
            robot->setLB(lback  * velMultiplier * OVERALL_VELOCITY);
            robot->setRB(rback  * velMultiplier * OVERALL_VELOCITY);
            break;
        }
    }
}


}
