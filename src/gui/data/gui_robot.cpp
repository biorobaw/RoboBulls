#define _USE_MATH_DEFINES
#include "gui_robot.h"
#include "model/team.h"
#include "utilities/measurements.h"

GuiRobot GuiRobot::proxies[2][MAX_ROBOTS_PER_TEAM];


GuiRobot::GuiRobot(){}

#define FOR_TEAM_ROBOT for(int team=0 ; team<2; team++) \
    for(int robot=0; robot<MAX_ROBOTS_PER_TEAM; robot++)

void GuiRobot::init_static_data(){
    FOR_TEAM_ROBOT {
        proxies[team][robot].initProxy(team,robot);
    }
}

void GuiRobot::initProxy(int team, int id)
{
    this->id = id;
    this->team = team;

}

void GuiRobot::updateRobots(){
    //update each robot
    FOR_TEAM_ROBOT {
        auto proxy = RobotTeam::getTeam(team)->getRobot(robot);
        auto& r = proxies[team][robot];

        r.is_in_field = proxy != nullptr;

        if(r.is_in_field){
            r.previous_pos = r.current_pos;
            r.previous_speed = r.current_speed;

            // get all info from proxy, all this info should have mutexes
            r.current_orientation = proxy->getOrientation();
            r.velocity_command = proxy->getPilot()->getVel();
            r.current_pos = proxy->getPosition();
            r.dribling = proxy->getDribble();
            r.kicking  = proxy->getKick() == 1;
            r.behaviorName = proxy->hasBehavior() ?
                        proxy->getBehavior()->getName() : "No Behavior";


            // do computations with values
            r.current_orientation *= 180/M_PI; // convert to degrees
            r.current_speed = Measurements::distance(r.current_pos,r.previous_pos);
            r.speed_command = (int)Measurements::mag(r.velocity_command);
        }

    }

}

bool GuiRobot::hasProxy(){
    return is_in_field;
}

Point GuiRobot::getCurrentPosition(){
    return current_pos;
}
Point GuiRobot::getVelocityCommand(){
    return velocity_command;
}
int GuiRobot::getSpeedCommand(){
    return speed_command;
}
float GuiRobot::getOrientation(){
    return current_orientation;
}

int GuiRobot::getCurrentSpeed() {
    return current_speed;
}


QString GuiRobot::getOrientationAsString(){
    std::string sOrient = std::to_string(int(current_orientation));
    return QString::fromStdString(sOrient);
}

string GuiRobot::getBehaviorName() {
    return behaviorName;
}


void  GuiRobot::setManualVelocity(Point vxy, float angular){
    auto proxy = RobotTeam::getTeam(team)->getRobot(id);
    if(proxy!=nullptr) proxy->getPilot()->setManualVelocity(vxy,angular);
}


void  GuiRobot::setKick(float power){
    auto proxy = RobotTeam::getTeam(team)->getRobot(id);
    if(proxy!=nullptr) {
        kicking = true;
        proxy->setKick(power);
    }
}
void  GuiRobot::setDribble(bool dribble){
    auto proxy = RobotTeam::getTeam(team)->getRobot(id);
    if(proxy!=nullptr) {
        dribling = dribble;
        proxy->setDribble(dribble);
    }

}

bool GuiRobot::isDribbling(){
    return dribling;
}
bool GuiRobot::isKicking(){
    return kicking;
}
