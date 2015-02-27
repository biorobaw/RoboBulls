#ifndef GENERIC_SKILL_BEHAVIOR_H
#define GENERIC_SKILL_BEHAVIOR_H

#include "behavior/behavior.h"

/* GenericSkillBehavior
 * GenericSkillBehavior is a wrapper convience behavior for a single skill.
 * Used in the robot's assignSkill function, this behavior simply performs
 * a single skill indefinately, and "isFinished" is determined directly
 * from that skill.
 */

class Robot;

template<typename SkillType>
class GenericSkillBehavior : public Behavior
{
public:
    template<typename... Args>
     GenericSkillBehavior(Args&&... args);
    ~GenericSkillBehavior();
    void perform(Robot* robot);
    bool isFinished() override;
private:
    SkillType* skill;
    bool skillFinished;
};

template<typename SkillType>
template<typename... Args>
GenericSkillBehavior<SkillType>::GenericSkillBehavior(Args&&... args)
    : skill(new SkillType(args...))
    , skillFinished(false)
    { }

template<typename SkillType>
GenericSkillBehavior<SkillType>::~GenericSkillBehavior()
{
    delete skill;
}

template<typename SkillType>
bool GenericSkillBehavior<SkillType>::isFinished()
{
    return skillFinished;
}

template<typename SkillType>
void GenericSkillBehavior<SkillType>::perform(Robot* robot)
{
    skillFinished = skill->perform(robot);
}

#endif
