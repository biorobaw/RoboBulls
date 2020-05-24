#define _USE_MATH_DEFINES
#include "gui_robot.h"
#include "model/team.h"
#include "utilities/measurements.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
#include "gui/main_window.h"
#include "robot/navigation/path_planning/move_collisions.h"
#include "model/ball.h"



// Static variables
GuiRobot*** GuiRobot::robots = GuiRobot::init_robots();
GuiRobot*   GuiRobot::selected_robot = nullptr;


// Private static function to initialize robots variable statically
GuiRobot*** GuiRobot::init_robots(){
    GuiRobot*** robots = new GuiRobot**[2];
    for(int i=0;i<2;i++){
        robots[i] = new GuiRobot*[MAX_ROBOTS_PER_TEAM];
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            robots[i][j] = new GuiRobot(nullptr,i,j);
    }
    return robots;
};

// Static method to get pointer to robot:
GuiRobot* GuiRobot::get(int team, int robot){
    return robots[team][robot];
}


// Private constructor
GuiRobot::GuiRobot(QObject* parent,int team, int robot_id) :
    QObject(parent), id(robot_id), team(team){

}

// Macro for eac
#define FOR_TEAM_ROBOT for(int team=0 ; team<2; team++) \
    for(int robot=0; robot<MAX_ROBOTS_PER_TEAM; robot++)


// function to update robot data from the game
void GuiRobot::updateRobots(){
    //update each robot
    for(int team=0 ; team<2; team++){
        for(int robot=0; robot<MAX_ROBOTS_PER_TEAM; robot++) {
            auto proxy = RobotTeam::getTeam(team)->getRobot(robot);
            auto r = robots[team][robot];

            r->is_in_field = proxy != nullptr;

            if(r->is_in_field){
                r->is_controlled = proxy->getTeam()->isControlled();
                r->has_ball = proxy->hasBall();
                r->move_status = Collisions::getMoveStatus(proxy);
                r->previous_pos = r->current_pos;
                r->previous_speed = r->current_speed;

                // get all info from proxy, all this info should have mutexes
                r->current_orientation = proxy->getOrientation();
                r->velocity_command = proxy->getPilot()->getVel();
                r->current_pos = proxy->getPosition();
                r->dribling = proxy->getDribble();
                r->kicking  = proxy->getKick() == 1;
                r->behaviorName = proxy->hasBehavior() ?
                            proxy->getBehavior()->getName().c_str() : "No Behavior";


                // do computations with values
                r->current_orientation *= 180/M_PI; // convert to degrees
                r->current_speed = Measurements::distance(r->current_pos,r->previous_pos);
                r->speed_command = (int)Measurements::mag(r->velocity_command);
            }

        }
    }

}


bool GuiRobot::isInField(){
    return is_in_field;
}

bool GuiRobot::isControlled(){
    return is_controlled;
}

bool GuiRobot::hasBall(){
    return has_ball;
}

int GuiRobot::getMoveStatus(){
    return move_status;
}

bool GuiRobot::selected(){
    return this == selected_robot;
}

void GuiRobot::select(){
    if(isInField()){
        if(selected_robot!=this){
            clearSelected();
            selected_robot = this;
            emit this->selectedChanged(this);
        }
    }
}

void GuiRobot::clearSelected(){
    if(selected_robot!=nullptr){
        selected_robot->setManualVelocity(Point(0,0),0);
        selected_robot->setDribble(0);
        selected_robot->setKick(0);
        auto aux = selected_robot;
        selected_robot = nullptr;
        emit aux->selectedChanged(aux);
    }
}

bool GuiRobot::isOverriden(){
    return overriden;
}

void GuiRobot::setOverriden(bool new_value){
    if(new_value!=overriden){
        overriden = new_value;
        if(overriden){
            setManualVelocity(Point(0,0),0);
            setDribble(0);
            setKick(0);
        }
        emit overridenChanged(new_value);
    }
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

QString GuiRobot::getBehaviorName() {
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

GuiRobot* GuiRobot::get_selected_robot(){
    return selected_robot;
}
