#include "kick.h"
#include "include/config/robot_types.h"

namespace Skill
{
Kick::Kick():lVel(100), rVel(100)
{

}

Kick::Kick(int leftVel, int rightVel)
{
    lVel = leftVel;
    rVel = rightVel;
}

bool Kick::perform(Robot * robot)
{
//    #if TRACE
//        cout << "Performing Skill::Kick" << endl;
//    #endif

    //RobComm *nxtbee = RobComm::getRobComm();
    roboKick = true;

    while (roboKick)
    {
        //nxtbee->sendKick(robot->getID());
        robot->setKick(1);
        if (robot->type() == differential)
        {
            robot->setL(lVel);
            robot->setR(rVel);
        }
        roboKick = false;
    }

	return true;
}

}
