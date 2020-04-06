#ifndef STOP_SKILL_H
#define STOP_SKILL_H
#include "../skill.h"

namespace Skill
{

/*! @brief A Skill to stop the robot
 * @details Sets the robot's wheel velocities to zero. */

class Stop : public Skill
{
public:
	bool perform(Robot *);
};

}

#endif
