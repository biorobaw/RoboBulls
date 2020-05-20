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
#include "main_window.h"
#include "data/gui_ball.h"
#include "gui/graphics/graphics_field.h"
#include "gui/data/gui_ball.h"
#include "gui/graphics/graphics_robot_label.h"
#include "gui/graphics/graphics_robot.h"
#include "gui/data/gui_robot.h"
#include "gui/graphics/graphics_line.h"
#include "gui_interface.h"
#include "strategy/controllers/joystick/joystick.h"
#include "robot/robcomm.h"
#include "panels/panel_field.h"
#include "gui/graphics/graphics_outter_field.h"

// Project classes
#include "model/game_state.h"
#include "robot/robot.h"
#include "data/gui_teams.h"
#include "graphics/graphics_ball.h"


using namespace std;

MainWindow* MainWindow::dash = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    GuiRobot::init_static_data();
    dash = this;

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
    GuiTeams::update();

    // process user input:
    process_user_input();

    // update panels
    panel_field->updateScene();
    panel_teams->update_panel();
    panel_game_info->update_panel();
    panel_selected_robot->update_panel();

}

void MainWindow::process_user_input(){
    // Process user input

    // Interaction scanners
//    panel_field->scanForScrollModifier();
//    panel_field->scanForSelection();
    GraphicsBall::setColor(panel_field->combo_ballColor->currentText());

}



void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Robot binds
    bool control = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    bool alt = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    auto robot = GuiRobot::get_selected_robot();
    switch(event->key()) {
        // Robot control bindings
        if (robot != nullptr) {
            case Qt::Key_W:
            case Qt::UpArrow:
                robot->setManualVelocity(Point(250,0),0);
                break;
            case Qt::Key_S:
            case Qt::DownArrow:
                robot->setManualVelocity(Point(-250,0),0);
                break;
            case Qt::Key_A:
            case Qt::LeftArrow:
                robot->setManualVelocity(Point(0,0),M_PI/2);
                break;
            case Qt::Key_D:
            case Qt::RightArrow:
                robot->setManualVelocity(Point(0,0),-M_PI/2);
                break;
            case Qt::Key_Q:
                robot->setManualVelocity(Point(0,0),M_PI/2);
                break;
            case Qt::Key_E:
                robot->setManualVelocity(Point(0,0),-M_PI/2);
                break;
            case Qt::Key_Space:
                robot->setKick();
                break;
            case Qt::Key_Shift:
                robot->setDribble(true);
                break;
            // Remove selection from bot
            case Qt::Key_Escape:
                GuiRobot::clearSelected(true);
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
                GuiRobot::proxies[0][robot_id].select(true);
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
    auto robot = GuiRobot::get_selected_robot();

    switch(event->key()) {
        if ( robot != nullptr) {
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
                robot->setManualVelocity(Point(0,0),0);
                break;
            case Qt::Key_Space:
                robot->setKick(0);
                break;
            case Qt::Key_J:
                robot->setDribble(false);
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

    //TODO: reimplement
    //delete -> toggle icon visible
//    connect(new QShortcut(Qt::Key_Delete, this), SIGNAL(activated()),
//            robotpanel, SLOT(toggleIconVisible()));

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

void MainWindow::signal_new_robot_selected(GuiRobot* robot){
    panel_teams->show_robot(robot->team,robot->id);
    panel_selected_robot->update_selected_robot();

}
