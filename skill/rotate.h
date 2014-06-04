#ifndef ROTATE_H
#define ROTATE_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "model/robot.h"


/*  Skill Rotate
    April 25th 2014. Contributor: JW

    Rotates the robot to a given angle. This is done by stopping the robot
    and determining fastest direction, then rotating at a constant velocity
    until the two angles are close enough. Combined with GoToPosition, this can
    result in movement and rotation at the same time.
    */
namespace Skill {

class Rotate : public Skill
{
public:
    Rotate();
    Rotate(float angle, float* lmv_out = nullptr, float* rmv_out = nullptr);
    void perform(Robot* robot);

private:
    float targetAngle;
    int	  direction;
    float* lmv_ptr, *rmv_ptr;
};

}

#endif // ROTATE_H
