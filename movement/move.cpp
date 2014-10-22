#include "include/config/tolerances.h"    //ROT/DIST Tolerance
#include "include/config/globals.h"        //OVERALL_VELOCITY
#include "movement/move.h"
#include "gui/mainwindow.h"

namespace Movement
{

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
   // if(!Measurments::isClose(m_targetPoint, robot->getRobotPosition(), lastDistTolerance) ||
     //  !Measurments::isClose(m_targetAngle, robot->getOrientation(), lastAngTolerance))
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

#if 0
static bool robotIDMoveStatus[] = {
 /*0*/	true,
 /*1*/	true,
 /*2*/	true,
 /*3*/	true,
 /*4*/	true,
 /*5*/	true,
 /*6*/	true,
 /*7*/	true,
 /*8*/	true,
 /*9*/	true
};

bool robotFacingRobot(Robot* a, Robot* b)
{
    Point robPos = a->getRobotPosition();
    float robAngle = a->getOrientation();
    float angleBetween = Measurments::angleBetween
            (robPos, b->getRobotPosition());
    return Measurments::isClose(robAngle, angleBetween, M_PI/6);
}

bool robotCollideHazard(Robot* a, Robot* b)
{
#if 0
    return Measurments::isClose(a->getRobotPosition(), b->getRobotPosition()) &&
           robotFacingRobot(a, b);
#else
    return Measurments::isClose(a->getRobotPosition(), b->getRobotPosition());
#endif
}

void configureRobotMoveStatus()
{
    GameModel* gm = GameModel::getModel();
    auto& myTeam = gm->getMyTeam();
    auto& opTeam = gm->getOponentTeam();
    std::vector<Robot*> allRobots(myTeam.size() + opTeam.size());
    std::copy(myTeam.begin(), myTeam.end(), allRobots.begin());
    std::copy(opTeam.begin(), opTeam.end(), allRobots.begin() + myTeam.size());

    for(unsigned i = 0;  i != allRobots.size(); ++i)
    for(unsigned j =i+1; j != allRobots.size(); ++j)
    {
        Robot* robotA = allRobots.at(i);
        Robot* robotB = allRobots.at(j);
        bool robotACanMove, robotBCanMove;

        robotACanMove = !robotCollideHazard(robotA, robotB);
        robotBCanMove = !robotCollideHazard(robotB, robotA);

        if(!robotACanMove || !robotBCanMove) {
            if(robotFacingRobot(robotA, robotB)) {
                robotIDMoveStatus[robotA->getID()] = false;
            }
            else if(robotFacingRobot(robotB, robotA))  {
                robotIDMoveStatus[robotB->getID()] = false;
            }
            else {
                int priorityA = robotA->getCurrentBeh()->getPriority();
                int priorityB = robotB->getCurrentBeh()->getPriority();
                if(priorityA > priorityB) {
                    robotIDMoveStatus[robotB->getID()] = false;
                } else {
                    robotIDMoveStatus[robotA->getID()] = false;
                }
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
        return true;
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
                const Point& nextNextPoint = pathQueue[1];
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
            float nextAngle = Measurments::angleBetween(robotPoint, nextPoint);
            this->calculateVels(robot, nextPoint, nextAngle, moveType);
            
            /**********///Path Queue Updating
            if(Measurments::isClose(robotPoint, nextPoint, nextDistTolerance)) {
                pathQueue.pop_front();
                if(pathQueue.empty()) {
                    hasFoundPathEnd = true;  //Finished path
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
            left = right = 0;
            return true;
        } else {
            this->calculateVels(robot, robot->getRobotPosition(), m_targetAngle, moveType);
        }
    }

    return false;   //Skill not finished
}


void Move::assignNewPath(const Point& robotPoint)
{
    FPPA::PathInfo p = FPPA::findShortestPath(robotPoint, m_targetPoint, lastDirection);
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
    if (MainWindow::getMainWindow()->overriddenBots[robot->id] == false) {
        MainWindow::getMainWindow()->guiPrint("guiOverride OFF");
        MainWindow::getMainWindow()->guiPrintRobot(robot->id, "guiOverride OFF \n");
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
    } else {
        MainWindow::getMainWindow()->guiPrint("guiOverride ON");
        MainWindow::getMainWindow()->guiPrintRobot(robot->id, "guiOverride ON \n");
    }
}


}
