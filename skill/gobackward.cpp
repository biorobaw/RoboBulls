#include "gobackward.h"
#include "Model/robot.h"

GoBackward::GoBackward()
{

}

void GoBackward::perform(Robot *robot)
{
    cout<<"Going Backward!"<<endl;

    robComm *nxtbee = robComm::getnxtbee();

//    //********************************************************************
//    //Checking shamsi's code
//    float left_wheel_velocity = robot->getLeftWheelVelocity();
//    float right_wheel_velocity = robot->getRightWheelVelocity();
//    nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());
//    //********************************************************************

    nxtbee->sendVels(-30, -30, robot->getID());
}
