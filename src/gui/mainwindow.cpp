#define _USE_MATH_DEFINES
// to not generate gamemodel: comment out contents of strategy/strategycontroller.cpp->gameModelUpdated()
// collision notification
// different field & robot scales based on SIMULATED

// Tool classes
#include <math.h>
#include <time.h>
#include <iostream>
#include <QGraphicsView>
#include <QShortcut>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QtWidgets/QMainWindow>
#include "model/team.h"
// Helper classes
#include "panels/robotpanel.h"
#include "mainwindow.h"
#include "data/guiball.h"
#include "gui/data/guifield.h"
#include "gui/data/guiball.h"
#include "gui/data/guibotlabel.h"
#include "gui/utils/gui_robot_drawer.h"
#include "gui/data/gui_robot.h"
#include "gui/utils/guidrawline.h"
#include "guiinterface.h"
#include "strategy/controllers/joystick/joystick.h"
#include "robot/robcomm.h"
#include "panels/panel_field/panel_field.h"
#include "gui/guisidelines.h"

// Project classes
#include "model/game_state.h"
#include "robot/robot.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    GuiRobot::init_static_data();
    on_btn_override_all_released(); // override all robots


    // Set panels
    robotpanel = new RobotPanel(this);
    robotpanel->setupBotPanel();
    robotpanel->updateTeamColors();

    // Set keyboard shorcuts and connect slots
    setupKeyShortcuts();
    connect_slots();

    // resize window, and start main loop
    MainWindow::resize(1400,900);
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this, SLOT(coreLoop()));
    timer->start(25);

    // Time, in milliseconds, before GUI autoconnects to project; increase value if needed
    QTimer::singleShot(1000, panel_game_info, SLOT(on_btn_connectGui_clicked()));


}

void MainWindow::connect_slots(){
    connect(panel_field,SIGNAL(field_mouse_moved(QPointF)),panel_game_info,SLOT(update_mouse_pos(QPointF)));
}

void MainWindow::coreLoop() {

    /* Top function of the GUI's loop */

    // Update game data:
    GuiBall::updateBall();
    GuiRobot::updateRobots();

    // process user input:
    process_user_input();

    // update panels
    panel_field->updateScene();
    robotpanel->updateBotPanel();
    panel_game_info->update_panel();
    panel_selected_robot->update_panel();

}

void MainWindow::process_user_input(){
    // Process user input

    // Interaction scanners
//    panel_field->scanForScrollModifier();
//    panel_field->scanForSelection();
    GuiBall::ball.color = panel_field->combo_ballColor->currentText();

}



void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Robot binds
    bool control = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    bool alt = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    switch(event->key()) {
        // Robot control bindings
        if (GuiRobot::selected_robot > -1) {
            case Qt::Key_W:
            case Qt::UpArrow:
                control_selected_robot(250,0,0);
                break;
            case Qt::Key_S:
            case Qt::DownArrow:
                control_selected_robot(-250,0,0);
                break;
            case Qt::Key_A:
            case Qt::LeftArrow:
                control_selected_robot(0,0,M_PI/2);
                break;
            case Qt::Key_D:
            case Qt::RightArrow:
                control_selected_robot(0,0,-M_PI/2);
                break;
            case Qt::Key_Q:
                control_selected_robot(0,0,M_PI/2);
                break;
            case Qt::Key_E:
                control_selected_robot(0,0,-M_PI/2);
                break;
            case Qt::Key_Space:
                kick_selected_robot();
                break;
            case Qt::Key_Shift:
                kick_selected_robot(true);
                break;
            // Remove selection from bot
            case Qt::Key_Escape:
                select_robot(0,-1);
                break;
        }

        // Number bindings -
        {
            case Qt::Key_QuoteLeft:
            case Qt::Key_0:
            case Qt::Key_1:
            case Qt::Key_2:
            case Qt::Key_3:
            case Qt::Key_4:
            case Qt::Key_5:
            case Qt::Key_6:
            case Qt::Key_7:
            case Qt::Key_8:
            case Qt::Key_9:
                int robot_id = event->key() - Qt::Key_0;
                if(robot_id < 0 || robot_id >9) robot_id = 0;
                select_robot(0,robot_id);
                break;


        }


        // Camera bindings -
        {
            case Qt::Key_Z:
                if (alt) panel_field->defaultZoom();
                else {
                    int zoom = control ? -5 : 5;
                    panel_field->zoom_slider->setValue(panel_field->zoom_slider->value()+zoom);
                }
                break;

            case Qt::Key_Plus:
            case Qt::Key_Minus:{
                int zoom = event->key() == Qt::Key_Plus ? 5 : -5;
                panel_field->zoom_slider->setValue(panel_field->zoom_slider->value()+zoom);
                break;
            }
        }


    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    // Robot binds
        switch(event->key()) {
            if (GuiRobot::selected_robot > -1) {
                case Qt::Key_W:
                case Qt::Key_A:
                case Qt::Key_S:
                case Qt::Key_D:
                case Qt::Key_E:
                case Qt::Key_Q:
                case Qt::Key_Up:
                case Qt::Key_Left:
                case Qt::Key_Down:
                case Qt::Key_Right:
                    control_selected_robot(0,0,0);
                    break;
                case Qt::Key_Space:
                    kick_selected_robot(0);
                    break;
                case Qt::Key_J:
                    dribble_selected_robot(false);
                    break;
            }
        }
}


void MainWindow::setupKeyShortcuts() {
    // enter -> connect gui
    connect(new QShortcut(Qt::Key_Enter,this), SIGNAL(activated()),
            panel_game_info , SLOT(on_btn_connectGui_clicked()));

    //backspace -> connect gui
    connect(new QShortcut(Qt::Key_Backspace,this), SIGNAL(activated()),
            panel_game_info, SLOT(on_btn_connectGui_clicked()));

    //o -> override selected robot
    connect(new QShortcut(Qt::Key_O,this), SIGNAL(activated()),
            panel_selected_robot->check_override, SLOT(click()));

    //delete -> toggle icon visible
    connect(new QShortcut(Qt::Key_Delete, this), SIGNAL(activated()),
            robotpanel, SLOT(toggleIconVisible()));

    // Ctrl+o -> Override team
    connect(new QShortcut(QKeySequence("Ctrl+O"), this), SIGNAL(activated()),
            this, SLOT(on_btn_override_all_released()));

    // Alt+o -> Release overrided team
    connect(new QShortcut(QKeySequence("Alt+O"), this), SIGNAL(activated()),
            this, SLOT(on_btn_override_none_released()));

    // i -> toggle show ids
    connect(new QShortcut(Qt::Key_I,this), SIGNAL(activated()),
            panel_field->check_showIDs, SLOT(click()));

    // Field bindings -
    connect(new QShortcut(Qt::Key_G,this), SIGNAL(activated()),
            panel_field->check_fieldGrid, SLOT(click()));

}


MainWindow::~MainWindow()
{
    //TODO: Actually cleanup things.
}

void MainWindow::control_selected_robot(float vx, float vy, float w){
    auto robot = GuiRobot::get_selected_robot();
    if(robot) robot->setManualVelocity(Point(vx,vy),w);
}

void MainWindow::dribble_selected_robot(bool dribble){
    auto robot = GuiRobot::get_selected_robot();
    if(robot) robot->setDribble(dribble);
}

void MainWindow::kick_selected_robot(float kick){
    auto robot = GuiRobot::get_selected_robot();
    if(robot) robot->setKick(kick);
}



void MainWindow::on_check_botOverride_clicked(bool checked) {
    if (panel_field->selectedBot > -1) {

        auto& robot = GuiRobot::proxies[selected_team_id][panel_field->selectedBot];
        robot.overridden = checked;
        if (checked) robot.setManualVelocity(Point(0,0),0);

    }// nullcheck
}

void MainWindow::on_btn_override_all_released() {

    for (int i=0; i<MAX_ROBOTS_PER_TEAM; i++) {
        // Telling robot QObjects to change color
        auto& robot = GuiRobot::proxies[selected_team_id][i];
        robot.overridden = true;
        robot.setManualVelocity(Point(0,0),0);
    }
}

void MainWindow::on_btn_override_none_released() {
    for (unsigned int i=0; i<MAX_ROBOTS_PER_TEAM; i++) {
        // Telling robot QObjects to change color
        GuiRobot::proxies[selected_team_id][i].overridden = false;
    }
}


// Field graphical settings which need to be refreshed when changed
void MainWindow::on_check_fieldGrid_clicked(){panel_field->refresh = true;}
void MainWindow::on_combo_gridScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    panel_field->refresh = true;
}
void MainWindow::on_check_coloredGoals_clicked(){panel_field->refresh = true;}
void MainWindow::on_combo_fieldColor_currentIndexChanged(int index){
    Q_UNUSED(index);
    panel_field->refresh = true;
}
void MainWindow::on_check_showIDs_stateChanged(int arg1){
    Q_UNUSED(arg1);
    panel_field->refresh = true;
}
void MainWindow::on_combo_botScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    panel_field->refresh = true;
}

void MainWindow::on_btn_toggleTeamColor_clicked() {
    selected_team_id = 1-selected_team_id;
    robotpanel->updateTeamColors();
    panel_field->updateTeamSelected();

}


int MainWindow::getSelectedTeamId(){
    return selected_team_id;
}
void MainWindow::setSelectedTeamId(int team_id){
    selected_team_id = team_id;
}
std::string MainWindow::getSelectedTeamName(){
    return selected_team_id == ROBOT_TEAM_BLUE ? "Blue" : "Yellow";
}
RobotTeam* MainWindow::getSelectedTeam(){
    return RobotTeam::getTeam(selected_team_id);
}

void MainWindow::select_robot(int team, int robot){
    // if currently controlling a robot
    if(GuiRobot::selected_robot!=-1)
        GuiRobot::get_selected_robot()->setManualVelocity(Point(0,0),0);

    GuiRobot::selected_team = team;
    GuiRobot::selected_robot = robot;


    robotpanel->scrollToSelBot(0);
    panel_selected_robot->update_selected_robot(team,robot);

}
