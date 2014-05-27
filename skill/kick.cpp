#include "kick.h"

Kick::Kick()
{
}

void Kick::perform(Robot *myRobot)
{
    RobComm *nxtbee = RobComm::getRobComm();
//    int count = 0;
    roboKick = true;
//    if (count > 4)
//    {
    while (roboKick)
    {
        nxtbee->sendKick(myRobot->getID());
//        count++;
//        if (count > 1)
            roboKick = false;
    }
//        roboKick = 0;
//    }
//    count++;
}

//void Kick::perform(Robot *rob)
//{

//}
