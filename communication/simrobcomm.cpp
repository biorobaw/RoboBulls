#include <math.h>
#include "libs/grsim/include/messages/grSim_Packet.pb.h"
#include "libs/grsim/include/messages/grSim_Commands.pb.h"
#include "libs/grsim/include/messages/grSim_Replacement.pb.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "simrobcomm.h"
#include <iostream>
#include "include/game_constants.h"

SimRobComm::SimRobComm(const char* addr, int port)
{
    _addr = addr;
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
    packet.mutable_commands()->set_isteamyellow( (GameModel::OUR_TEAM == TEAM_YELLOW) );
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
    if(gameModel->hasRobotReplacements)
    {
        for(GameModel::RobotReplacement& data : gameModel->robotReplacements) {
            grSim_RobotReplacement* robotReplace = replacement->add_robots();
            robotReplace->set_x(data.x / 1000.0);
            robotReplace->set_y(data.y / 1000.0);
            robotReplace->set_yellowteam((data.team == TEAM_YELLOW));
            robotReplace->set_id(data.id);
            robotReplace->set_dir(data.dir * (180.0/M_PI));
        }
        gameModel->hasRobotReplacements = false;
        gameModel->robotReplacements.clear();
    }
    if(gameModel->hasBallReplacement)
    {
        grSim_BallReplacement* ballReplace = replacement->mutable_ball();
        const GameModel::BallReplacement& data = gameModel->ballReplacement;
        ballReplace->set_x(data.x / 1000.0);
        ballReplace->set_y(data.y / 1000.0);
        ballReplace->set_vx(data.vx);
        ballReplace->set_vy(data.vy);
        gameModel->hasBallReplacement = false;
        gameModel->ballReplacement = {0, 0, 0, 0};
    }

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
