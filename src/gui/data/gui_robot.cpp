#define _USE_MATH_DEFINES
#include "gui_robot.h"
#include "model/team.h"
#include "utilities/measurements.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
#include "gui/main_window.h"
#include "robot/navigation/path_planning/move_collisions.h"
#include "model/ball.h"
#include "robot/navigation/robot_pilot.h"
#include "gui_game_state.h"
#include "gui_teams.h"

// Static variables
GuiRobot*   GuiRobot::selected_robot = nullptr;

GuiRobot* GuiRobot::get(int team_id, int robot_id){
    return  GuiGameState::getRobot(team_id, robot_id);
}


// Private constructor
GuiRobot::GuiRobot(QObject* parent, int id, int team) :
    QObject(parent), Robot(id, team)
{

}

void GuiRobot::update(){
    if(!isInField()) return;

    auto proxy = getProxy();
    setTargetVelocity(proxy->getTargetVelocity(),
                      proxy->getTargetAngularSpeed() );

    dribble = proxy->getDribble();
    chip = proxy->getChip();

    behaviorName = proxy->hasBehavior() ?
                proxy->getBehavior()->getName().c_str() : "No Behavior";

    move_status = Collisions::getMoveStatus(proxy);

}


int GuiRobot::getMoveStatus(){
    return move_status;
}


QString GuiRobot::getBehaviorName(){
    return behaviorName;
}

bool GuiRobot::isControlled(){
    return GuiTeams::is_controlled(team_id);
}

float GuiRobot::getOrientationInDegrees(){
    return orientation*180/M_PI;
}


QString GuiRobot::getOrientationAsString(){
    std::string sOrient = std::to_string(int(orientation*180/M_PI));
    return QString::fromStdString(sOrient);
}









void GuiRobot::setOverriden(bool new_value){
    if(new_value!=overriden){
        overriden = new_value;
        if(overriden){
            setGuiTargetVelocity(Point(0,0),0);
            setGuiDribble(0);
            setGuiKickSpeed(0);
        }
        emit overridenChanged(new_value);
    }
}

void  GuiRobot::setGuiTargetVelocity(Point vxy, float angular){
    gui_target_velocity = vxy;
    gui_target_angular_speed = angular;
//    auto proxy = RobotTeam::getTeam(team)->getRobot(id);
//    if(proxy!=nullptr) proxy->setTargetVelocity(vxy,angular);
}


void  GuiRobot::setGuiKickSpeed(int speed){
    gui_kick_speed = speed;
}

void  GuiRobot::setGuiDribble(bool dribble){
    gui_dribble = dribble;

}










bool GuiRobot::isOverriden(){
    return overriden;
}

Point GuiRobot::getGuiTargetVelocity(){
    return gui_target_velocity;
}
int GuiRobot::getGuiTargetSpeed(){
    return gui_target_velocity.norm();
}

float GuiRobot::getGuiTargetAngularSpeed(){
    return gui_target_angular_speed;
}
int   GuiRobot::getGuiKickSpeed(){
    return gui_kick_speed;
}
bool  GuiRobot::getGuiDribble(){
    return gui_dribble;
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

bool GuiRobot::selected(){
    return this == selected_robot;
}

void GuiRobot::clearSelected(){
    if(selected_robot!=nullptr){
        selected_robot->setGuiTargetVelocity(Point(0,0),0);
        selected_robot->setGuiDribble(0);
        selected_robot->setGuiKickSpeed(0);
        auto aux = selected_robot;
        selected_robot = nullptr;
        emit aux->selectedChanged(aux);
    }
}

GuiRobot* GuiRobot::get_selected_robot(){
    return selected_robot;
}




Robot* GuiRobot::getProxy(){
    return  getTeam()->getRobot(id);
}
