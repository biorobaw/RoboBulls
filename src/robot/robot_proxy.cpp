#include "robots/grsim/proxy_grsim.h"
#include "robots/yisibot/proxy_yisibot.h"
#include "robots/none/proxy_none.h"
#include "robot_proxy.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
#include <QDebug>
#include "utilities/my_yaml.h"

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


RobotProxy* RobotProxy::load(YAML::Node* proxy_node){
    // set robot communication:

    qInfo() << "        ROBOT_PROXY";
    qInfo() << "            TYPE          -" << (*proxy_node)["TYPE"];

    auto robot_type = (*proxy_node)["TYPE"].as<string>();

    if( robot_type == "grsim"){
        return new ProxyGrsim(proxy_node);

    } else if (robot_type == "yisibot"){
        return new ProxyYisi(proxy_node);

    } else if (robot_type == "rpi_2019"){
        qCritical() << "ERROR: rpi_2019 is not yet supported" ;
        exit (-1);

    } else if (robot_type == "none"){
        return new ProxyNone();
    } else {
        qCritical() << "ERROR: unrecognized robot type";
        exit (-1);
    }
}
