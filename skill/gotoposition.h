#ifndef GOTOPOSITION_H
#define GOTOPOSITION_H

#include "skill.h"
#include "model/robot.h"
#include "skill/rotate.h"

/* GoToPosition Skill
   April 25 2014. Contributor: James Waugh

   Moves the robot to a position. This is done by continually
   updating the rotation along a straight path. Unlike before,
   these components are not seperate; however, there are still
   issues with this implementation.

   -If the ball is too close to the side of the robot, it will
    do nothing, or infinitely loop around the ball.
*/
namespace Skill {

class GoToPosition: public Skill
{
public:
    GoToPosition(Point target);
    GoToPosition(float tx, float ty);

    void perform(Robot * robot);
private:
    float   angleDiff;
    bool    rotationDone;
    Point   targetPosition;
};

}

#endif // GOTOPOSITION_H
