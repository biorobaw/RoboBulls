#ifndef ROTATE_H
#define ROTATE_H

#include "Performance/behavior.h"
#include "Performance/skill.h"
#include "Robot/robot.h"

#define ROT_TOLERANCE 6*(M_PI/180)
#define ROT_VELOCITY 40
#define CLOCKWISE   1
#define CCLOCKWISE -1

/*  Skill Rotate
    April 25th 2014. Contributor: James Waugh

    Rotates the robot to a given angle. This is done by stopping the robot
    and determining fastest direction, then rotating at a constant velocity
    until the two angles are close enough. Combined with GoToPosition, this can
    result in movement and rotation at the same time.

    Constants:
    ROT_TOLERANCE: The range the target and robot angle have to be to each other
        to be considered equal.
    ROT_VELOCITY:  Velocity applied to each motor in rotation.
    */
class Rotate : public Skill
{
public:
    Rotate();
    Rotate(float, bool finishedStop=false);
    void perform(Robot* robot);

private:
    float targetAngle;
    int	  direction;
    bool  stopWhenDone;
};

#endif // ROTATE_H
