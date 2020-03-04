#include <math.h>
#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"
#include "robot/robot.h"
#include "model/team.h"
#include "model/game_state.h"
#include "simrobcomm.h"
#include "robot_grsim.h"
#include <iostream>
#include "utilities/measurements.h"

SimRobComm::SimRobComm(YAML::Node t_node)
{

    std::cout << "        GRSIM_ADDR : " <<  t_node["GRSIM_ADDR"] << std::endl
              << "        GRSIM_PORT : " << t_node["GRSIM_PORT"] << std::endl;

    std::string ip = t_node["GRSIM_ADDR"].as<std::string>();
    int port = t_node["GRSIM_PORT"].as<int>();

    _addr = QHostAddress(ip.c_str());
    _port = port;
}

void SimRobComm::sendVels(std::set<Robot*>& robots)
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
    RobotGrsim* r = (RobotGrsim*)robot;
    double wheelSpeeds[4];
    r->getWheelSpeeds(wheelSpeeds);


    double LF = Measurements::clamp(wheelSpeeds[LEFT_FRONT], -100.0, 100.0);
    double LB = Measurements::clamp(wheelSpeeds[LEFT_BACK], -100.0, 100.0);
    double RB = Measurements::clamp(wheelSpeeds[RIGHT_BACK], -100.0, 100.0);
    double RF = Measurements::clamp(wheelSpeeds[RIGHT_FRONT], -100.0, 100.0);

    if(LF != 0 || LB != 0 || RB != 0 || RF!= 0 || r->getID() ==3){
        auto rv = r->getVelocityMetersPerSecond();
        std::cout<<"Moving at: "<< rv.x*1000 << " " <<rv.y*1000 << " " <<std::endl;
        std::cout<<"R: " << r->getTeamId() << " " << r->getID() <<std::endl;
        std::cout<< r->getPilot()->getVel().x << " " << r->getPilot()->getVel().y<< " " << r->getPilot()->getAngular() <<std::endl;
        std::cout<<"Sending: " << LF << " " << LB << " " << RB << " " << RF << std::endl;
    }

    float kick = robot->getKick();
    bool  dribble = robot->getDribble();

    // Fill in simulator packet
    command->set_id(id);
    command->set_wheelsspeed(true);
//    command->set_wheel1( 0 * mappingRatio);    //Left Forward
//    command->set_wheel2( 0 * mappingRatio);    //Left Backward
//    command->set_wheel3( 0 * mappingRatio);    //Right Backward
//    command->set_wheel4( 0 * mappingRatio);    //Right Forward

    command->set_wheel1( LF );    //Left Forward
    command->set_wheel2( LB );    //Left Backward
    command->set_wheel3( RB );    //Right Backward
    command->set_wheel4( RF );    //Right Forward
    auto v = r->getPilot()->getVel();
    auto w = r->getPilot()->getAngular();
    command->set_veltangent(v.x);
    command->set_velnormal(v.y);  // No normal velocity
    command->set_velangular(w);
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
