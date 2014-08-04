#include "skill/basic_movement.h"

namespace Skill
{

bool GoForward::perform(Robot* robot)
{
    robot->setL(30);
    robot->setR(30);
	return true;
}


bool GoBackward::perform(Robot* robot)
{
    robot->setL(-30);
    robot->setR(-30);
	return true;
}


bool Stop::perform(Robot* robot)
{
    robot->setL(0);
    robot->setR(0);
	return true;
}

}
