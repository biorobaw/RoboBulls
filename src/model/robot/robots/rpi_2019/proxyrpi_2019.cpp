#include "proxyrpi_2019.h"
#include "utilities/my_yaml.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/pilots/pilot_differential.h"
#include <map>
#include <utility>

#include "pirobot2019.pb.h"
//#include





ProxyRpi_2019::ProxyRpi_2019(YAML::Node* t_node)
{


   //AML::Node* robots = (*t_node)["ROBOT"];

    for (const auto& robots : (*t_node)["ROBOT"])
    {
       //int id = stoi(robots);
        int id = robots.first.as<int>();
        ROBOT_ADDRS.insert(std::make_pair(id, std::make_pair(QHostAddress(robots.second["ADDR"].Scalar().c_str()), robots.second["PORT"].as<int>())));
        qInfo()<< "Robot Map: " << ROBOT_ADDRS[id].first << ROBOT_ADDRS[id].second;
    }




}


//NOT IMPLEMENTED
void ProxyRpi_2019::sendCommands(const QSet<Robot*>& robots)
{

    //Send standard robot packets
    for (Robot* rob : robots)
    {
        sendPacket(rob);
        rob->setKickSpeed(0);
        //rob->setDrible(0);
    }

    //Send replacement packets
    //sendReplacementPackets();
}


//NOT IMPLEMENTED
void ProxyRpi_2019::sendPacket(Robot* r)
{

    // Retrive robot information
    int id        = r->getId();
    auto controls = r->getActiveController();
    auto v        = controls->getTargetVelocity(); // in what value? m/s? mm/s? we assume mm/s
    auto w        = controls->getTargetAngularSpeed(); // we assume rad/s
    float kick    = controls->getKickSpeed();
    bool  dribble = controls->getDribble();


//    // prepare command package
    pirobot2019::VelocityXYZ packet;
    packet.set_x(v.x);
    packet.set_z(w);

    //Send packet
    QByteArray dgram;
    dgram.resize(packet.ByteSizeLong());
    packet.SerializeToArray(dgram.data(), dgram.size());
    string temp = packet.SerializeAsString();
    string temp2;
    for(int i = 0; i<temp.length(); i++){
        int itemp = temp[i];
        temp2+= std::to_string(itemp);
    }
    std::cout << "temp: "<<std::endl;
    std::cout << temp2<<std::endl;
    dgram.push_front(char(0));
    qInfo() << dgram;
    std::pair<QHostAddress, int> add_port = ROBOT_ADDRS[id];
    qInfo() << "Address: " <<add_port.first << "\nPort: " << add_port.second << "\n X, W: " << v.x << " "<<w;
    udpsocket->writeDatagram(dgram, add_port.first, add_port.second);

//    packet.mutable_commands()->set_isteamyellow( r->getTeamId() == ROBOT_TEAM_YELLOW );
//    packet.mutable_commands()->set_timestamp(0.0);
//    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
//    command->set_id(id);
//    command->set_kickspeedx((float)kick/1000); // convert mm/s to m/s
//    command->set_kickspeedz(0); // No chipper
//    command->set_spinner(dribble ? 300 : 0);
//    command->set_veltangent(v.x / 1000.0); // set_veltangent expect m/s units
//    command->set_velnormal(v.y / 1000.0);  // set_velnormal expect m/s units
//    command->set_velangular(w); // set vel angular expects rad/s

//    // Fill in simulator packet
//    command->set_wheelsspeed(false); // change if you want to control each wheel

//    //Send packet
//    QByteArray dgram;
//    dgram.resize(packet.ByteSizeLong());
//    packet.SerializeToArray(dgram.data(), dgram.size());
//    udpsocket->writeDatagram(dgram, _addr, _port);

}

//NOT IMPLEMENTED
//void ProxyRpi_2019::sendReplacementPackets()
//{
//    grSim_Packet packet;
//    grSim_Replacement* replacement = packet.mutable_replacement();
//    (void)replacement;

//    //Send packet
//    QByteArray dgram;
//    dgram.resize(packet.ByteSizeLong());
//    packet.SerializeToArray(dgram.data(), dgram.size());
//    udpsocket->writeDatagram(dgram, _addr, _port);
//}


//NOT IMPLEMENTED
void ProxyRpi_2019::close(){
    // udp socket seems to close automatically if necessary when destroyed
    // nothing to be done
}

//NOT IMPLEMENTED
bool  ProxyRpi_2019::hasKicker() {
    return false;
}

//NOT IMPLEMENTED
bool  ProxyRpi_2019::isHolonomic() {
    return false;
}

//NOT IMPLEMENTED

RobotPilot* ProxyRpi_2019::createPilot(Robot* robot) {
    return new PilotDifferential(robot);
}

//NOT IMPLEMENTED
//void ProxyRpi_2019::getWheelSpeeds(Point velocity, float angular_speed, double wheelSpeeds[4]){
//    auto res = drive.getWheelSpeeds(velocity.x, velocity.y, angular_speed);
//    for(int i=0; i<4; i++)
//        wheelSpeeds[i] = res[i];
//}

QString ProxyRpi_2019::getName(){
    return "rpi_2019";
}

