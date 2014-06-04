#include "robcomm.h"
#include <stdio.h>
#include "include/globals.h"
#include "simrobcomm.h"
#include "nxtrobcomm.h"

RobComm * RobComm::robcomm = NULL;

RobComm * RobComm::getRobComm()
{

    if (robcomm == NULL)
#if SIMULATED
        robcomm = new SimRobComm();
#else
        robcomm = new NXTRobComm();
#endif

    return robcomm;
}
