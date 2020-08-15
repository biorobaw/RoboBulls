#include "gui_teams.h"
#include "model/team.h"
#include "robot/robot.h"
#include "model/configuration.h"

QString GuiTeams::robot_type[2] = {"none"};
QString GuiTeams::controller[2] = {"none"};
QString GuiTeams::strategy[2]   = {"none"};

namespace  {
    Robot team_proxy[2] = {Robot(0,0), Robot(1,0)};
}

QString GuiTeams::getRobotType(int team_id){

    return robot_type[team_id];
}

QString GuiTeams::getController(int team_id){
    return controller[team_id];
}

bool GuiTeams::isControlled(int team_id){
    return controller[team_id] != "none";
}

QString GuiTeams::getStrategy(int team_id){
    return strategy[team_id];
}

void GuiTeams::updateTeams(){
    for(int i=0;i<2;i++){
        auto team = team_proxy[i].getTeam();
        strategy[i]   = team->getStrategyName();
    }
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
    robot_type[ROBOT_TEAM_BLUE]   = team_b["ROBOT_PROXY"]["TYPE"].Scalar().c_str();
    robot_type[ROBOT_TEAM_YELLOW] = team_y["ROBOT_PROXY"]["TYPE"].Scalar().c_str();

    controller[ROBOT_TEAM_BLUE]   = team_b["STRATEGY_CONTROLLER"]["ID"].Scalar().c_str();
    controller[ROBOT_TEAM_YELLOW] = team_y["STRATEGY_CONTROLLER"]["ID"].Scalar().c_str();

}
