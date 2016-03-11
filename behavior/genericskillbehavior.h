#ifndef GENERIC_SKILL_BEHAVIOR_H
#define GENERIC_SKILL_BEHAVIOR_H
#include "behavior/behavior.h"
class Robot;

/*! @brief A wrapper convenience behavior for a single skill.
 * @details Used in the robot's assignSkill function, this behavior simply performs
 * a single skill indefinitely, and "isFinished" is determined directly
 * from that skill. */

template<typename SkillType>
class GenericSkillBehavior : public Behavior
{
public:
    template<typename... Args>
    GenericSkillBehavior(Args&&... args);

    ~GenericSkillBehavior();

     //!< @brief Perform the skill
    void perform(Robot* robot);

    //!< @brief The isFinished returns the result from <i>perform</i> of the Skill
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
