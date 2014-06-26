#include "skill/basic_movement.h"

namespace Skill
{

bool GoForward::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();
    nxtbee->sendVels(30, 30, robot->getID());
	return true;
}


bool GoBackward::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();
    nxtbee->sendVels(-30, -30, robot->getID());
	return true;
}


bool Stop::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();
    nxtbee->sendVels(0, 0, robot->getID());
	return true;
}

}
