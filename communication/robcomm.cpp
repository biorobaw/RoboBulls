#include "robcomm.h"
#include <stdio.h>
#include "include/globals.h"
#include "simrobcomm.h"
#include "nxtrobcomm.h"

RobComm * RobComm::robComm = NULL;

RobComm * RobComm::getRobComm()
{

    if (robComm == NULL)
#ifdef SIMULATED
        robComm = new SimRobComm();
#else
        robComm = new NXTRobComm();
#endif

    return robComm;


}
