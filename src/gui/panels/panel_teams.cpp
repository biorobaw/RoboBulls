#include "panel_teams.h"
#include "gui/data/gui_robot.h"
#include <iostream>
using std::cout, std::endl;


PanelTeams::PanelTeams(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);

    // add robot frames to scroll area
    tab_blue->set_team(0);
    tab_yellow->set_team(1);


    tab_yellow->setStyleSheet("background-color: blue;" "color: white");
    tab_yellow->setStyleSheet("background-color: yellow;" "color: black");
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            connect(GuiRobot::get(i,j), SIGNAL(selectedChanged(GuiRobot*)),
                    this, SLOT(scroll_to_selected(GuiRobot*)));


}

PanelTeams::~PanelTeams()
{
}

void PanelTeams::update_panel(){

    if(tabWidget->currentIndex()==ROBOT_TEAM_BLUE)
        tab_blue->update_tab();
    else tab_yellow->update_tab();
}

void PanelTeams::scroll_to_selected(GuiRobot* robot){
    if(robot==nullptr || !robot->selected()) return;
    tabWidget->setCurrentIndex(robot->team);
    (robot->team == ROBOT_TEAM_BLUE ? tab_blue : tab_yellow)
            ->show_robot(robot);

}

void PanelTeams::override_team(){
    auto tab = tabWidget->currentIndex() == ROBOT_TEAM_BLUE ? tab_blue : tab_yellow;
    tab->on_button_override_clicked();
}

void PanelTeams::release_team(){
    auto tab = tabWidget->currentIndex() == ROBOT_TEAM_BLUE ? tab_blue : tab_yellow;
    tab->on_button_release_clicked();
}






