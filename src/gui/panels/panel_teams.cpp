#include "panel_teams.h"

#include "panel_teams/frame_robot.h"
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

}

PanelTeams::~PanelTeams()
{
}

void PanelTeams::update_panel(){

    if(tabWidget->currentIndex()==ROBOT_TEAM_BLUE)
        tab_blue->update_tab();
    else tab_yellow->update_tab();
}

void PanelTeams::show_robot(int team, int id){

    tabWidget->setCurrentIndex(team);
    (team == ROBOT_TEAM_BLUE ? tab_blue : tab_yellow)->show_robot(id);

}


//    USELESS, THUS NOT REIMPLEMENTED
//void RobotPanel::toggleIconVisible() {
//    int id = dash->panel_field->selectedBot;
//    int team_id = dash->getSelectedTeamId();
//    auto& thisBot = GuiRobot::proxies[team_id][id];
//    GuiRobotDrawer *thisIcon = robotIcon[id];

//    if (dash->panel_field->selectedBot > -1) {
//        if (thisBot.visible) {
//            thisBot.visible = false;
////            thisIcon->enabled = false;
//            thisIcon->setOpacity(.3);
////            thisBot.setOpacity(.3);
//        } else {
//            thisBot.visible = true;
////            thisIcon->enabled = true;
//            thisIcon->setOpacity(1);
////            thisBot.setOpacity(1);
//        }
//    }
//}





