#ifndef GOTOPOSITIONFACING_H
#define GOTOPOSITIONFACING_H

#include "skill.h"
#include "model/robot.h"
#include "skill/rotate.h"

namespace Skill {

class GoToPositionFacing : public Skill
{
public:
    GoToPositionFacing(Point target);
    GoToPositionFacing(float tx, float ty);

    void perform(Robot * robot);
private:
    Point   targetPosition;
};

}

#endif // GOTOPOSITIONFACING_H
