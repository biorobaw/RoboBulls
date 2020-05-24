#include "gui_teams.h"
#include "model/team.h"

QString GuiTeams::robot_type[2] = {""};
QString GuiTeams::controller[2] = {""};
QString GuiTeams::strategy[2]   = {""};

QString GuiTeams::get_robot_type(int team_id){
    return robot_type[team_id];
}

QString GuiTeams::get_controller(int team_id){
    return controller[team_id];
}

QString GuiTeams::get_strategy(int team_id){
    return strategy[team_id];
}

void GuiTeams::updateTeams(){
    for(int i=0;i<2;i++){
        auto team = RobotTeam::getTeam(i);
        robot_type[i] = team->getRobotType().c_str();
        controller[i] = team->getControllerName().c_str();
        strategy[i]   = team->getStrategyName().c_str();
    }
}
