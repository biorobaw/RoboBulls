#include "gui_teams.h"
#include "model/team.h"
#include "robot/robot.h"

QString GuiTeams::robot_type[2] = {""};
QString GuiTeams::controller[2] = {""};
QString GuiTeams::strategy[2]   = {""};

namespace  {
    Robot team_proxy[2] = {Robot(0,0), Robot(0,1)};
}

QString GuiTeams::get_robot_type(int team_id){
    return robot_type[team_id];
}

QString GuiTeams::get_controller(int team_id){
    return controller[team_id];
}

bool GuiTeams::is_controlled(int team_id){
    return controller[team_id] != "none";
}

QString GuiTeams::get_strategy(int team_id){
    return strategy[team_id];
}

void GuiTeams::updateTeams(){
    for(int i=0;i<2;i++){
        auto team = team_proxy[i].getTeam();
        robot_type[i] = team->getRobotType();
        controller[i] = team->getTeamControllerName().c_str();
        strategy[i]   = team->getStrategyName().c_str();
    }
}
