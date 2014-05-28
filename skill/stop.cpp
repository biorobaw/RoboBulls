#include "stop.h"
#include "Model/robot.h"

Stop::Stop()
{

}

void Stop::perform(Robot *robot)
{
    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(0, 0, robot->getID());
}
