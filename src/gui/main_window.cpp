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

// Helper classes
#include "main_window.h"

#include "data/gui_ball.h"
#include "gui/data/gui_ball.h"
#include "gui/data/gui_field.h"
#include "gui/data/gui_robot.h"
#include "gui/data/gui_teams.h"
#include "gui/graphics/graphics_ball.h"


using namespace std;

MainWindow* MainWindow::dash = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
//    GuiRobot::init_static_data();
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
    connect(panel_game_info,SIGNAL(toggle_connect_gui()),this,SLOT(toggle_connect_gui()));
}

void MainWindow::coreLoop() {

    if(!gui_connected) return;
    /* Top function of the GUI's loop */

    // Update game data:
    GuiBall::updateBall();
    GuiRobot::updateRobots();
    GuiTeams::updateTeams();
    GuiField::updateField();

    // process user input:
    process_user_input();

    // update panels
    panel_field->updateScene();
    panel_teams->update_panel();
    panel_game_info->update_panel();
    panel_selected_robot->update_panel();

}


void MainWindow::toggle_connect_gui(){
    if(gui_connected){
        //disable  components
        panel_field->setEnabled(false);
        panel_selected_robot->setEnabled(false);
        panel_teams->setEnabled(false);
        panel_output->setEnabled(false);
    } else {
        // re enable components
        panel_field->setEnabled(true);
        panel_selected_robot->setEnabled(true);
        panel_teams->setEnabled(true);
        panel_output->setEnabled(true);
    }
    gui_connected=!gui_connected;
}

void MainWindow::process_user_input(){
    // Process user input

    // Interaction scanners
//    panel_field->scanForScrollModifier();
//    panel_field->scanForSelection();
    GraphicsBall::setColor(panel_field->combo_ball_color->currentText());

}



void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Robot binds
    bool control = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    bool alt = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    auto robot = GuiRobot::get_selected_robot();

    if (robot != nullptr) {

        switch(event->key()) {
            // Robot control bindings
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
            case Qt::Key_Space:
                robot->setKick();
                break;
            case Qt::Key_Shift:
                cout << "Dribbling!!!\n";
                robot->setDribble(true);
                cout << "Dribbling done!!!\n";
                break;
            // Remove selection from bot
            case Qt::Key_Escape:
                GuiRobot::clearSelected();
                break;
        }
    }

    switch(event->key()) {
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
                int team = control ? ROBOT_TEAM_YELLOW : ROBOT_TEAM_BLUE;
                GuiRobot::get(team,robot_id)->select();
                break;
        }


        // Camera bindings -
        {
            case Qt::Key_Z:
                if (alt) panel_field->defaultZoom();
                else {
                    int zoom = control ? -5 : 5;
                    panel_field->slider_zoom->setValue(panel_field->slider_zoom->value()+zoom);
                }
                break;

            case Qt::Key_Plus:
            case Qt::Key_Minus:{
                int zoom = event->key() == Qt::Key_Plus ? 5 : -5;
                panel_field->slider_zoom->setValue(panel_field->slider_zoom->value()+zoom);
                break;
            }
            case Qt::Key_Control:
                panel_field->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
                break;
        }

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    // Robot binds
    auto robot = GuiRobot::get_selected_robot();

    if ( robot != nullptr) {
        switch(event->key()) {
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

    if(event->key() == Qt::Key_Control)
        panel_field->setDragMode(QGraphicsView::DragMode::NoDrag);
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


    // Ctrl+o -> Override team
    connect(new QShortcut(QKeySequence("Ctrl+O"), this), SIGNAL(activated()),
            this, SLOT(on_btn_override_all_released()));

    // Alt+o -> Release overrided team
    connect(new QShortcut(QKeySequence("Alt+O"), this), SIGNAL(activated()),
            this, SLOT(on_btn_override_none_released()));

    // i -> toggle show ids
    connect(new QShortcut(Qt::Key_I,this), SIGNAL(activated()),
            panel_field->check_show_ids, SLOT(click()));

    // Field bindings -
    connect(new QShortcut(Qt::Key_G,this), SIGNAL(activated()),
            panel_field->check_field_grid, SLOT(click()));

}



MainWindow::~MainWindow()
{
    //TODO: Actually cleanup things.
}