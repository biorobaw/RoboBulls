#ifndef MOVE_COLLISIONS_H
#define MOVE_COLLISIONS_H

#include <QMap>
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "configuration/constants.h"

class Robot;
class Point;
class GameState;


//! @brief Indicates the robot is free to move.
#define MOVE_OK        0

//! @brief Indicates the robot should be stationary to yield to another robot
#define MOVE_YIELDING  1

//! @brief Indicates the robot has collided with another
#define MOVE_COLLIDED  2



 /*! @brief Contains structures and routines for detecting and avoiding collisions
  * between multiple robots.
  * @author JamesW
  *
  * The movement of a robot is defined as one of three
  * states: MOVE_OK, MOVE_YIELDING, and MOVE_COLLIDED. When detected as collided, one
  * or both robots will back up. When yeilding, one robot will stop for another.
  * This is an internal file and should not be included directly. */

struct CollisionStatus;
class Collisions
{

public:
    Collisions(GameState* game_state);
    ~Collisions();

    //! @brief Returns the MOVE_OK...MOVE_COLLIDED for a given robot. */
    int getMoveStatus(Robot* robot);


    /*! @brief Returns the best direction the robot should back up in to exit a collision
     * @details If `robot` is collided (getMoveStatus(robot) == MOVE_COLLIDED) this returns
     * the best direction the robot should back up in to avoid the other robot
     * it collided with. */
    Point getBackupDirection(Robot* robot);

    void updateAllCollisions();

protected:
    /*! @brief Updates the "moving" statuses of the provided robot and sets it to
     *  MOVE_OK/MOVE_YIELDING/MOVE_COLLIDED based on some factors */
    void updateCollision(Robot* robot);
    
    GameState* game_state;

    CollisionStatus* status[2][MAX_ROBOTS_PER_TEAM];


};


#endif
