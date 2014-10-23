#include "kick.h"
#include "include/config/robot_types.h"

namespace Skill
{

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
            robot->setL(100);
            robot->setR(100);
        }
        roboKick = false;
    }

	return true;
}

}
