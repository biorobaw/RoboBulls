#include "kick.h"

Kick::Kick()
{
}

void Kick::perform(Robot *myRobot)
{
    robComm *nxtbee = robComm::getnxtbee();
    roboKick = true;

    while (roboKick)
    {
        nxtbee->sendKick(myRobot->getID());
        roboKick = false;
    }
}

