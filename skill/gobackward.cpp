#include "gobackward.h"
#include "Model/robot.h"

GoBackward::GoBackward()
{

}

void GoBackward::perform(Robot *robot)
{
    RobComm *nxtbee = RobComm::getRobComm();

    nxtbee->sendVels(-30, -30, robot->getID());
}
