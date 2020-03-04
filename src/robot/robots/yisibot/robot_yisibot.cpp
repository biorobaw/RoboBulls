#include "robot_yisibot.h"
#include "robot/navigation/pilots/pilot_omni.h"



RobotYisibot::RobotYisibot(int id, int team, RobotRole role) :
    Robot(id,team,role),
    pilot(PilotOmni(this,0.25,0,0.1,0))
{

    // Robot Physical Properties
//    static constexpr double LF_OFFSET = 144*M_PI/180; //135 (Robot's x-Axis/right side is zero)
//    static constexpr double LB_OFFSET = 224*M_PI/180; //225
//    static constexpr double RF_OFFSET =  36*M_PI/180; //45
//    static constexpr double RB_OFFSET = 316*M_PI/180; //315
//    const double TRANS_OFFSET = 0.0149;
//    const double WHEEL_RADIUS = 27;

}


bool  RobotYisibot::hasKicker() {
    return true;
}
Pilot* RobotYisibot::getPilot() {
    return &pilot;
}

