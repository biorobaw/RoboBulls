#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"
#include "model/robot.h"
#include "skill/skill.h"
#include "utilities/paramlist.h"

class SimpleBehaviors : public Behavior
{
public:
    SimpleBehaviors(const ParameterList& list);
    void perform(Robot*);
private:
    Skill::Skill *s;
};

#endif // SIMPLEBEHAVIORS_H
