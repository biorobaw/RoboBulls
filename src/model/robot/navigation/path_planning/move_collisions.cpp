#include <vector>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "model/game_state.h"
#include "model/robot/robot.h"
#include "move_collisions.h"
#include "model/team/team.h"

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


struct CollisionStatus {

    int    m_status = MOVE_OK;                  //!<MOVE_* status
    bool   m_isMoving = true;                   //!<Is robot moving?
    int    m_observeCount = 0;                  //!<Count of times observed non-moving
    Point  m_lastDiffPoint = Point(9999,9999);  //!<Last point observed moving at
    Robot* m_collideBot;                        //!<for YIELDING/COLLIDED, what robot did it collide with
    int    m_collideCounter = 0;                //!<If COLLIDED, Counter to how long robot is backing up
    Point  m_collideDirection;                  //!<If COLLIDED, unit direction robot should backup in

    void   set(int newStatus){
        if(newStatus == MOVE_COLLIDED)
            m_collideCounter = 0;
        m_status = newStatus;
    }



};

namespace  {





    bool isRobotFacingRobot(Robot* a, Robot* b)
    {
        float tolerance = 100 * (M_PI / 180);

        /* For the omni robots, they don't face each other. So we look at their
         * velocity direction to determine "facing" (TODO: Fix this) */
    //            if(a->getDriveType() != differential) {
            Point vel = a->getVelocity();
            float ang = atan2(vel.y, vel.x);
            float bad = Measurements::angleBetween(a, b);
            return Measurements::isClose(ang, bad, tolerance);
    //            } else {
    //                return Comparisons::isFacingPoint(a, b, tolerance);
    //            }
    }

    bool hasRobotCollideHazard(Robot* a, Robot* b)
    {
        return Measurements::distance(a,b) < ROBOT_COLLIDE_HAZARD_DIST;
    }


    bool areCollided(Robot *a, Robot *b)
    {
        //If the other guy is stopped, we don't consider them collided
        bool robotsTooClose = Measurements::distance(a, b) <= ROBOT_COLLIDED_DIST;
        bool robFacingOther = isRobotFacingRobot(a, b);
        return hasRobotCollideHazard(a,b) && robotsTooClose && robFacingOther;
    }



}





Collisions::Collisions(GameState* game_state) : game_state(game_state){
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            status[i][j] = new CollisionStatus;
}

Collisions::~Collisions(){
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            delete status[i][j];
}

void Collisions::updateAllCollisions(){
    for(auto r : game_state->getFieldRobots()) updateCollision(r);
}

void Collisions::updateCollision(Robot* robot)
{
    auto r_status = status[robot->getTeamId()][robot->getId()];

    //Updates m_isMoving before we act on it (ncluding enemy team)
    /* The robot *is* moving if it is ROBOT_MOVING_DIST_TOL away from its last
     * recorded moved position */
    if(Measurements::distance(robot, r_status->m_lastDiffPoint) > ROBOT_MOVING_DIST_TOL) {
        r_status->m_isMoving      = true;
        r_status->m_lastDiffPoint = *robot;
        r_status->m_observeCount  = 0;
    } else {
        /* The robot is *not* moving after we've seen it's distance close enough
         * for ROBOT_MOVE_DIST_COUNT times */
        if(++r_status->m_observeCount >= ROBOT_MOVE_DIST_COUNT) {
            r_status->m_isMoving = false;
        }
    }

    //We don't keep tabs on if the opponents are yielded or collided
    //            if(!robot->getTeam()->isControlled())
    //                return;

    switch(r_status->m_status)
    {
        case MOVE_OK:{
            for(Robot* other : game_state->getFieldRobots())
            {
                if(robot == other) continue;

                /* If we are collided or should yield, update state
                 * and store the robot collided/yielded with */
               auto  o_status = status[other->getTeamId()][other->getId()];

                if(o_status->m_isMoving && areCollided(robot, other)) {
                    r_status->set(MOVE_COLLIDED);
                    r_status->m_collideBot = other;
                }
                // check if robot should yield:
                else if( isRobotFacingRobot(robot, other) &&
                         hasRobotCollideHazard(robot,other) &&
                         o_status->m_isMoving) {
                    r_status->set(MOVE_YIELDING);
                    r_status->m_collideBot = other;
                }


            }
            break;
        }
        case MOVE_YIELDING: {
            //We can go back to moving if the collidebot has stopped, or it is far away
            auto c_robot = r_status->m_collideBot;
            auto c_status = status[c_robot->getTeamId()][c_robot->getId()];

            bool farAway = Measurements::distance(robot, c_robot) > ROBOT_COLLIDE_HAZARD_DIST;
            if(farAway || !c_status->m_isMoving) r_status->set(MOVE_OK);
            break;
        }
        case MOVE_COLLIDED:
        {
            /* We can go back to moving if we are far enough from the
             * collision bot, or the backup count times out.
             * Otherwise update the backup direciton. This is directly away from the collidebot.
             */
            bool maxBackupCountHit = ++r_status->m_collideCounter >= ROBOT_MOVE_BACKUP_COUNT;
            bool isFarAwayFromHit  = Comparisons::isDistanceToGreater(robot, r_status->m_collideBot, ROBOT_MOVE_BACKUP_DIST);

            if(isFarAwayFromHit || maxBackupCountHit) {
                r_status->m_collideCounter = 0;
                r_status->m_collideDirection = Point(0,0);
                r_status->set(MOVE_OK);
            } else {
                float angle = Measurements::angleBetween(r_status->m_collideBot, robot);
                r_status->m_collideDirection = Point(cos(angle), sin(angle));
            }


            break;
        }
    }


}

int Collisions::getMoveStatus(Robot* robot)
{
    return status[robot->getTeamId()][robot->getId()]->m_status;
}

Point Collisions::getBackupDirection(Robot* robot)
{
    return status[robot->getTeamId()][robot->getId()]->m_collideDirection;
}

