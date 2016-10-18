#include "include/config/simulated.h"
#include "simrobcomm.h"
#include "nxtrobcomm.h"
#include "yisirobcomm.h"
#include "robcomm.h"

RobComm * RobComm::robcomm = NULL;

RobComm * RobComm::getRobComm()
{
    if (robcomm == NULL)
    {
    #if SIMULATED
        robcomm = new YisiRobComm();
    #else
        robcomm = new YisiRobComm();
    #endif
    }

    return robcomm;
}
