#include "include/config/tolerances.h"     //ROT/DIST Tolerance
#include "include/config/globals.h"        //OVERALL_VELOCITY
#include "movement/move.h"
#include "movement/move_collisions.h"
#include "model/gamemodel.h"
// Ryan
#include "gui/guiinterface.h"

/* Set this to 1 to use the robot-robot obstacle avoidance routines.
 * Without this there's little point in trying an actual game
 */
#define MOVEMENT_USE_ROB_COLLIDE    1


namespace Movement
{

Move::Move()
    : velMultiplier(1.0)
{
}

Move::~Move()
{
}

Move::Move(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
    : velMultiplier(1.0)
{
    recreate(targetPoint, targetAngle, withObstacleAvoid, avoidBall);
}


void Move::recreate(Point targetPoint, float targetAngle, bool withObstacleAvoid, bool avoidBall)
{
    if(Measurments::distance(m_targetPoint, targetPoint) > recrDistTolerance) {
        /* In most cases, this is called each loop to track a possibly moving point.
         * But that is very inefficient in that basically the pathfinding is 
         * remade each time, and other expensive things are done. So if the new point
         * is close enough to the other, let's not do anything
         */
        m_targetPoint      = targetPoint;
        m_targetAngle      = targetAngle;
        isInitialized      = false;
        useObstacleAvoid   = withObstacleAvoid;
        useAvoidBall       = avoidBall;
        pathEndInfo.hasFoundPathEnd = false;
        pathEndInfo.endingPoint = Point(9999,9999);
        currentPathIsClear = false;
        nextTargetAngle    = UNUSED_ANGLE_VALUE;
        nextDistTolerance  = 250;
        lastObsPoint       = Point(9999, 9999);
        pathInfo.first.clear();
        pathQueue.clear();
        lastDirection = FPPA::PathDirection::None;
        lastObstacles.clear();
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
    
    if(!isInitialized) {
    #if MOVEMENT_MOVE_DEBUG
        std::cout << "Warning: Move called without initialization" << std::endl;
    #endif
        return false;
    }
    
    /* This is a correction  factor; since all movement classes derive from
     * this class, the angle is corrected if the special flag is used. Likewise,
     * the movementType corrections to Movement::Default are done in
     * GoToPositon. The lower levels don't see this.
     */
    if(m_targetAngle == UNUSED_ANGLE_VALUE)
        m_targetAngle = Measurments::angleBetween(robot->getRobotPosition(), m_targetPoint);

    /* Let's only bother moving if we need to */
    //if(!Measurments::isClose(m_targetPoint, robot->getRobotPosition(), lastDistTolerance) ||
    // !Measurments::isClose(m_targetAngle, robot->getOrientation(), lastAngTolerance))

    {
        if(useObstacleAvoid) {
            if(pathEndInfo.hasFoundPathEnd) {
                /* Here we have a failsafe mechanism that checks if the robot
                 * has moved from the path's end. If so, hasFoundPathEnd is false
                 */
                Point robPos = robot->getRobotPosition();
                if(Measurments::distance(robPos, pathEndInfo.endingPoint) > 150) {
                    pathEndInfo.hasFoundPathEnd = false;
                }
            }
            finished = this->calcObstacleAvoidance(robot, moveType);
        } else {
            finished = this->calcRegularMovement(robot, moveType);
        }
    }

#if MOVEMENT_MOVE_DEBUG
    std::cout << "Move perform on " << robot->getID() << std::endl;
#endif

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

    // Check to see if movement is necessary
    if (Measurments::isClose(m_targetPoint, robotPos, lastDistTolerance) &&
        Measurments::isClose(m_targetAngle, robotAng, lastAngTolerance))
    {
        lfront=lback=rfront=rback=left=right=back=0;
        return true;
    }
    else
    {
        /* Using regular movement, we calculate the velocities to move
         * directly to the target point and angle.
         */
        this->calculateVels(robot, m_targetPoint, m_targetAngle, moveType);
        return false;
    }
}


bool Move::calcObstacleAvoidance(Robot* robot, Type moveType)
{
#if MOVEMENT_USE_ROB_COLLIDE
    Collisions::update();
    switch(Collisions::getMoveStatus(robot))
    {
    case MOVE_OK:
        break;
    case MOVE_NOTOK:
        left = right = back = lfront = lback = rfront = rback = 0;
        return false;
        break;
    case MOVE_COLLIDED:
        left = right = -75;
        back = 0;
        if(Collisions::needsNewPath(robot)) {
            this->assignNewPath(robot->getRobotPosition());
        }
        return false;
        break;
    default:
        throw "This never happens.";
        break;
    }
#endif

    if(!pathEndInfo.hasFoundPathEnd)
    {
        /* Primary: Obstacle avoidance
         * Robot is currently in pathfinding mode
         * and has not travelled to the end of the path
         */
        Point robotPoint = robot->getRobotPosition();

        /**** Dynamic path updating ****/
        if(currentPathIsClear) //No known obstacles in path; test for new ones
        {
            if(pathQueue.empty())
                return false;
            Point nextPoint = this->pathQueue.front();
            Point obsPoint;
            bool isNewObstacleInPath
                = FPPA::isObstacleInLine(robotPoint, nextPoint, &obsPoint);

            /* An interesting problem is that if the ending point for the
             * current line is right before an obstacle, then that obstacle 
             * will not be detected and it will be hit. So, if there are enough
             * points, this also checks the NEXT line
             */
            if(!isNewObstacleInPath && pathQueue.size() > 2) 
            {
                const Point& nextNextPoint = pathQueue[1];
                isNewObstacleInPath 
                    = FPPA::isObstacleInLine(nextPoint, nextNextPoint, &obsPoint, useAvoidBall);
            }
            
            if(isNewObstacleInPath && !Measurments::isClose(obsPoint, lastObsPoint, 100)) 
            {
                /* We have a possible obstacle..
                 * If it is NOT close any of the obstacles used to generate
                 * the current path, it is a new obstacle
                 */
                if(std::none_of(lastObstacles.begin(), lastObstacles.end(),
                    [&](const Point& pt) {
                        return Measurments::isClose(obsPoint, pt, 100);
                    }))
                {
                    lastObsPoint = obsPoint;
                    currentPathIsClear = false;
                }
            }
            
            /*********************************************
             * Velocity Calculating (Important part)
             *********************************************/

            float nextAngle = Measurments::angleBetween(robotPoint, nextPoint);
            this->calculateVels(robot, nextPoint, nextAngle, moveType);
            
            /**********///Path Queue Updating
            if(Measurments::isClose(robotPoint, nextPoint, nextDistTolerance)) {
                pathQueue.pop_front();
                if(pathQueue.empty()) {
                    pathEndInfo.hasFoundPathEnd = true;  //Finished path
                    pathEndInfo.endingPoint = robotPoint;
                } 
                else if(pathQueue.size() == 1) {
                    nextDistTolerance = lastDistTolerance;
                    nextTargetAngle   = m_targetAngle;
                }
            }
        } else {
            /* There is a known obstacle in the current path
             * We want to rebuild the path and set the (new)
             * path to be clear again. This also happens on the 
             * initial condition that the robot has no path after
             * construction
             */
            this->assignNewPath(robotPoint);
            currentPathIsClear = true;
        }
    } else {
        /* Secondary: Rotating mode
         * Dropped into by default, if there is a desired
         * optional end orientation, that rotation is done here
         */
        Point robotPoint = robot->getRobotPosition();
        double robotAngle = robot->getOrientation();
        this->calculateVels(robot, m_targetPoint, m_targetAngle, moveType);
        if (Measurments::isClose(m_targetPoint, robotPoint, lastDistTolerance) &&
                Measurments::isClose(m_targetAngle, robotAngle, lastAngTolerance))
        {
            lfront=lback=rfront=rback=left=right=back=0;
            return true;
        }
    }

    return false;   //Skill not finished
}


void Move::assignNewPath(const Point& robotPoint)
{ 
    FPPA::PathInfo p = FPPA::findShortestPath
            (robotPoint, m_targetPoint, useAvoidBall, lastDirection);
    this->pathQueue.assign(p.first.begin(), p.first.end());
    this->lastDirection = p.second;
    this->lastObstacles = FPPA::getCurrentObstacles();    //Copies

    // TEST of drawPath functions
    for (unsigned int i=1; i<pathQueue.size(); i++){
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

    // Ryan has perpetrated this boolean check
    if (!GuiInterface::getGuiInterface()->isOverriddenBot()[robot->id]) {
        switch(robot->type())
        {
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
