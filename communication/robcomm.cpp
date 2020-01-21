#include "simrobcomm.h"
#include "nxtrobcomm.h"
#include "yisirobcomm.h"
#include "robcomm.h"


RobComm * RobComm::robcomm = NULL;


void RobComm::open_communication(YAML::Node team_node){

    if (robcomm == NULL){

        std::cout << "--ROB_COMM " << std::endl
                  << "        ROBOT_TYPE : " << team_node["ROBOT_TYPE"] << std::endl;
        auto robot_type = team_node["ROBOT_TYPE"].as<std::string>();
        if( robot_type == "grsim"){
            std::cout << "        GRSIM_ADDR : " <<  team_node["GRSIM_ADDR"] << std::endl
                      << "        GRSIM_PORT : " << team_node["GRSIM_PORT"] << std::endl;

            std::string ip = team_node["GRSIM_ADDR"].as<std::string>();
            int port = team_node["GRSIM_PORT"].as<int>();

            robcomm = new SimRobComm(ip.c_str(),port);

        } else if (robot_type == "yisibot"){

            std::cout << "        YISI_USB_PORT  : " <<  team_node["YISI_USB_PORT"] << std::endl
                      << "        YISI_FREQUENCY : " <<  team_node["YISI_FREQUENCY"] << std::endl;

            std::string usb_port = team_node["YISI_USB_PORT"].as<std::string>();
            int frequency = team_node["YISI_FREQUENCY"].as<int>();

            robcomm = new YisiRobComm(usb_port,frequency);

        } else if (robot_type == "rpi_2019"){
            std::cout << "ERROR: rpi_2019 is not yet supported" << std::endl;
            exit (-1);
            //robcomm = new YisiRobComm();

        } else {
            std::cout << "ERROR: unrecognized robot type" << std::endl;
            exit (-1);
        }

    }

    std::cout << "--Robcomm DONE" << std::endl;

}


void RobComm::close_communication(std::set<Robot*>& robots){
    if(robcomm != NULL){

        for(Robot* rob : robots) {
            rob->setLF(0); rob->setLB(0);
            rob->setRF(0); rob->setRB(0);
            rob->setB(0);
            rob->setDribble(0);
            rob->setKick(0);
        }
        RobComm::sendVels(robots);

        robcomm->close();
        delete robcomm;
        robcomm = NULL;

    }
}


void RobComm::sendVels(std::set<Robot*>& robots){
    if(robcomm == NULL){
        std::cout << "WARNING: robcomm closed, can't sent vels" << std::endl;
    } else {
        robcomm->sendVelsLarge(robots);
    }
}
