#include "robotnone.h"

RobotNone::RobotNone(int _id, int _team, RobotRole role) :
    Robot(_id,_team,role)
{

}

bool RobotNone::hasKicker(){
    return false;
}
Pilot* RobotNone::getPilot(){
    return &dummyPilot;
}
