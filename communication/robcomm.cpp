#include "include/config/simulated.h"
#include "simrobcomm.h"
#include "nxtrobcomm.h"
#include "robcomm.h"

RobComm * RobComm::robcomm = NULL;

RobComm * RobComm::getRobComm()
{
    if (robcomm == NULL)
    {
    #if SIMULATED
        robcomm = new NXTRobComm();
    #else
        robcomm = new NXTRobComm();
    #endif
    }

    return robcomm;
}
