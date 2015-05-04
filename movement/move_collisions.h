#ifndef MOVE_COLLISIONS_H
#define MOVE_COLLISIONS_H

/* Movement Collisons
 * Contains structures and routines for detecting and avoiding collisions
 * between multiple robots. the movement of a robot is defined as one of three
 * states; MOVE_OK, MOVE_NOTOK, and MOVE_COLLIDED. When detected as collided, one
 * or both robots will back up.
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

#define MOVE_OK        0
#define MOVE_NOTOK     1
#define MOVE_COLLIDED  2

namespace Movement
{
namespace Collisions 
{
    /* Updates all "moving" statuses of each robot and sets each to
     * MOVE_OK/MOVE_NOTOK/MOVE_COLLIDED based on some factors
     */
    void update();
    
    /* Returns the MOVE_OK...MOVE_COLLIDED for a given robot. */
    int  getMoveStatus(Robot* robot);
    
    /* Returns a bool representing if the robot needs new path. Happens
     * when the robot has backed up after MOVE_COLLIDED has been set
     */
    bool needsNewPath(Robot* robot);
}
}

#endif
