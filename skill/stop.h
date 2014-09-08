#ifndef STOP_SKILL_H
#define STOP_SKILL_H

#include "skill/skill.h"
#include "model/robot.h"

namespace Skill
{

class Stop : public Skill
{
public:
	bool perform(Robot *);
};

}

#endif
