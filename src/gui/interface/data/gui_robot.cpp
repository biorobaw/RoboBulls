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

void GuiRobot::connectWithModel(){
    for(int i=0; i<2; i++)
        for(int j=0; j <MAX_ROBOTS_PER_TEAM; j++){
            auto gui_robot = get(i,j);
            auto proxy = gui_robot->getProxy();
            auto controls = proxy->getOverridenController();
            QObject::connect(gui_robot, &GuiRobot::overridenChanged     ,proxy, &Robot::useOverridenControls);
            QObject::connect(gui_robot, &GuiRobot::setGuiTargetVelocity ,controls, &RobotLowLevelControls::setTargetVelocity  );
            QObject::connect(gui_robot, &GuiRobot::setGuiKickSpeed      ,controls, &RobotLowLevelControls::setKickSpeed       );
            QObject::connect(gui_robot, &GuiRobot::setGuiDribble        ,controls, &RobotLowLevelControls::setDribble         );
            QObject::connect(gui_robot, &GuiRobot::setGuiChip           ,controls, &RobotLowLevelControls::setChip            );

        }
}


// Private constructor
GuiRobot::GuiRobot(QObject* parent, int team, int id) :
    Robot(team, id)
{
    setParent(parent);
}

void GuiRobot::update(){
    if(!isInField()) return;

    // We need to update all information other than the information provided by ssl vision
    // In other words, control information
    auto proxy = getProxy();
    auto controls = proxy->getActiveController();

    // undo flip : see "Field side hypothesis" in documentation for details
    // in summary, teams assume their side is the negative field side
    // so we need to correct for this
    flip_x = proxy->getFlipXCoordinates();

    auto v = controls->getTargetVelocity();
    v.x*=flip_x;
    setTargetVelocity(controls->getTargetVelocity(),
                      controls->getTargetAngularSpeed() * flip_x);


    dribble = controls->getDribble();
    chip = controls->getChip();

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
    return GuiTeams::isControlled(team_id);
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
            emit setGuiTargetVelocity(Point(0,0),0);
            emit setGuiDribble(0);
            emit setGuiKickSpeed(0);
        }
        emit overridenChanged(new_value);
    }
}




bool GuiRobot::isOverriden(){
    return overriden;
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
    if(selected_robot!=nullptr && selected_robot->isOverriden()){
        emit selected_robot->setGuiTargetVelocity(Point(0,0),0);
        emit selected_robot->setGuiDribble(0);
        emit selected_robot->setGuiKickSpeed(0);
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

int GuiRobot::getFlipXCoordinates(){
    return flip_x;
}
