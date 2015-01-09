#include <vector>
#include <stdlib.h>
#include <string.h>
#include "include/config/team.h"
#include "utilities/measurments.h"
#include "utilities/comparisons.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "movement/move_collisions.h"

/************************************************************************/
/* USER CONFIGURATION */

/* Defines the number of calls to Collision::update (via calcObstacleAvoidance;
 * called any time move->perform() is called on a robot with obstacle avoidance)
 * that must entail to update the robot-robot collision status of each robot.
 * This actually serves two purposes:
 * 1) Efficiency, and 2) Reduce amount of false-positives
 */
#define ROBOT_MOVE_UPDATE_COUNT     3
/* Defines the distance that the robot's position must change
 * for the robot to be considered moving. This is used to have the robots
 * not yield to a robot that is not moving.
 */
#define ROBOT_MOVING_DIST_TOL       110
/* Defines the amount of times each robot's distance has to be considered
 * "not moving" for the yielding robots to not yield to that robot
 */
#define ROBOT_MOVE_DIST_COUNT       4
/* Defines the amount of counts to update, while a robot is backing up,
 * until it is free to move forward again
 */
#define ROBOT_MOVE_BACKUP_COUNT     95
/* Defines the distance the robot must move back from its collision point before
 * it is allowed to move forward again. Beware, if this is too small, the robot
 * will never find a path around the collision object due to the close-to-start
 * FPPA exclusion rule.
 */
#define ROBOT_MOVE_BACKUP_DIST      (ROBOT_SIZE*1.5)

/************************************************************************/
/* Implementation */

namespace Movement
{
namespace Collisions
{
namespace detail
{
    /* Internal structure to keep track of all move/collide statuses
     * of each robot
     */
    struct RobotMoveStatus
    {
        RobotMoveStatus();
        int  status();
        bool moving();
        void update(Robot* robot);
        void updateMovingStatusOnly(Robot* robot);
        void set(int newStatus);
    private:
        void    updateMoveOk(Robot* robot);
        void    updateMoveNotOK(Robot* robot);
        void    updateMoveCollided(Robot* robot);
        int     m_status;          //MOVE_ status
        bool    m_isMoving;        //Is robot moving?
        Point   m_lastDiffPoint;   //Last point observed moving at
        Point   m_collidePoint;    //If COLLIDED, where did it collide
        Robot*  m_collideBot;      //If COLLIDED, what robot did it collide with
        int     m_observeCount;    //Count of times observed non-moving
        int     m_collideCounter;  //If COLLIDED, Counter to how long robot is backing up
        friend bool detail::needsNewPath(Robot *robot);
    };
    
    /****************************************************/
    /* State Variables */

    /* "Move statuses" of all robots. Contains MOVE_OK/MOVE_COLLIDED status 
     * as well as variables to keep track of when to switch states on each. This
     * array is updated with a call to update()
     */
    static RobotMoveStatus currentMoveStatuses[10];
    
    /* Bool for each robot representing if that robot is yielding to another robot */
    static bool currentMoveStops[10];
    
    /* A vector of all robots on the field. Placed here for convience. Populated
     * on a call to moveUpdateStart() and emptied on moveUpdateEnd()
     */
    static std::vector<Robot*> currentAllRobots;
    
    /****************************************************/

    int getMoveStatus(Robot* robot) 
    {
        return currentMoveStatuses[robot->getID()].status();
    }

    void moveUpdateStart()
    {
        //Obtains all robots on both teams
        GameModel* gm = GameModel::getModel();
        currentAllRobots = gm->getMyTeam();
        for(Robot* rob : gm->getOponentTeam())
            currentAllRobots.push_back(rob);

        //Assumes all robots can move at the start of a move-collide update
        memset(currentMoveStops, false, 10*sizeof(bool));
    }

    void moveUpdateEnd()
    {
        currentAllRobots.clear();    //Gets rid of knowledge of all robots
    }

    void update() 
    {
        /* One of the best ways to do this might be to automatically assume
         * they can move each iteration, and let the code find an exception again
         * that makes them not able to move. Then we can just tighten the code
         * that finds the movement exception
         */
        GameModel* gm = GameModel::getModel();
        moveUpdateStart();
        
        for(Robot* robot : gm->getMyTeam()) {
		    currentMoveStatuses[robot->getID()].updateMovingStatusOnly(robot);
        }
        
        for(Robot* robot : gm->getMyTeam()) {
            RobotMoveStatus& stat = currentMoveStatuses[robot->getID()];
            if(stat.status() != MOVE_COLLIDED) {
                stat.set(MOVE_OK);
            }
            currentMoveStatuses[robot->getID()].update(robot);
        }
        moveUpdateEnd();
    }

    bool needsNewPath(Robot *robot)
    {
        RobotMoveStatus& status = currentMoveStatuses[robot->getID()];
        int   backupCount  = status.m_collideCounter;
        Point collidePoint = status.m_collidePoint;

        if(status.m_collideBot != NULL) {
            collidePoint = status.m_collideBot->getRobotPosition();
        }

        bool farFromHit = Measurments::distance(robot->getRobotPosition(),collidePoint)
                >= ROBOT_MOVE_BACKUP_DIST*0.80;
        bool collideCounterClose = abs(backupCount - ROBOT_MOVE_BACKUP_COUNT)
                <= ROBOT_MOVE_BACKUP_COUNT*0.30;

        return (farFromHit or collideCounterClose);
    }

    
    /****************************************************/
    /* Static Utility Functions */
    
    void robotDisableMovement(Robot* a, Robot* b)
    {
        if(currentMoveStatuses[b->getID()].moving()) {
            currentMoveStatuses[a->getID()].set(MOVE_NOTOK);
            currentMoveStops[a->getID()] = true;
        }
    }

    bool robotFacingRobot(Robot* a, Robot* b)
    {
        int tolerance = M_PI/3;
        return Comparisons::isFacingPoint(a, b, tolerance);
    }

    bool robotCollideHazard(Robot* a, Robot* b)
    {
        int tolerance = 300;
        return Comparisons::isDistanceToLess(a, b, tolerance);
    }
    
    /****************************************************/
    /* RobotMoveStatus Implementation */
    
    RobotMoveStatus::RobotMoveStatus()
        : m_status(MOVE_OK)
        , m_isMoving(true)
        , m_lastDiffPoint(Point(9999,9999))
        , m_collidePoint(Point(9999,9999))
        , m_observeCount(0)
        , m_collideCounter(0)
        {}
        
    int RobotMoveStatus::status()
    {
        return this->m_status;
    }
    
    bool RobotMoveStatus::moving()
    {
        return this->m_isMoving;
    }
    
    void RobotMoveStatus::set(int newStatus)
    {
        switch(newStatus) 
        {
        case MOVE_OK:
            break;
        case MOVE_NOTOK:
            break;
        case MOVE_COLLIDED:
            m_collideCounter = 0;
            break;
        }
        m_status = newStatus;
    }
    
    void RobotMoveStatus::update(Robot* robot)
    {
        switch(m_status) 
        {
        case MOVE_OK:
            updateMoveOk(robot);
            break;
        case MOVE_NOTOK:
            updateMoveNotOK(robot);
            break;
        case MOVE_COLLIDED:
            updateMoveCollided(robot);
            break;
        }
    }
    
    void RobotMoveStatus::updateMovingStatusOnly(Robot* robot)
    {
        Point currentPos = robot->getRobotPosition();
        if(Measurments::distance(currentPos, m_lastDiffPoint) > ROBOT_MOVING_DIST_TOL) {
            m_isMoving      = true;
            m_lastDiffPoint = currentPos;
            m_observeCount  = 0;
        } else {
            if(m_observeCount >= ROBOT_MOVE_DIST_COUNT) {
                m_isMoving = false;
            } else {
                ++m_observeCount;
            }
        }
    }
    
    void RobotMoveStatus::updateMoveOk(Robot* robot)
    {
        for(Robot* other : currentAllRobots)
        {
            bool hazardExists   = robotCollideHazard(robot, other);
            bool otherIsStopped = (currentMoveStops[other->getID()] == true);
            if((robot == other) or otherIsStopped or not(hazardExists)) {
                continue;
            }
            
            Point robPos = robot->getRobotPosition();
            Point othPos = other->getRobotPosition();
            int closeTol = ROBOT_RADIUS * 2.5;
            bool robOtherTooClose = Measurments::distance(robPos, othPos)
                    <= closeTol;
            bool robFacingOther = robotFacingRobot(robot, other);

            if(robOtherTooClose and robFacingOther) {
                set(MOVE_COLLIDED);
                currentMoveStops[robot->getID()] = true;
                this->m_collidePoint = othPos;
                this->m_collideBot = other;
                continue;
            }
            
            if(robotFacingRobot(robot, other) and robotFacingRobot(other, robot)) {
                if(other->isOnMyTeam()) {
                    if(robot->hasBeh and other->hasBeh) {
                        /* TODO: Add Behavior Priorities. For now this if-statement
                         * doesn't really matter
                         */
                        robotDisableMovement(other, robot);
                    } else {
                        robotDisableMovement(robot, other);
                    }
                }
                else {
                    /* For now I'm just doing this. If this is compiled for the yellow team,
                     * we just give precedence to the blues. It's fancy to show.
                     */
                #if TEAM == TEAM_YELLOW
                    robotDisableMovement(robot, other);
                #endif
                }
            }
            else if(robotFacingRobot(robot, other)) {
                robotDisableMovement(robot, other);
            }
            else if(robotFacingRobot(other, robot)) {
                robotDisableMovement(other, robot);
            }
            else {
                //No robots are facing each other
            }
        }
    }
    
    void RobotMoveStatus::updateMoveNotOK(Robot* robot)
    {
        UNUSED_PARAM(robot);
    }
    
    void RobotMoveStatus::updateMoveCollided(Robot* robot)
    {
        bool maxBackupCountHit = ++m_collideCounter >= ROBOT_MOVE_BACKUP_COUNT;
        bool isFarAwayFromHit  = false;

        if(m_collideBot != NULL) {
            isFarAwayFromHit = Comparisons::isDistanceToGreater
                (robot, m_collideBot, ROBOT_MOVE_BACKUP_DIST);
        }

        if(isFarAwayFromHit or maxBackupCountHit) {
            m_collideCounter = 0;
            m_collidePoint = Point(9999,9999);
            set(MOVE_OK);    //Robot can now move
        }
    }
}    //namespace detail

    static int moveUpdateCounter;

    void update() 
    {
        if(++moveUpdateCounter >= ROBOT_MOVE_UPDATE_COUNT) {
            moveUpdateCounter = 0;
            detail::update();
        }
    }
    
    int getMoveStatus(Robot* robot) 
    {
        return detail::getMoveStatus(robot);
    }

    bool needsNewPath(Robot* robot)
    {
        return detail::needsNewPath(robot);
    }
}
}
