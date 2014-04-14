#include "gobackward.h"
#include "Robot/robot.h"

GoBackward::GoBackward()
{

}

void GoBackward::perform(Robot *robot)
{
    cout<<"Going Backward!"<<endl;

    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(-40, -40, robot->getID());
}
