#include "robots/grsim/robcomm_grsim.h"
#include "robots/yisibot/robcomm_yisibot.h"
#include "robcomm.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"

RobComm::~RobComm(){
}

void RobComm::close_communication(std::set<Robot*>& robots){

        for(Robot* rob : robots) {
            rob->getPilot()->setManualVelocity(Point(0,0),0);
            rob->setDribble(0);
            rob->setKick(0);
        }
        sendVels(robots);
        close();
}


RobComm* RobComm::loadRobComm(std::string robot_type,YAML::Node* comm_node){
    // set robot communication:

    if( robot_type == "grsim"){
        return new RobCommGrsim(comm_node);
    } else if (robot_type == "yisibot"){
        return new YisiRobComm(comm_node);
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
