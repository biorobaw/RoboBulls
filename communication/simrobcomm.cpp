#include <math.h>
#include "include/grSim_Packet.pb.h"
#include "include/grSim_Commands.pb.h"
#include "include/grSim_Replacement.pb.h"
#include "include/config/team.h"
#include "model/robot.h"
#include "simrobcomm.h"

SimRobComm::SimRobComm()
{
    //Shamsi Sim Address
   // _addr = "131.247.14.101";

    //James Sim Address
    _addr = "131.247.14.106";

    //Narges Sim Address
    //_addr = "127.0.0.1";


    _port = 20011;
}

void SimRobComm::sendVelsLarge(std::vector<Robot *>& robots)
{
    for (Robot* rob : robots)
    {
        bool kick = rob->getKick();
        bool dribble = rob->getDrible();
        sendPacket(rob->getID(),rob->getLF(),rob->getRF(),rob->getLB(),rob->getRB(),
                   kick,dribble);
        rob->setKick(0);
        rob->setDrible(0);
    }
}

void SimRobComm::sendPacket(int id, int LF, int RF, int LB, int RB, bool kick, bool drible)
{
    grSim_Packet packet;
    bool yellow = TEAM == 1;

    packet.mutable_commands()->set_isteamyellow(yellow);
    packet.mutable_commands()->set_timestamp(0.0);

    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();

    // Fill in simulator packet
    command->set_id(id);
    command->set_wheelsspeed(true);
    command->set_wheel1(-LF * mappingRatio);    //Left Forward
    command->set_wheel2(-LB * mappingRatio);    //Left Backward
    command->set_wheel3( RB * mappingRatio);    //Right Backward
    command->set_wheel4( RF * mappingRatio);    //Right Forward
    command->set_veltangent(0);
    command->set_velnormal(0);  // No normal velocity, differentials cannot move sideways
    command->set_velangular(0);
    command->set_kickspeedx(kick ? 6 : 0);
    command->set_kickspeedz(0); // No chipper
    command->set_spinner(drible ? 20 : 0);

    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}
