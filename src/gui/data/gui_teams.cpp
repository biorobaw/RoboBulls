#include "gui_teams.h"
#include "model/team.h"

string GuiTeams::robot_type[2] = {""};
string GuiTeams::controller[2] = {""};
string GuiTeams::strategy[2]   = {""};

string GuiTeams::get_robot_type(int team_id){
    return robot_type[team_id];
}

string GuiTeams::get_controller(int team_id){
    return controller[team_id];
}

string GuiTeams::get_strategy(int team_id){
    return strategy[team_id];
}

void GuiTeams::updateTeams(){
    for(int i=0;i<2;i++){
        auto team = RobotTeam::getTeam(i);
        robot_type[i] = team->getRobotType();
        controller[i] = team->getControllerName();
        strategy[i]   = team->getStrategyName();
    }
}
