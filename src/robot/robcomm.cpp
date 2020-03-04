#include "robots/grsim/robcomm_grsim.h"
#include "robots/yisibot/robcomm_yisibot.h"
#include "robcomm.h"

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
