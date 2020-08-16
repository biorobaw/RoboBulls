#include "gui_teams.h"
#include "model/team.h"
#include "robot/robot.h"
#include "model/configuration.h"


GuiTeams* GuiTeams::instance = new GuiTeams;

namespace  {
    Robot team_proxy[2] = {Robot(0,0), Robot(1,0)};
}

GuiTeams::GuiTeams(){

}


QString GuiTeams::getRobotType(int team_id){

    return instance->robot_type[team_id];
}

QString GuiTeams::getController(int team_id){
    return instance->controller[team_id];
}

bool GuiTeams::isControlled(int team_id){
    return instance->controller[team_id] != "none";
}

QString GuiTeams::getStrategy(int team_id){
    return instance->strategy[team_id];
}




void GuiTeams::updateRobotType(int team, QString type){
    robot_type[team] = type;
}
void GuiTeams::updateController(int team, QString c){
    controller[team] = c;
}
void GuiTeams::updateStrategy(int team, QString s){
    strategy[team]   = s;
}

void GuiTeams::initData(){
    auto& c = *Configuration::get();
    auto team_b = c["teams"]["TEAM_BLUE"];
    auto team_y = c["teams"]["TEAM_YELLOW"];
    instance->robot_type[ROBOT_TEAM_BLUE]   = team_b["ROBOT_PROXY"]["TYPE"].Scalar().c_str();
    instance->robot_type[ROBOT_TEAM_YELLOW] = team_y["ROBOT_PROXY"]["TYPE"].Scalar().c_str();

    instance->controller[ROBOT_TEAM_BLUE]   = team_b["STRATEGY_CONTROLLER"]["ID"].Scalar().c_str();
    instance->controller[ROBOT_TEAM_YELLOW] = team_y["STRATEGY_CONTROLLER"]["ID"].Scalar().c_str();

    for(int i=0; i<2; i++){
        auto t = team_proxy->getTeam();
        connect(t, &RobotTeam::strategyChanged, instance, &GuiTeams::updateStrategy);
    }


}
