#include <math.h>
#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"
#include "robot/robot.h"
#include "model/team.h"
#include "model/game_state.h"
#include "robcomm_grsim.h"
#include "robot_grsim.h"
#include <iostream>
#include "utilities/measurements.h"

RobCommGrsim::RobCommGrsim(YAML::Node t_node)
{

    std::cout << "        GRSIM_ADDR : " <<  t_node["GRSIM_ADDR"] << std::endl
              << "        GRSIM_PORT : " << t_node["GRSIM_PORT"] << std::endl;

    std::string ip = t_node["GRSIM_ADDR"].as<std::string>();
    int port = t_node["GRSIM_PORT"].as<int>();

    _addr = QHostAddress(ip.c_str());
    _port = port;
}

void RobCommGrsim::sendVels(std::set<Robot*>& robots)
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

void RobCommGrsim::sendPacket(Robot* robot)
{
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow( robot->getTeamId() == TEAM_YELLOW );
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();

    //Retrive robot information
    RobotGrsim* r = (RobotGrsim*)robot;
    int id = robot->getID();
    float kick = robot->getKick();
    bool  dribble = robot->getDribble();
    command->set_id(id);
    command->set_kickspeedx(kick);
    command->set_kickspeedz(0); // No chipper
    command->set_spinner(dribble ? 300 : 0);


    auto v = r->getPilot()->getVel(); // in what value? m/s? mm/s? we assume mm/s
    auto w = r->getPilot()->getAngular(); // we assume rad/s
    command->set_veltangent(v.x / 1000.0); // set_veltangent expect m/s units
    command->set_velnormal(v.y / 1000.0);  // set_velnormal expect m/s units
    command->set_velangular(w); // set vel angular expects rad/s


//    if(v.x!=0 || v.y !=0 || w!=0 || r->getID() ==3){
//        auto rv = r->getVelocityMetersPerSecond();
//        std::cout<<"Moving at: "<< rv.x << " " <<rv.y << " " <<std::endl;
//        std::cout<< "Sending: " << id << "," << r->getTeamId() << " "
//                 << v.x/1000 << "," << v.y/1000 << "," << w <<std::endl;
//    }

    // Fill in simulator packet

    command->set_wheelsspeed(false);
    command->set_wheel1( 0 );    //Left Forward
    command->set_wheel2( 0 );    //Left Backward
    command->set_wheel3( 0 );    //Right Backward
    command->set_wheel4( 0 );    //Right Forward


//    double wheelSpeeds[4];
//    r->getWheelSpeeds(wheelSpeeds);
//    double LF = Measurements::clamp(wheelSpeeds[LEFT_FRONT], -100.0, 100.0);
//    double LB = Measurements::clamp(wheelSpeeds[LEFT_BACK], -100.0, 100.0);
//    double RB = Measurements::clamp(wheelSpeeds[RIGHT_BACK], -100.0, 100.0);
//    double RF = Measurements::clamp(wheelSpeeds[RIGHT_FRONT], -100.0, 100.0);
//    command->set_wheel1( LF );    //Left Forward
//    command->set_wheel2( LB );    //Left Backward
//    command->set_wheel3( RB );    //Right Backward
//    command->set_wheel4( RF );    //Right Forward
//    if(LF != 0 || LB != 0 || RB != 0 || RF!= 0 || r->getID() ==3){
//        auto rv = r->getVelocityMetersPerSecond();
//        std::cout<<"Moving at: "<< rv.x*1000 << " " <<rv.y*1000 << " " <<std::endl;
//        std::cout<<"R: " << r->getTeamId() << " " << r->getID() <<std::endl;
//        std::cout<< r->getPilot()->getVel().x << " " << r->getPilot()->getVel().y<< " " << r->getPilot()->getAngular() <<std::endl;
//        std::cout<<"Sending: " << LF << " " << LB << " " << RB << " " << RF << std::endl;
//    }


    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSizeLong());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);





}

void RobCommGrsim::sendReplacementPackets()
{
    grSim_Packet packet;
    grSim_Replacement* replacement = packet.mutable_replacement();
    (void)replacement;

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
    dgram.resize(packet.ByteSizeLong());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}

void RobCommGrsim::close(){
    // udp socket seems to close automatically if necessary when destroyed
    // nothing to be done
}
