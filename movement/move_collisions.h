#ifndef MOVE_COLLISIONS_H
#define MOVE_COLLISIONS_H

/* Movement Collisons
 * Contains structures and routines for detecting and avoiding collisions
 * between multiple robots. the movement of a robot is defined as one of three
 * states; MOVE_OK, MOVE_NOTOK, and MOVE_COLLIDED. When detected as collided, one
 * or both robots will back up
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
	void update();
    int  getMoveStatus(Robot* robot);
    bool needsNewPath(Robot* robot);
	
namespace detail
{
    void update();
    int  getMoveStatus(Robot *robot);
    bool needsNewPath(Robot *robot);
    void moveUpdateStart();
    void moveUpdateEnd();

    struct RobotMoveStatus
    {
        RobotMoveStatus();
        int  status();
        bool moving();
        void update(Robot* robot);
        void updateMovingStatusOnly(Robot* robot);
        void set(int newStatus);
    private:
        void 	updateMoveOk(Robot* robot);
        void 	updateMoveNotOK(Robot* robot);
        void 	updateMoveCollided(Robot* robot);
        int 	m_status;
        bool	m_isMoving;
        Point 	m_lastDiffPoint;
        Point   m_collidePoint;
        Robot*  m_collideBot;
        int  	m_observeCount;
        int 	m_collideCounter;
        int     m_moveDisableCount;
        friend bool detail::needsNewPath(Robot *robot);
    };
}

}	//namespace Collisions
}	//namespace Movement

#endif
