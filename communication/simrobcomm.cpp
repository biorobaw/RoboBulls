#include "simrobcomm.h"
#include <math.h>
#include "include/globals.h"
#include "model/robot.h"

SimRobComm::SimRobComm()
{
    _addr = "131.247.14.106";
    //_addr = "127.0.0.1";
    _port = 20011;
}

void SimRobComm::sendVelsLarge(vector<Robot*> robots)
{
    for (Robot * r : robots)
        sendPacket(r->getID(), r->getL(), r->getR(), r->getKick(), r->getDrible());
}

void SimRobComm::sendVels(int leftVel, int rightVel, int robotId)
{
    sendPacket(robotId, leftVel, rightVel, false, true);
}

void SimRobComm::sendKick(int robotId)
{
    sendPacket(robotId, 0, 0, true, false);
}


void SimRobComm::sendPacket(int id, int leftVel, int rightVel, bool kick, bool drible)
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

    command->set_wheel1(-(double)leftVel/100 * mappingRatio);
    command->set_wheel2(-(double)leftVel/100 * mappingRatio);
    command->set_wheel3((double)rightVel/100 * mappingRatio);
    command->set_wheel4((double)rightVel/100 * mappingRatio);

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
