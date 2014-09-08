#include "include/config/tolerances.h"
#include "movement/move.h"

namespace Movement
{

/* This might be better off a settable member function. 
 * It's a low tolerance to not produce problems when users need 
 * a wide range of tolerances, and also  make it the user's responsibility 
 * to only recreate this object if the points are changed significantly.
 */
#define ENOUGH_TO_CHANGE 25


Move::Move()
    : velMultiplier(1.0)
{
}

Move::~Move()
{
}

Move::Move(Point targetPoint, float targetAngle, bool withObstacleAvoid)
    : velMultiplier(1.0)
{
    recreate(targetPoint, targetAngle, withObstacleAvoid);
}


void Move::recreate(Point targetPoint, float targetAngle, bool withObstacleAvoid)
{
    if(Measurments::distance(m_targetPoint, targetPoint) > ENOUGH_TO_CHANGE) {
        /* In most cases, this is called each loop to track a possibly moving point.
         * But that is very inefficient in that basically the pathfinding is 
         * remade each time, and other expensive things are done. So if the new point
         * is close enough to the other, let's not do anything
         */
        m_targetPoint      = targetPoint;
        m_targetAngle      = targetAngle;
        isInitialized      = false;
        useObstacleAvoid   = withObstacleAvoid;
        hasFoundPathEnd    = false;
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
    else if(!Measurments::isClose(m_targetAngle, targetAngle)) {
        /* ROT_TOLERANCE defines the tolerance at which two angles are
         * considered equal. So if the angles are "equal", let's not
         * bother updating it.
         */
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
    if(!Measurments::isClose(m_targetPoint, robot->getRobotPosition()) ||
       !Measurments::isClose(m_targetAngle, robot->getOrientation())) 
	{
        if(useObstacleAvoid) {
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
    /* Using regular movement, we calculate the velocities to move 
     * directly to the target point and angle.
     */
    this->calculateVels(robot, m_targetPoint, m_targetAngle, moveType);

#if SIMULATED
    int dist_tolerance = DIST_TOLERANCE;
#else
    int dist_tolerance = DIST_TOLERANCE;
#endif

    Point robotPos = robot->getRobotPosition();
    float robotAng = robot->getOrientation();

    return Measurments::isClose(m_targetPoint, robotPos, dist_tolerance) &&
           Measurments::isClose(m_targetAngle, robotAng);
}


bool Move::calcObstacleAvoidance(Robot* robot, Type moveType)
{
    if(!hasFoundPathEnd)
    {
        /* Primary: Obstacle avoidance
         * Robot is currently in pathfinding mode
         * and has not travelled to the end of the path
         */
        Point robotPoint = robot->getRobotPosition();

        /**** Dynamic path updating ****/
        if(currentPathIsClear) //No known obstacles in path; test for new ones
        {
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
                const Point& nextNextPoint = pathQueue[2];
                isNewObstacleInPath 
					= FPPA::isObstacleInLine(nextPoint, nextNextPoint, &obsPoint);
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
             * ******************************************/
            this->calculateVels(robot, nextPoint, nextTargetAngle, moveType);
            
            /**********///Path Queue Updating
            if(Measurments::isClose(robotPoint, nextPoint, nextDistTolerance)) {
                pathQueue.pop_front();
                if(pathQueue.empty()) {
                    hasFoundPathEnd = true;  //Finished path
                } 
				else if(pathQueue.size() == 1) {
                    nextDistTolerance = DIST_TOLERANCE;
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
        if(this->m_targetAngle == UNUSED_ANGLE_VALUE)
            return true;
        else if(Measurments::isClose(robot->getOrientation(), m_targetAngle))
            return true;

        this->calculateVels(robot, robot->getRobotPosition(), m_targetAngle, moveType);
    }

    return false;   //Skill not finished
}


void Move::assignNewPath(const Point& robotPoint)
{
    FPPA::PathInfo p = FPPA::findShortestPath(robotPoint, m_targetPoint, lastDirection);
    this->pathQueue.assign(p.first.begin(), p.first.end());
    this->lastDirection = p.second;
    this->lastObstacles = FPPA::getCurrentObstacles();	//Copies
}


void Move::setVels(Robot *robot)
{
    /* This is the function that, after velocity is calculated above,
     * sets the velocities on the actual robot. This then funnels down
     * to sendVelsLarge.
     */
    switch(robot->type())
    {
    case differential:
        robot->setL(left  * velMultiplier);
        robot->setR(right * velMultiplier);
        break;
    case threeWheelOmni:
		robot->setL(left  * velMultiplier);
		robot->setR(right * velMultiplier);
		robot->setB(back  * velMultiplier);    //?
        break;
    case fourWheelOmni:
		robot->setLF(lfront * velMultiplier);
		robot->setRF(rfront * velMultiplier);
		robot->setLB(lback  * velMultiplier);
		robot->setRB(rback  * velMultiplier);
        break;
    }
}


}
