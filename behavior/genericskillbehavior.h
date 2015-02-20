#ifndef GENERIC_SKILL_BEHAVIOR_H
#define GENERIC_SKILL_BEHAVIOR_H

#include "behavior/behavior.h"
#include "model/robot.h"

template<typename SkillType>
class GenericSkillBehavior : public Behavior
{
public:
	template<typename... Args>
	 GenericSkillBehavior(Args&&... args);
	~GenericSkillBehavior();
	void perform(Robot* robot);
private:
	SkillType* skill;
};

template<typename SkillType>
template<typename... Args>
GenericSkillBehavior<SkillType>::GenericSkillBehavior(Args&&... args)
	: skill(new SkillType(args...))
	{ }

template<typename SkillType>
GenericSkillBehavior<SkillType>::~GenericSkillBehavior()
{
	delete skill;
}

template<typename SkillType>
void GenericSkillBehavior<SkillType>::perform(Robot* robot)
{
	skill->perform(robot);
}

#endif
