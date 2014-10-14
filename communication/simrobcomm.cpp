#include "simrobcomm.h"

#include <math.h>
#include "include/config/team.h"
#include "model/robot.h"

SimRobComm::SimRobComm()
{
    //Shamsi Sim Address
    _addr = "131.247.14.101";

    //James Sim Address
    //_addr = "131.247.14.106";

    //Narges Sim Address
//    _addr = "127.0.0.1";
    _port = 20011;
}

void SimRobComm::sendVels(int leftVel, int rightVel, int robotId)
{
    sendPacket(robotId, leftVel, rightVel, leftVel, rightVel, false, true);
}

void SimRobComm::sendVelsLarge(std::vector<Robot *>& robots)
{
    for (Robot* rob : robots)
    {
        bool kick    = rob->getKick();
        bool dribble = rob->getDrible();
        sendPacket(rob->getID(),rob->getLF(),rob->getRF(),rob->getLB(),rob->getRB(),
                   kick,dribble);
        rob->setKick(0);
		rob->setDrible(0);
    }
}

void SimRobComm::sendVelsThreeOmni(int left,int right, int back, int ID)
{
    UNUSED_PARAM(left);
    UNUSED_PARAM(right);
    UNUSED_PARAM(back);
    UNUSED_PARAM(ID);
}

void SimRobComm::sendKick(int robotId)
{
    sendPacket(robotId, 0, 0, 0, 0, true, false);
}


void SimRobComm::sendPacket(int id, int LF, int RF, int LB, int RB, bool kick, bool drible)
{
    grSim_Packet packet;
    // TODO: set team
    bool yellow = TEAM == 1;

    packet.mutable_commands()->set_isteamyellow(yellow);
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
    command->set_id(id);

    // Adapt velocities to the format and range of the simulator
    // The simulator needs a forward velocity and spin velocity separate
    //double forwardVel = (leftVel + rightVel) / 2;
    //double rotationVel = (leftVel - rightVel) / 2;
    double forwardVel = 0;
    double rotationVel = 0;
    // Supposing that the robot goes 1 m/s at full speed
    forwardVel = forwardVel * 1 / 100;
    // Supposing the robot rotates PI/4 at max rotation speed (100, -100).
    rotationVel = rotationVel * M_PI / 200;
    // Set the actual velocities
    command->set_wheelsspeed(true);

    command->set_wheel1(-(double)LF/100 * mappingRatio);    //Left Forward
    command->set_wheel2(-(double)LB/100 * mappingRatio);    //Left Backward
    command->set_wheel3((double)RB/100 * mappingRatio);     //Right Backward
    command->set_wheel4((double)RF/100 * mappingRatio);     //Right Forward


    command->set_veltangent(forwardVel);
    // No normal velocity, differentials cannot move sideways
    command->set_velnormal(0);
    command->set_velangular(rotationVel);

    if (kick)
        command->set_kickspeedx(6);
    else
        command->set_kickspeedx(0);
    // No chipper
    command->set_kickspeedz(0);
    if (drible)
        command->set_spinner(20);
    else
        command->set_spinner(0);

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}
