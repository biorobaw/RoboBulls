#include "robots/grsim/proxy_grsim.h"
#include "robots/yisibot/proxy_yisibot.h"
#include "robot_proxy.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"

RobotProxy::~RobotProxy(){
}

void RobotProxy::close_communication(const QSet<Robot*>& robots){

        for(Robot* rob : robots) {
            rob->setTargetVelocity(Point(0,0),0);
            rob->setDribble(0);
            rob->setKickSpeed(0);
        }
        sendVels(robots);
        close();
}


RobotProxy* RobotProxy::load(QString robot_type,YAML::Node* comm_node){
    // set robot communication:

    if( robot_type == "grsim"){
        return new ProxyGrsim(comm_node);

    } else if (robot_type == "yisibot"){
        return new ProxyYisi(comm_node);

    } else if (robot_type == "rpi_2019"){
        std::cout << "ERROR: rpi_2019 is not yet supported" << std::endl;
        exit (-1);

    } else if (robot_type == "none"){
        return nullptr;

    } else {
        std::cout << "ERROR: unrecognized robot type" << std::endl;
        exit (-1);
    }
}
