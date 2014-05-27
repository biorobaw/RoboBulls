#include "goforward.h"
#include "communication/robcomm.h"
#include "Model/gamemodel.h"
#include "Model/robot.h"
#include "skill/closedloopcontrol.h"

GoForward::GoForward()
{

}

void GoForward::perform(Robot *robot)
{
    cout<<"Going Forward!"<<endl;
    RobComm *nxtbee = RobComm::getRobComm();


    nxtbee->sendVels(30, 30, robot->getID());
}
