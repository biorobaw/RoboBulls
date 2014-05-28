#include "kick.h"

Kick::Kick()
{
}

void Kick::perform(Robot *myRobot)
{

    RobComm *nxtbee = RobComm::getRobComm();
    roboKick = true;

    while (roboKick)
    {
        nxtbee->sendKick(myRobot->getID());
        roboKick = false;
    }
}

