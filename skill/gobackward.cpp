#include "gobackward.h"
#include "Model/robot.h"

GoBackward::GoBackward()
{

}

void GoBackward::perform(Robot *robot)
{
    cout<<"Going Backward!"<<endl;

    robComm *nxtbee = robComm::getnxtbee();

    nxtbee->sendVels(-30, -30, robot->getID());
}
