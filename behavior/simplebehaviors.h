#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"
#include "model/robot.h"
#include "skill/skill.h"
#include "utilities/paramlist.h"
#include "movement/gotopositionwithorientation.h"

class SimpleBehaviors : public Behavior
{
public:
    SimpleBehaviors(const ParameterList& list);
    void perform(Robot*);
private:
    Skill::Skill *s;
    Movement::GoToPosition move;
};

#endif // SIMPLEBEHAVIORS_H
