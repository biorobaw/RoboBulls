#include <math.h>
#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"
#include "src/model/robot.h"
#include "src/model/team.h"
#include "src/model/game_state.h"
#include "simrobcomm.h"
#include <iostream>
#include "src/utilities/measurements.h"

SimRobComm::SimRobComm(const char* addr, int port)
{
    _addr = QHostAddress(addr);
    _port = port;
}

void SimRobComm::sendVelsLarge(std::set<Robot*>& robots)
{
    //Send standard robot packets
    for (Robot* rob : robots)
    {
        sendPacket(rob);
        rob->setKick(0);
        //rob->setDrible(0);
    }

    //Send replacement packets
    sendReplacementPackets();
}

void SimRobComm::sendPacket(Robot* robot)
{
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow( robot->getTeamId() == TEAM_YELLOW );
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();

    //Retrive robot information
    int id = robot->getID();
    int LF = Measurements::clamp(robot->getLF(), -100, 100);
    int RF = Measurements::clamp(robot->getRF(), -100, 100);
    int LB = Measurements::clamp(robot->getLB(), -100, 100);
    int RB = Measurements::clamp(robot->getRB(), -100, 100);
    float kick = robot->getKick();
    bool  dribble = robot->getDribble();

    // Fill in simulator packet
    command->set_id(id);
    command->set_wheelsspeed(true);
    command->set_wheel1(-LF * mappingRatio);    //Left Forward
    command->set_wheel2(-LB * mappingRatio);    //Left Backward
    command->set_wheel3( RB * mappingRatio);    //Right Backward
    command->set_wheel4( RF * mappingRatio);    //Right Forward
    command->set_veltangent(0);
    command->set_velnormal(0);  // No normal velocity
    command->set_velangular(0);
    command->set_kickspeedx(kick);
    command->set_kickspeedz(0); // No chipper
    command->set_spinner(dribble ? 300 : 0);

    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}

void SimRobComm::sendReplacementPackets()
{
    grSim_Packet packet;
    grSim_Replacement* replacement = packet.mutable_replacement();

    //Fill in data if it is aviliable
    //For some reason the simulator multiplies the X and Y coordinates by 1000
    //It also takes angles in degrees
//    if(GameState::hasRobotReplacements)
//    {
//        for(GameState::RobotReplacement& data : GameState::robotReplacements) {
//            grSim_RobotReplacement* robotReplace = replacement->add_robots();
//            robotReplace->set_x(data.x / 1000.0);
//            robotReplace->set_y(data.y / 1000.0);
//            robotReplace->set_yellowteam((data.team == TEAM_YELLOW));
//            robotReplace->set_id(data.id);
//            robotReplace->set_dir(data.dir * (180.0/M_PI));
//        }
//        GameState::hasRobotReplacements = false;
//        GameState::robotReplacements.clear();
//    }
//    if(GameState::hasBallReplacement)
//    {
//        grSim_BallReplacement* ballReplace = replacement->mutable_ball();
//        const GameState::BallReplacement& data = GameState::ballReplacement;
//        ballReplace->set_x(data.x / 1000.0);
//        ballReplace->set_y(data.y / 1000.0);
//        ballReplace->set_vx(data.vx);
//        ballReplace->set_vy(data.vy);
//        GameState::hasBallReplacement = false;
//        GameState::ballReplacement = {0, 0, 0, 0};
//    }

    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}

void SimRobComm::close(){
    // udp socket seems to close automatically if necessary when destroyed
    // nothing to be done
}
