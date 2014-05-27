#include "simrobcomm.h"
#include <math.h>

SimRobComm::SimRobComm()
{
    _addr = "127.0.0.1";
    _port = 20011;
}

void SimRobComm::sendVels(int leftVel, int rightVel, int robotId)
{
    sendPacket(robotId, leftVel, rightVel, false);
}

void SimRobComm::sendKick(int robotId)
{
    sendPacket(robotId, 0, 0, true);
}


void SimRobComm::sendPacket(int id, int leftVel, int rightVel, bool kick)
{
    grSim_Packet packet;
    // TODO: set team
    bool yellow = false;

    packet.mutable_commands()->set_isteamyellow(yellow);
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
    command->set_id(id);

    // Adapt velocities to the format and range of the simulator
    // The simulator needs a forward velocity and spin velocity separate
//    double forwardVel = (leftVel + rightVel) / 2;
//    double rotationVel = (leftVel - rightVel) / 2;
    double forwardVel = 0;
    double rotationVel = 0;
    // Supposing that the robot goes 1 m/s at full speed
    forwardVel = forwardVel * 1 / 100;
    // Supposing the robot rotates PI/4 at max rotation speed (100, -100).
    rotationVel = rotationVel * M_PI / 200;
    // Set the actual velocities
    command->set_wheelsspeed(true);
    command->set_wheel1(-(double)leftVel/100 * 20);
    command->set_wheel2(-(double)leftVel/100 * 20);
    command->set_wheel3((double)rightVel/100 * 20);
    command->set_wheel4((double)rightVel/100 * 20);

    command->set_veltangent(forwardVel);
    // No normal velocity, differentials cannot move sideways
    command->set_velnormal(0);
    command->set_velangular(rotationVel);

    if (kick)
        command->set_kickspeedx(2);
    else
        command->set_kickspeedx(0);
    // No chipper
    command->set_kickspeedz(0);
    // No spinner
    command->set_spinner(0);

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}
