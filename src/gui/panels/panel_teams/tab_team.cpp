#include "tab_team.h"
#include "frame_robot.h"

#include "gui/interface/data/gui_teams.h"
#include "gui/interface/data/gui_robot.h"
#include <iostream>
using std::cout, std::endl;


TabTeam::TabTeam(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);

    for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++){
        robot_frames[i] = new FrameRobot(this);
        layout_team->insertWidget(2+i,robot_frames[i]); // inserts robots at the end of the list before the vertical spacer

    }

}

TabTeam::~TabTeam()
{
}

void TabTeam::set_team(int id){
    team_id = id;
    for(int i=0; i<MAX_ROBOTS_PER_TEAM;i++){
        robot_frames[i]->set_robot(team_id,i);
    }
}

void TabTeam::update_tab(){
    label_strategy->setText(GuiTeams::getStrategy(team_id));
    label_controller->setText(GuiTeams::getController(team_id));
    label_robot_type->setText(GuiTeams::getRobotType(team_id));

    for(int i=0; i< MAX_ROBOTS_PER_TEAM; i++)
        robot_frames[i]->update_frame();
}

void TabTeam::show_robot(GuiRobot* robot){
    if(robot->getTeamId() == team_id && robot->selected())
        scrollArea->ensureWidgetVisible(robot_frames[robot->getID()]);
}



void TabTeam::on_button_release_clicked()
{
    for(int i=0;i<MAX_ROBOTS_PER_TEAM;i++){
        GuiRobot::get(team_id,i)->setOverriden(false);
    }
}

void TabTeam::on_button_override_clicked()
{
    for(int i=0;i<MAX_ROBOTS_PER_TEAM;i++){
        GuiRobot::get(team_id,i)->setOverriden(true);
    }
}
