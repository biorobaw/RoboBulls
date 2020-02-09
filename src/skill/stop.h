#ifndef STOP_SKILL_H
#define STOP_SKILL_H
#include "src/skill/skill.h"
#include "src/model/robot.h"

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
