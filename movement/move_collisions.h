#ifndef MOVE_COLLISIONS_H
#define MOVE_COLLISIONS_H

/* Movement Collisons
 * Contains structures and routines for detecting and avoiding collisions
 * between multiple robots. The movement of a robot is defined as one of three
 * states: MOVE_OK, MOVE_YIELDING, and MOVE_COLLIDED. When detected as collided, one
 * or both robots will back up. When yeilding, one robot will stop for another.
 * This is an internal file and should not be included directly.
 *  ______________________________________________________
 * [    ______  ___  _   _    ___  _  _  _    _    ___    ]
 * [   |_    _||  _|| | | |  | _ || || || \__| |  /   |   ]
 * [     |  |  | |_ | |_| |  ||_||| || ||      | |   _|   ]
 * [     |  |  |  _||  _  |  |  _|| /\ || |\   | _\  \    ]
 * [     |  |  | |_ | | | |  | |  | || || | \  ||    |    ]
 * [     |__|  |___||_| |_|  |_|  |_||_||_|  \_||___/     ]
 * [______________________________________________________]
 */
class Robot;
class Point;

/* Movement states. OK indicates the robot is free to move.
 * YIELDING means the robot should be stationary to yield to another robot
 * COLLIDED means the robot has collided with another */
#define MOVE_OK        0
#define MOVE_YIELDING  1
#define MOVE_COLLIDED  2

namespace Movement
{
namespace Collisions 
{
    /* Updates all "moving" statuses of each robot and sets each to
     * MOVE_OK/MOVE_STOPPED/MOVE_COLLIDED based on some factors */
    void update();
    
    /* Returns the MOVE_OK...MOVE_COLLIDED for a given robot. */
    int getMoveStatus(Robot* robot);

    /* if `robot` is collided (getMoveStatus(robot) == MOVE_COLLIDED) this returns
     * the best direction the robot should back up in to avoid the other robot
     * it collided with. */
    Point getBackupDirection(Robot* robot);
}
}

#endif
