#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"
#include "model/robot.h"
#include "skill/skill.h"
#include "skill/kicktopoint.h"
#include "utilities/paramlist.h"
#include "movement/gotopositionwithorientation.h"
#include "utilities/point.h"


class SimpleBehaviors : public Behavior
{
public:
    SimpleBehaviors(const ParameterList& list);
    void perform(Robot*);
private:
    Point target = Point (-1600, 0);
    Movement::GoToPosition move;
};

#endif // SIMPLEBEHAVIORS_H
