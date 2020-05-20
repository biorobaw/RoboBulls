#include "tab_team.h"
#include "gui/data/gui_teams.h"
#include "frame_robot.h"
#include <iostream>
#include "gui/data/gui_robot.h"

using std::cout, std::endl;

TabTeam::TabTeam(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);

    for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++){
        robot_frames[i] = new FrameRobot(this);
        layout_team->insertWidget(2+i,robot_frames[i]);
    }

}

TabTeam::~TabTeam()
{
}

void TabTeam::set_team(int id){
    team_id = id;
    for(int i=0; i<MAX_ROBOTS_PER_TEAM;i++){
        robot_frames[i]->set_robot(i,team_id);
    }
}

void TabTeam::update_tab(){
    label_strategy->setText(GuiTeams::get_strategy(team_id).c_str());
    label_controller->setText(GuiTeams::get_controller(team_id).c_str());
    label_robot_type->setText(GuiTeams::get_robot_type(team_id).c_str());

    for(int i=0; i< MAX_ROBOTS_PER_TEAM; i++)
        robot_frames[i]->update_frame();
}

void TabTeam::show_robot(int id){
    scrollArea->ensureWidgetVisible(robot_frames[id]);
    //    dash->scroll_robots->ensureVisible(0,91*(id+2),50,50);
}


void TabTeam::on_btn_override_all_released() {

//    for (int i=0; i<MAX_ROBOTS_PER_TEAM; i++) {
//        // Telling robot QObjects to change color
//        auto& robot = GuiRobot::proxies[selected_team_id][i];
//        robot.overridden = true;
//        robot.setManualVelocity(Point(0,0),0);
//    }
}

void TabTeam::on_btn_override_none_released() {
//    for (unsigned int i=0; i<MAX_ROBOTS_PER_TEAM; i++) {
//        // Telling robot QObjects to change color
//        GuiRobot::proxies[selected_team_id][i].overridden = false;
//    }
}
