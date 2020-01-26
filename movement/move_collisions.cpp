#include <vector>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "model/game_state.h"
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
#define ROBOT_MOVE_UPDATE_COUNT     5
/* Defines the distance that the robot's position must change
 * for the robot to be considered moving. This is used to have the robots
 * not yield to a robot that is not moving.
 */
#define ROBOT_MOVING_DIST_TOL       60
/* Defines the amount of times each robot's distance has to be considered
 * "not moving" for the yielding robots to not yield to that robot
 */
#define ROBOT_MOVE_DIST_COUNT       10
/* Defines the amount of counts to update, while a robot is backing up,
 * until it is free to move forward again
 */
#define ROBOT_MOVE_BACKUP_COUNT     90
/* Defines the distance the robot must move back from its collision point before
 * it is allowed to move forward again. Beware, if this is too small, the robot
 * will never find a path around the collision object due to the close-to-start
 * FPPA exclusion rule.
 */
#define ROBOT_MOVE_BACKUP_DIST      (ROBOT_RADIUS * 4)
/* Defines a distance such that if two robots are within this distance, and one is
 * facing the other, it will yield to the other robot. This is to avoid a collison
 * when in a "collizion hazard"
 */
#define ROBOT_COLLIDE_HAZARD_DIST   ROBOT_RADIUS * 3
/* Defines a "very close" distance such that if two robots are within this distance
 * to each other, they can be considered for being collided with each other
 * collided. **Beware** This needs to be less than ROBOT_COLLIDE_HAZARD_DIST
 */
#define ROBOT_COLLIDED_DIST         (ROBOT_RADIUS*2 + 30)

/************************************************************************/
/* Implementation */

namespace Move {
namespace Collisions {
namespace detail {

/*! @brief Internal structure to keep track of all move/collide statuses of each robot */
struct RobotMoveStatus
{
    RobotMoveStatus();
    int   status() const;          //!<Returns MOVE_* status
    bool  moving() const;          //!<Returns true if seen moiving
    void  update(Robot* robot);    //!<Updates the status and moving status
    Point backupDirection() const; //!<Gets the unit direction the robot should backup in

private:
    //Querying and helper functions
    bool robotFacingRobot(Robot* a, Robot* b);
    bool robotCollideHazard(Robot* a, Robot* b);
    bool areCollided(Robot* a, Robot* b);
    bool shouldYieldFor(Robot* robot, Robot* other);

private:
    //Stat and functions to update state
    void   set(int newStatus);
    void   updateIsMovingStatus(Robot* robot);
    void   updateMoveOk(Robot* robot);
    void   updateMoveYielding(Robot* robot);
    void   updateMoveCollided(Robot* robot);
    int    m_status;          //!<MOVE_* status
    bool   m_isMoving;        //!<Is robot moving?
    int    m_observeCount;    //!<Count of times observed non-moving
    Point  m_lastDiffPoint;   //!<Last point observed moving at
    Robot* m_collideBot;      //!<for YIELDING/COLLIDED, what robot did it collide with
    int    m_collideCounter;  //!<If COLLIDED, Counter to how long robot is backing up
    Point  m_collideDirection;//!<If COLLIDED, unit direction robot should backup in
};

/****************************************************/
/* State Variables */

/*! @brief stored RobotMoveStatus for all robots.
 * @details This container contains both Blue/Yellow statuses,
 * and is needed to differentiate same IDs with different teams
 */
struct RobotMoveStatusContainer
{
    /*! @brief Returns a status in moveStatusesMine or moveStatusesOpponent
     * based on robot's team */
    RobotMoveStatus& operator[](Robot* robot) {
        return status[robot->getTeamId()][robot->getID()];
    }
    RobotMoveStatus status[2][10];
}
currentMoveStatuses;

/* A vector of all robots on the field. Placed here for convience. Populated
 * on a call to moveUpdateStart() and emptied on moveUpdateEnd()
 */

/****************************************************/
//detail interface functions


void update()
{
    for(Robot* robot : Robot::getAllRobots())
        currentMoveStatuses[robot].update(robot);
}

int getMoveStatus(Robot* robot)
{
    return currentMoveStatuses[robot].status();
}

/****************************************************/
/* RobotMoveStatus Implementation */

RobotMoveStatus::RobotMoveStatus()
    : m_status(MOVE_OK)
    , m_isMoving(true)
    , m_observeCount(0)
    , m_lastDiffPoint(Point(9999,9999))
    , m_collideCounter(0)
    { }

int RobotMoveStatus::status() const
{
    return m_status;
}

bool RobotMoveStatus::moving() const
{
    return m_isMoving;
}

Point RobotMoveStatus::backupDirection() const
{
    return m_collideDirection;
}

void RobotMoveStatus::set(int newStatus)
{
    switch(newStatus)
    {
    case MOVE_OK:
        break;
    case MOVE_YIELDING:
        break;
    case MOVE_COLLIDED:
        m_collideCounter = 0;
        break;
    }
    m_status = newStatus;
}

bool RobotMoveStatus::robotFacingRobot(Robot* a, Robot* b)
{
    float tolerance = 100 * (M_PI / 180);

    /* For the omni robots, they don't face each other. So we look at their
     * velocity direction to determine "facing" (TODO: Fix this) */
    if(a->getDriveType() != differential) {
        Point vel = a->getVelocityMetersPerSecond();
        float ang = atan2(vel.y, vel.x);
        float bad = Measurements::angleBetween(a, b);
        return Measurements::isClose(ang, bad, tolerance);
    } else {
        return Comparisons::isFacingPoint(a, b, tolerance);
    }
}

bool RobotMoveStatus::robotCollideHazard(Robot* a, Robot* b)
{
    return Measurements::distance(a,b) < ROBOT_COLLIDE_HAZARD_DIST;
}

bool RobotMoveStatus::areCollided(Robot *a, Robot *b)
{
    //If the other guy is stopped, we don't consider them collided
    if(!currentMoveStatuses[b].moving())
        return false;
    bool robotsTooClose = Measurements::distance(a, b) <= ROBOT_COLLIDED_DIST;
    bool robFacingOther = robotFacingRobot(a, b);
    return robotCollideHazard(a,b) && robotsTooClose && robFacingOther;
}

bool RobotMoveStatus::shouldYieldFor(Robot* robot, Robot* other)
{
    /* `robot` should yield to `other` if robot is facing other,
     * they are in close proximity, and other is in motion */
    return robotFacingRobot(robot, other) &&
           robotCollideHazard(robot,other) &&
           currentMoveStatuses[other].moving();
}

void RobotMoveStatus::update(Robot* robot)
{
    //Updates m_isMoving before we act on it (ncluding enemy team)
    updateIsMovingStatus(robot);

    //We don't keep tabs on if the opponents are yielded or collided
    if(!robot->getTeam()->isControlled())
        return;

    switch(m_status)
    {
    case MOVE_OK:
        updateMoveOk(robot);
        break;
    case MOVE_YIELDING:
        updateMoveYielding(robot);
        break;
    case MOVE_COLLIDED:
        updateMoveCollided(robot);
        break;
    }
}

void RobotMoveStatus::updateIsMovingStatus(Robot* robot)
{
    /* The robot *is* moving if it is ROBOT_MOVING_DIST_TOL away from its last
     * recorded moved position */
    if(Measurements::distance(robot, m_lastDiffPoint) > ROBOT_MOVING_DIST_TOL) {
        m_isMoving      = true;
        m_lastDiffPoint = robot->getPosition();
        m_observeCount  = 0;
    } else {
        /* The robot is *not* moving after we've seen it's distance close enough
         * for ROBOT_MOVE_DIST_COUNT times */
        if(++m_observeCount >= ROBOT_MOVE_DIST_COUNT) {
            m_isMoving = false;
        }
    }
}

void RobotMoveStatus::updateMoveOk(Robot* robot)
{
    for(Robot* other : Robot::getAllRobots())
    {
        if(robot == other)
            continue;

        /* If we are collided or should yield, update state
         * and store the robot collided/yielded with */
        if(areCollided(robot, other)) {
            set(MOVE_COLLIDED);
            m_collideBot = other;
        }
        else if(shouldYieldFor(robot, other)) {
            set(MOVE_YIELDING);
            m_collideBot = other;
        }
    }
}

void RobotMoveStatus::updateMoveYielding(Robot* robot)
{
    //We can go back to moving if the collidebot has stopped, or it is far away
    bool farAway = Measurements::distance(robot, m_collideBot) > ROBOT_COLLIDE_HAZARD_DIST;
    if(farAway || !currentMoveStatuses[m_collideBot].moving())
        set(MOVE_OK);
}

void RobotMoveStatus::updateMoveCollided(Robot* robot)
{
    /* We can go back to moving if we are far enough from the
     * collision bot, or the backup count times out.
     * Otherwise update the backup direciton. This is directly away from the collidebot.
     */
    bool maxBackupCountHit = ++m_collideCounter >= ROBOT_MOVE_BACKUP_COUNT;
    bool isFarAwayFromHit  = Comparisons::isDistanceToGreater(robot, m_collideBot, ROBOT_MOVE_BACKUP_DIST);

    if(isFarAwayFromHit or maxBackupCountHit) {
        m_collideCounter = 0;
        m_collideDirection = Point(0,0);
        set(MOVE_OK);
    } else {
        float angle = Measurements::angleBetween(m_collideBot, robot);
        m_collideDirection = Point(cos(angle), sin(angle));
    }
}

}   //namespace detail

/************************************************/
//Movement::Collisions top-level interface methods

void update()
{
    static int moveUpdateCounter;
    if(++moveUpdateCounter > ROBOT_MOVE_UPDATE_COUNT) {
        moveUpdateCounter = 0;
        detail::update();
    }
}

int getMoveStatus(Robot* robot)
{
    return detail::getMoveStatus(robot);
}

Point getBackupDirection(Robot* robot)
{
    return detail::currentMoveStatuses[robot].backupDirection();
}

}   //namespace Collisions
}   //namespace Movement
