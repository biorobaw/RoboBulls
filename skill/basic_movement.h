#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include "skill/skill.h"
#include "model/robot.h"

namespace Skill
{

class GoForward : public Skill
{
public:
	void perform(Robot *robot);
};


class GoBackward : public Skill
{
public:
	void perform(Robot *);
};


class Stop : public Skill
{
public:
	void perform(Robot *);
};

}


#endif

