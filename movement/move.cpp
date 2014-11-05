#include <string.h>
#include "include/config/tolerances.h"    //ROT/DIST Tolerance
#include "include/config/globals.h"        //OVERALL_VELOCITY
#include "movement/move.h"
#include "gui/guiinterface.h"


/* Set this to 1 to use the robot-robot obstacle avoidance routines.
 * Current problems (with known solutions):
 *   - After a time, Robots hit active non-moving (i.e., "at target") robots
 *     even though they are close to the robots
 */
#define MOVEMENT_USE_ROB_COLLIDE    1
/* Defines the number of calls to calcObstacleAvoidance
 * (called any time move->perform() is called on a robot with obstacle avoidance)
 * that must entail to update the robot-robot collision status of each robot.
 * This actually serves two purposes:
 * 1) Efficiency. The state of two robots about to collide will very likely not change
 *    multiple times 20/40/55 times a second, so we don't need to update that often.
 * 2) Accuracy and correctness. Because real life is full of errors, not updating the 
 *    status reduces outlying false-positives by ensuring that, if the robot is
 *    told to stop, it will not have the chance to move again for a period of time.
 *    However, this makes false-negatives more harmful by possibly not correcting
 *    them sooner.
 */
#define ROBOT_COLLIDE_UPDATE_COUNT 3
/* Defines the distance that the robot's position must change
 * for the robot to be considered moving. This is used to have the robots
 * not stop if the other robot is not moving
 */
#define ROBOT_COLLIDE_DIST_TOL     110
/* Defines the amount of times each robot's distance has to be considered
 * "not moving" for the yielding robots to not yield to that robot
 */
#define ROBOT_COLLIDE_DIST_UPDATE  75

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
                if(Measurments::distance(robPos, pathEndInfo.endingPoint) > 100) {
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

#if MOVEMENT_USE_ROB_COLLIDE
/***********************************************/
/*** Robot-Robot collision avoidance section ***/
/***********************************************/

bool robotIDMoveStatus[] = {
 /*0*/ true, /*1*/ true,
 /*2*/ true, /*3*/ true,
 /*4*/ true, /*5*/ true,
 /*6*/ true, /*7*/ true,
 /*8*/ true, /*9*/ true
};

struct moveStat
{
    bool  isMoving;
    Point lastDiffPoint;
    int   observeCount;
};

moveStat robotIDMovingStats[] = {
 /*0*/ {false, Point(0,0), 0}, /*1*/ {false, Point(0,0), 0},
 /*2*/ {false, Point(0,0), 0}, /*3*/ {false, Point(0,0), 0},
 /*4*/ {false, Point(0,0), 0}, /*5*/ {false, Point(0,0), 0},
 /*6*/ {false, Point(0,0), 0}, /*7*/ {false, Point(0,0), 0},
 /*8*/ {false, Point(0,0), 0}, /*9*/ {false, Point(0,0), 0}
};

int robotMoveStatusUpdate = 0;


void robotDisableMovement(Robot* a, Robot* b)
{
    int aid = a->getID();
    int bid = b->getID();
    if(robotIDMovingStats[bid].isMoving) {
        robotIDMoveStatus[aid] = false;
    }
}

bool robotFacingPoint(Robot* a, Point b)
{
    Point robPosA = a->getRobotPosition();
    float robAngleA = a->getOrientation();
    float ABAng = Measurments::angleBetween(robPosA, b);
    return Measurments::isClose(robAngleA, ABAng, M_PI/3);
}


bool robotCollideHazard(Robot* a, Robot* b)
{
    Point robPosA = a->getRobotPosition();
    Point robPosB = b->getRobotPosition();
    return Measurments::isClose(robPosA, robPosB, 500);
}

void configurRobotMovingStatus()
{
    /* Here we configure and update the moving status of the robots.
     * If the robots position has been seen to not change for
     * ROBOT_COLLIDE_DIST_UPDATE times, then we set the robot's moving
     * status to false.
     */
    GameModel* gm = GameModel::getModel();
    for(Robot* robot : gm->getMyTeam())
    {
        int robID = robot->getID();
        moveStat& idMoveStat  = robotIDMovingStats[robID];
        Point currentPos  = robot->getRobotPosition();
        int&   idObserveCount = idMoveStat.observeCount;
        Point& idLastDiffPos  = idMoveStat.lastDiffPoint;
        bool&  idIsMoving     = idMoveStat.isMoving;

        if(Measurments::distance(currentPos, idLastDiffPos)
                > ROBOT_COLLIDE_DIST_TOL) {
            idIsMoving = true;
            idLastDiffPos = currentPos;
            idObserveCount = 0;
        } else {
            if(++idObserveCount >= ROBOT_COLLIDE_DIST_UPDATE) {
                idIsMoving = false;
            }
        }
    }
}


void configureRobotMoveStatus()
{
    GameModel* gm = GameModel::getModel();
    std::vector<Robot*> allRobots(gm->getMyTeam());
    for(Robot* rob : gm->getOponentTeam())
        allRobots.push_back(rob);

    memset(robotIDMoveStatus, true, sizeof(robotIDMoveStatus));
    configurRobotMovingStatus();

    for(unsigned i =  0;  i != allRobots.size(); ++i)
    for(unsigned j = i+1; j != allRobots.size(); ++j)
    {
        Robot* robotA = allRobots.at(i);
        Robot* robotB = allRobots.at(j);
        int robotA_ID = robotA->getID();
        int robotB_ID = robotB->getID();

        /* Ensures at least one of the robots is on myTeam. We don't care if
         * the enemy robots collide with themselves
         */
        if(not(robotA->isOnMyTeam() or robotB->isOnMyTeam()))
            continue;

        Point robPosA = robotA->getRobotPosition();
        Point robPosB = robotB->getRobotPosition();
        bool robotAShouldStop = robotCollideHazard(robotA, robotB);
        bool robotBShouldStop = robotCollideHazard(robotB, robotA);

        if(robotAShouldStop xor robotBShouldStop) {
            /* In this case, only one of the robots cannot move. Here,
             * if that robot is on myTeam we just stop them
             */
            if(robotA->isOnMyTeam() and robotAShouldStop) {
                robotDisableMovement(robotA, robotB);
            } else if(robotB->isOnMyTeam() and robotBShouldStop) {
                robotDisableMovement(robotB, robotA);
            }
        }
        else if(robotAShouldStop and robotBShouldStop) {
            /* In this case, both of the robots should not move, likely because
             * they are facing each other. First, I want to get the status of
             * who is facing who, and if only a single robot is facing another,
             * stop that robot. If both are, then we look their Behavior priorities
             * and the lower priority must yield
             */
            bool AfacingB = robotFacingPoint(robotA, robPosB);
            bool BfacingA = robotFacingPoint(robotB, robPosA);

            if(robotB->isOnMyTeam() and BfacingA and not(AfacingB)) {
                robotDisableMovement(robotB, robotA);
            }
            else if(robotA->isOnMyTeam() and AfacingB and not(BfacingA)) {
                robotDisableMovement(robotA, robotB);
            }
            else if(AfacingB and AfacingB) {
                if(robotA->isOnMyTeam() and robotB->isOnMyTeam()) {
                    if(robotA->hasBeh and robotB->hasBeh) {
                        int priorityA = robotA->getCurrentBeh()->getPriority();
                        int priorityB = robotB->getCurrentBeh()->getPriority();
                        if(priorityA > priorityB) {
                            robotDisableMovement(robotB, robotA);
                        } else if(priorityB > priorityA){
                            robotDisableMovement(robotA, robotB);
                        } else {
                            if(robotA_ID > robotB_ID) {
                                robotDisableMovement(robotB, robotA);
                            } else {
                                robotDisableMovement(robotA, robotB);
                            }
                        }
                    }
                } else if(robotA->isOnMyTeam()) {
                    robotDisableMovement(robotA, robotB);
                } else {
                    robotDisableMovement(robotB, robotA);
                }
            } else {
                /* Here, no robots are facing another (AfacingB nor BfacingA).
                 * Likely, this should never happen, because robotAShouldStop and
                 * robotBShouldStop should not return true is nobody is facing another..
                 */
            }
        }
    }
}
#endif

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
    if(--robotMoveStatusUpdate <= 0) {
        robotMoveStatusUpdate = ROBOT_COLLIDE_UPDATE_COUNT;
        configureRobotMoveStatus();
    }
    if(robotIDMoveStatus[robot->getID()] == false) {
        left = right = back = lfront = lback = rfront = rback = 0;
        return false;
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
        if(currentPathIsClear and !pathQueue.empty()) //No known obstacles in path; test for new ones
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
        if(Measurments::isClose(robot->getOrientation(), m_targetAngle, lastAngTolerance)) {
            lfront=lback=rfront=rback=left=right=back=0;
            return true;
        } else {
            this->calculateVels(robot, robot->getRobotPosition(), m_targetAngle, moveType);
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
