#define _USE_MATH_DEFINES
#include <cmath>
#include "robot_grsim.h"
#include "robot/navigation/pilots/pilot_omni.h"
#include <iostream>

// dimensions of grsim robot define in grsim project in file srd/configwidget (search for robot_settings)
#define WHEEL_RADIUS 32.5        // 32.5mm
#define ROBOT_RADIUS 90          // 90 mm
#define LF_OFFSET ( 60*M_PI/180) // 60 degrees
#define LB_OFFSET (135*M_PI/180) // 135 degrees
#define RB_OFFSET (225*M_PI/180) // 225 degrees
#define RF_OFFSET (300*M_PI/180) // 300 degrees




RobotGrsim::RobotGrsim(int id, int team, RobotRole role) :
    Robot(id,team,role),
    drive(OmniDrive(ROBOT_RADIUS, WHEEL_RADIUS,
                    LF_OFFSET, LB_OFFSET, RB_OFFSET, RF_OFFSET)),
    pilot(PilotOmni(this,10.0/7, 0, 0.5, 0.0015))
{

}


bool  RobotGrsim::hasKicker() {
    return true;
}
Pilot* RobotGrsim::getPilot() {
    return &pilot;
}

void RobotGrsim::getWheelSpeeds(double wheelSpeeds[4]){
    auto v = pilot.getVel();
    float w = pilot.getAngular();
    auto res = drive.getWheelSpeeds(v.x,v.y,w);
//    if(v.x != 0 || v.y != 0 || w) {
//        std::cout << "(" << v.x <<  "," << v.y << "," << w << "):\n" << res << std::endl;
//        std::cout << res.rows() << " " << res.cols() << std::endl;
//    }
    for(int i=0; i<4; i++)
        wheelSpeeds[i] = res[i];
}
