#include "kick.h"

namespace Skill
{

bool Kick::perform(Robot *myRobot)
{

    RobComm *nxtbee = RobComm::getRobComm();
    roboKick = true;

    while (roboKick)
    {
        nxtbee->sendKick(myRobot->getID());
        roboKick = false;
    }
	
	return true;
}

}
