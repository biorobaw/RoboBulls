#include "skill/basic_movement.h"

namespace Skill
{

void GoForward::perform(Robot* robot)
{
    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(30, 30, robot->getID());
}


void GoBackward::perform(Robot* robot)
{
    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(-30, -30, robot->getID());
}


void Stop::perform(Robot* robot)
{
    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(0, 0, robot->getID());
}

}
