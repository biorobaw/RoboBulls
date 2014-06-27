#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include "skill/skill.h"
#include "model/robot.h"

namespace Skill
{

class GoForward : public Skill
{
public:
	bool perform(Robot *robot);
};


class GoBackward : public Skill
{
public:
	bool perform(Robot *);
};


class Stop : public Skill
{
public:
	bool perform(Robot *);
};

}


#endif

