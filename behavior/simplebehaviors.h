#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"
#include "model/robot.h"
#include "skill/skill.h"
#include "skill/kicktopoint.h"
#include "movement/gotopositionwithorientation.h"
#include "utilities/point.h"


class SimpleBehaviors : public Behavior
{
public:
    SimpleBehaviors();
    void perform(Robot*);
private:
    Point target = Point (0, 0);
    bool hasTargetPos;
    Movement::GoToPosition move;
};

#endif // SIMPLEBEHAVIORS_H
