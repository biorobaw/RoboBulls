#include "skill/basic_movement.h"

namespace Skill
{

void GoForward::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();

    nxtbee->sendVels(30, 30, robot->getID());
}


void GoBackward::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();

    nxtbee->sendVels(-30, -30, robot->getID());
}


void Stop::perform(Robot* robot)
{
    RobComm *nxtbee = RobComm::getRobComm();

    nxtbee->sendVels(0, 0, robot->getID());
}

}
