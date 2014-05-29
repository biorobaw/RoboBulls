#include "stop.h"
#include "Model/robot.h"

Stop::Stop()
{

}

void Stop::perform(Robot *robot)
{

    RobComm *nxtbee = RobComm::getRobComm();

    nxtbee->sendVels(0, 0, robot->getID());
}
