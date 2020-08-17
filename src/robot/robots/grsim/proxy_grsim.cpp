#include "proxy_grsim.h"


#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"

#include "robot/robot.h"
#include "robot/navigation/pilots/pilot_omni.h"
#include "model/team.h"
#include "model/game_state.h"
#include "utilities/measurements.h"
#include "utilities/my_yaml.h"

#include <math.h>


// dimensions of grsim robot define in grsim project in file srd/configwidget (search for robot_settings)
#define WHEEL_RADIUS 32.5        // 32.5mm
#define ROBOT_RADIUS 90          // 90 mm
#define LF_OFFSET ( 60*M_PI/180) // 60 degrees
#define LB_OFFSET (135*M_PI/180) // 135 degrees
#define RB_OFFSET (225*M_PI/180) // 225 degrees
#define RF_OFFSET (300*M_PI/180) // 300 degrees



ProxyGrsim::ProxyGrsim(YAML::Node* t_node) :
    drive(ROBOT_RADIUS, WHEEL_RADIUS, LF_OFFSET, LB_OFFSET, RB_OFFSET, RF_OFFSET)
{

    qInfo() << "            ADDR          -" <<  (*t_node)["ADDR"];
    qInfo() << "            PORT          -" <<  (*t_node)["PORT"] ;

    QString ip = (*t_node)["ADDR"].Scalar().c_str();
    int port = (*t_node)["PORT"].as<int>();

    _addr = QHostAddress(ip);
    _port = port;
}

void ProxyGrsim::sendVels(const QSet<Robot*>& robots)
{
    //Send standard robot packets
    for (Robot* rob : robots)
    {
        sendPacket(rob);
        rob->setKickSpeed(0);
        //rob->setDrible(0);
    }

    //Send replacement packets
    sendReplacementPackets();
}

void ProxyGrsim::sendPacket(Robot* r)
{

    // Retrive robot information
    int id = r->getId();
    auto controls = r->getActiveController();
    float kick = controls->getKickSpeed();
    bool  dribble = controls->getDribble();
    auto v = controls->getTargetVelocity(); // in what value? m/s? mm/s? we assume mm/s
    auto w = controls->getTargetAngularSpeed(); // we assume rad/s
    int flip_x = r->getFlipXCoordinates();
    v.x*=flip_x;
    w*=flip_x;

    // prepare command package
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow( r->getTeamId() == ROBOT_TEAM_YELLOW );
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
    command->set_id(id);
    command->set_kickspeedx(kick); // is the unit correct? robobulls is mm/s
    command->set_kickspeedz(0); // No chipper
    command->set_spinner(dribble ? 300 : 0);
    command->set_veltangent(v.x / 1000.0); // set_veltangent expect m/s units
    command->set_velnormal(v.y / 1000.0);  // set_velnormal expect m/s units
    command->set_velangular(w); // set vel angular expects rad/s

    // Fill in simulator packet
    command->set_wheelsspeed(false); // change if you want to control each wheel
//    command->set_wheel1( 0 );    //Left Forward
//    command->set_wheel2( 0 );    //Left Backward
//    command->set_wheel3( 0 );    //Right Backward
//    command->set_wheel4( 0 );    //Right Forward



    //    if(v.x!=0 || v.y !=0 || w!=0 || r->getID() ==3){
    //        auto rv = r->getVelocityMetersPerSecond();
    //        std::cout<<"Moving at: "<< rv.x << " " <<rv.y << " " <<std::endl;
    //        std::cout<< "Sending: " << id << "," << r->getTeamId() << " "
    //                 << v.x/1000 << "," << v.y/1000 << "," << w <<std::endl;
    //    }


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
//        std::cout<< r->getTargetVelocity().x << " " << r->getTargetVelocity().y<< " " << r->getTargetAngularSpeed() <<std::endl;
//        std::cout<<"Sending: " << LF << " " << LB << " " << RB << " " << RF << std::endl;
//    }


    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSizeLong());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket->writeDatagram(dgram, _addr, _port);

}

void ProxyGrsim::sendReplacementPackets()
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
    udpsocket->writeDatagram(dgram, _addr, _port);
}

void ProxyGrsim::close(){
    // udp socket seems to close automatically if necessary when destroyed
    // nothing to be done
}


bool  ProxyGrsim::hasKicker() {
    return true;
}
bool  ProxyGrsim::isHolonomic() {
    return true;
}
Pilot* ProxyGrsim::createPilot(Robot* robot) {
    return new PilotOmni(robot,10.0/7, 0, 0.5, 0.0015);
}

void ProxyGrsim::getWheelSpeeds(Point velocity, float angular_speed, double wheelSpeeds[4]){
    auto res = drive.getWheelSpeeds(velocity.x, velocity.y, angular_speed);
    for(int i=0; i<4; i++)
        wheelSpeeds[i] = res[i];
}

QString ProxyGrsim::getName(){
    return "grsim";
}
