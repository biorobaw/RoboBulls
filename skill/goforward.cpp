#include "goforward.h"
#include "communication/robcomm.h"
#include "Model/gamemodel.h"
#include "Model/robot.h"

GoForward::GoForward()
{

}

void GoForward::perform(Robot *robot)
{
    cout<<"Going Forward!"<<endl;
    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(30, 30, robot->getID());
}
