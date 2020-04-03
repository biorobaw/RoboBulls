#ifndef GENERIC_SKILL_BEHAVIOR_H
#define GENERIC_SKILL_BEHAVIOR_H
#include "../behavior.h"
class Robot;

/*!
 * @file
 * @author James Waugh
 *
 * @brief A wrapper convenience behavior for a single skill.
 * @details Used in the robot's assignSkill function, this behavior simply performs
 * a single skill indefinitely, and "isFinished" is determined directly
 * from that skill.
 */

template<typename SkillType>
class GenericSkillBehavior : public Behavior
{
public:
    template<typename... Args>
    GenericSkillBehavior(Robot* robot, Args&&... args);

    ~GenericSkillBehavior();

     //!< @brief Perform the skill
    void perform() override;

    //!< @brief The isFinished returns the result from <i>perform</i> of the Skill
    bool isFinished() override;

    string getName() override;
private:
    SkillType* skill;
    bool skillFinished;
};

template<typename SkillType>
template<typename... Args>
GenericSkillBehavior<SkillType>::GenericSkillBehavior(Robot*, Args&&... args)
    : Behavior(robot),
      skill(new SkillType(args...))
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
void GenericSkillBehavior<SkillType>::perform()
{
    skillFinished = skill->perform(robot);
}

template<typename SkillType>
string GenericSkillBehavior<SkillType>::getName() {
    return "GenericSkill";
}

#endif
