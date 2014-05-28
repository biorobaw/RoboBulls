#ifndef GOTOPOSITIONFACING_H
#define GOTOPOSITIONFACING_H

#include "skill.h"
#include "Model/robot.h"
#include "skill/rotate.h"

class GoToPositionFacing : public Skill
{
public:
    GoToPositionFacing(Point target);
    GoToPositionFacing(float tx, float ty);

    void perform(Robot * robot);
private:
    Point   targetPosition;
};

#endif // GOTOPOSITIONFACING_H
