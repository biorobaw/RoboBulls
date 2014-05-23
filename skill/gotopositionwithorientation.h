#ifndef GOTOPOSITIONWITHORIENTATION_H
#define GOTOPOSITIONWITHORIENTATION_H

#include "skill.h"
#include "Model/robot.h"
#include "skill/rotate.h"

class GoToPositionWithOrientation : public Skill
{
public:
    GoToPositionWithOrientation(Point target);
    GoToPositionWithOrientation(float tx, float ty);

    void perform(Robot * robot);
private:
    Point   targetPosition;
};

#endif // GOTOPOSITIONWITHORIENTATION_H
