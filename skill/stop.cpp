#include "stop.h"
#include "Model/robot.h"

Stop::Stop()
{

}

void Stop::perform(Robot *robot)
{
    cout<<"Stoping!"<<endl;

    RobComm *nxtbee = RobComm::getRobComm();

//    //********************************************************************
//    //Checking shamsi's code
//    float left_wheel_velocity = robot->getLeftWheelVelocity();
//    float right_wheel_velocity = robot->getRightWheelVelocity();
//    nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());
//    //********************************************************************

    nxtbee->sendVels(0, 0, robot->getID());
}
