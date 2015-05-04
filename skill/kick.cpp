#include "kick.h"
#include "include/config/robot_types.h"

namespace Skill
{

Kick::Kick():lVel(100), rVel(100)
{

}

Kick::Kick(int leftVel, int rightVel)
{
    lVel = leftVel;
    rVel = rightVel;
}

bool Kick::perform(Robot * robot)
{
    robot->setKick(1);
    if (robot->type() == differential)
    {
        robot->setL(lVel);
        robot->setR(rVel);
    }

	return true;
}

}
