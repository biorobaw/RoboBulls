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
#include "panels/selrobotpanel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "panels/gamepanel.h"
#include "panels/panel_field/fieldpanel.h"
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

// Project classes
#include "model/game_state.h"
#include "robot/robot.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Setting up GUI; not enabling thread until we're done
    ui->btn_connectGui->setEnabled(false);
    // Creating helper classes (order is important)
    selrobotpanel   = new SelRobotPanel(this);
    robotpanel      = new RobotPanel(this);
    fieldpanel      = new FieldPanel(this);
    gamepanel       = new GamePanel(this);

    // Generating GUI
    teamSize_blue = 10;
    teamSize_yellow = 10;
    fieldpanel->setupScene();
    fieldpanel->defaultZoom();
    robotpanel->setupBotPanel();
    selrobotpanel->setupSelRobotPanel();
    robotpanel->updateTeamColors();
    setupKeyShortcuts();
    GuiRobot::init_static_data();
    ui->btn_connectGui->setEnabled(true);
    MainWindow::resize(1400,900);
    setFocusOnField();

    // Time, in milliseconds, before GUI autoconnects to project; increase value if needed
    QTimer::singleShot(1000, this, SLOT(on_btn_connectGui_clicked()));


    // coreLoop
    timer.start(50);
    connect(&timer,SIGNAL(timeout()),this, SLOT(coreLoop()));

    // Zoom slider
    connect(ui->zoom_slider, SIGNAL(valueChanged(int)), fieldpanel, SLOT(zoomField(int)));

    // Default zoom button
    connect(ui->zoom_default, SIGNAL(clicked()), fieldpanel, SLOT(defaultZoom()));

    //All rboots overridden by default
    on_btn_override_all_released();
}

void MainWindow::coreLoop() {
    /* Top function of the GUI's loop
     */
    // Ctrl override
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        ui->scrollArea->verticalScrollBar()->setEnabled(false);
        ui->gView_field->verticalScrollBar()->setEnabled(false);
    }

    // Wiping values at beginning of cycle
    // prevents crash caused by (I think) the appended strings getting too long
    for (int i=0; i<teamSize_blue; i++) {
        selrobotpanel->botBehavior[i] = "";
    }

    // Interaction scanners
    fieldpanel->scanForScrollModifier();
    fieldpanel->scanForSelection();
    setMyVelocity();
    fieldpanel->updateScene();
    robotpanel->updateBotPanel();
    clockLoop();

}

void MainWindow::clockLoop() {
    // Clock-dependent stuff
    gamepanel->guiClock();
    // These three functions are used for bot speed getting;
    // ...their order is VERY important
    GuiBall::updateBall();
    updateBallInfo();
    GuiRobot::updateRobots();

}


void MainWindow::drawLine(int originX, int originY, int endX, int endY) {
    guidrawline->x1 = originX;
    guidrawline->y1 = originY;
    guidrawline->x2 = endX;
    guidrawline->y2 = endY;
    ui->gView_field->update();
}

void MainWindow::guiPrint(string output) {
//    guiOutput.insert(0, QString::fromStdString(output));
    ui->panel_output->print_output(output);
}

QString MainWindow::getRemTime() {
    QString t;  // return value
    std::string time = std::to_string(GameState::getRemainingTime());
    t = QString::fromStdString(time);

    return t;
}

void MainWindow::updateBallInfo() {
    ui->lcd_coordX_ball->display(GuiBall::getPosition().x);
    ui->lcd_coordY_ball->display(GuiBall::getPosition().y);

    GuiBall::ball.color = ui->combo_ballColor->currentText();
//    robotpanel->ballIcon->color =
    ui->gView_ball->update();
    // Displaying ball icon
    if (ui->gView_ball->isHidden()) {
        ui->gView_ball->show();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Robot binds
        switch(event->key()) {
            if (fieldpanel->selectedBot > -1) {
                case Qt::Key_W:
                    on_btn_botForward_pressed();
                    break;
                case Qt::Key_A:
                    on_btn_botTurnLeft_pressed();
                    break;
                case Qt::Key_Q:
                    on_btn_botRotateLeft_pressed();
                    break;
                case Qt::Key_S:
                    on_btn_botReverse_pressed();
                    break;
                case Qt::Key_D:
                    on_btn_botTurnRight_pressed();
                    break;
                case Qt::Key_E:
                    on_btn_botRotateRight_pressed();
                    break;
                // Alternate arrow bindings. Don't seem to work :(
                case Qt::UpArrow:
                    on_btn_botForward_pressed();
                    break;
                case Qt::LeftArrow:
                    on_btn_botTurnLeft_pressed();
                    break;
                case Qt::DownArrow:
                    on_btn_botReverse_pressed();
                    break;
                case Qt::RightArrow:
                    on_btn_botTurnRight_pressed();
                    break;

                case Qt::Key_Space:
                    on_btn_botKick_pressed();
                    break;
                case Qt::Key_J:
                    on_btn_botDrible_pressed();
                    break;
            }

        // Number bindings
        case Qt::Key_QuoteLeft:
            //GuiRobotProxy::proxies[selected_team_id][0].setSelected(true);
            robotpanel->scrollToSelBot(0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                GuiRobot::proxies[selected_team_id][0].doubleClicked = true; }
            break;

        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9: {
            int robot_id = event->key() - Qt::Key_0;
            auto& r = GuiRobot::proxies[selected_team_id][robot_id];
            //r.setSelected(true);
            robotpanel->scrollToSelBot(robot_id);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
                r.doubleClicked = true;
            break;
        }
        // Camera bindings
        case Qt::Key_Z:
            if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier)) {
                ui->zoom_slider->setValue(ui->zoom_slider->value()-5);
            } else {
                ui->zoom_slider->setValue(ui->zoom_slider->value()+5);
            }
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->defaultZoom();
            }
            break;

        case Qt::Key_Plus:
            ui->zoom_slider->setValue(ui->zoom_slider->value()+5);
            break;
        case Qt::Key_Minus:
            ui->zoom_slider->setValue(ui->zoom_slider->value()-5);
            break;

        case Qt::Key_R:
            if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier)) {
                on_btn_rotateField_left_clicked();
            } else {
                on_btn_rotateField_right_clicked();
            }
            break;

        // Remove selection from bot
        case Qt::Key_Escape:
            if (fieldpanel->selectedBot > -1)
                fieldpanel->sidelines->Pressed = true;
            break;

        // Field bindings
        case Qt::Key_G:
            ui->check_fieldGrid->click();
            break;

        // center window on field
        case Qt::Key_F:
            setFocusOnField();
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    // Robot binds
        switch(event->key()) {
            if (fieldpanel->selectedBot > -1) {
                case Qt::Key_W:
                    on_btn_botForward_released();
                    break;
                case Qt::Key_A:
                    on_btn_botTurnLeft_released();
                    break;
                case Qt::Key_Q:
                    on_btn_botRotateLeft_released();
                    break;
                case Qt::Key_S:
                    on_btn_botReverse_released();
                    break;
                case Qt::Key_D:
                    on_btn_botTurnRight_released();
                    break;
                case Qt::Key_E:
                    on_btn_botRotateRight_released();
                    break;
                case Qt::Key_Up:
                    on_btn_botForward_released();
                    break;
                case Qt::Key_Left:
                    on_btn_botTurnLeft_released();
                    break;
                case Qt::Key_Down:
                    on_btn_botReverse_released();
                    break;
                case Qt::Key_Right:
                    on_btn_botTurnRight_released();
                    break;

                case Qt::Key_Space:
                    on_btn_botKick_released();
                    break;
                case Qt::Key_J:
                    on_btn_botDrible_released();
                    break;
            }
        }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
//        ui->gView_field->centerOn(objectPos->getMouseCoordX(), objectPos->getMouseCoordY());

        if (event->delta() > 0) {
            ui->zoom_slider->setValue(ui->zoom_slider->value()+2);
        } else {
            ui->zoom_slider->setValue(ui->zoom_slider->value()-2);
        }

    } else {
        ui->scrollArea->verticalScrollBar()->setDisabled(false);
        ui->gView_field->verticalScrollBar()->setDisabled(false);
    }
}

void MainWindow::setupKeyShortcuts() {
    QShortcut *enter = new QShortcut(this);
    enter->setKey(Qt::Key_Enter);

    QShortcut *backspace = new QShortcut(this);
    backspace->setKey(Qt::Key_Backspace);

    QShortcut *o = new QShortcut(this);
    o->setKey(Qt::Key_O);

    QShortcut *delKey = new QShortcut(this);
    delKey->setKey(Qt::Key_Delete);

    QShortcut *teamOverride = new QShortcut(QKeySequence("Ctrl+O"), this);
    QShortcut *teamLiberate = new QShortcut(QKeySequence("Alt+O"), this);

    QShortcut *i = new QShortcut(this);
    i->setKey(Qt::Key_I);

    // Connecting key signals to their respective slots
    connect(enter, SIGNAL(activated()), this, SLOT(on_btn_connectGui_clicked()));
    connect(backspace, SIGNAL(activated()), this, SLOT(on_btn_connectGui_clicked()));
    connect(o, SIGNAL(activated()), ui->check_botOverride, SLOT(click()));
    connect(delKey, SIGNAL(activated()), robotpanel, SLOT(toggleIconVisible()));
    // Team override
    connect(teamOverride, SIGNAL(activated()), this, SLOT(on_btn_override_all_released()));
    connect(teamLiberate, SIGNAL(activated()), this, SLOT(on_btn_override_none_released()));

    connect(i, SIGNAL(activated()), ui->check_showIDs, SLOT(click()));
}

void MainWindow::setFocusOnField() {
    ui->scrollArea->ensureWidgetVisible(ui->gView_field,0,0);
    ui->scrollArea->verticalScrollBar()->setValue(205);
    ui->scrollArea->horizontalScrollBar()->setValue(315);
}

void MainWindow::setMyVelocity() {
    if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true) {
//        cout << "Shift \n";
        myVelocity = 100;
    } else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true) {
//        cout << "Alt \n";
        myVelocity = 25;
    } else {
        myVelocity = 50;
    }
}


MainWindow::~MainWindow()
{
    //TODO: Actually cleanup things.
}

void MainWindow::on_btn_connectGui_clicked() {
    if(ui->btn_connectGui->text() == "Connect") {
        ui->btn_connectGui->setText("Disconnect");
    } else {
        ui->btn_connectGui->setText("Connect");
    }
}

void MainWindow::on_btn_rotateField_right_clicked() {
    int rAngle = -45;
    ui->gView_field->rotate(rAngle);
    fieldpanel->currentFieldAngle += rAngle;
}

void MainWindow::on_btn_rotateField_left_clicked() {
    int lAngle = 45;
    ui->gView_field->rotate(lAngle);
    fieldpanel->currentFieldAngle += lAngle;
}

void MainWindow::on_btn_multithread_clicked() {
    if(ui->btn_multithread->text() == "Enabled") {
        ui->btn_multithread->setText("Disabled");
    } else {
        ui->btn_multithread->setText("Enabled");
    }
}

void MainWindow::on_btn_botForward_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botForward->setDown(true);
        setMyVelocity();

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(250,0),0);


    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botReverse_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(true);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(-250,0),0);

    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botRotateRight_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(true);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),-M_PI/2);

    }
}

void MainWindow::on_btn_botRotateLeft_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(true);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),M_PI/2);

    }
}

void MainWindow::on_btn_botTurnRight_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(true);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),-M_PI/2);

    }

}


void MainWindow::on_btn_botTurnLeft_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(true);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),M_PI/2);

    }

}















void MainWindow::on_btn_botForward_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botForward->setDown(false);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),0);

    }
}


void MainWindow::on_btn_botReverse_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(false);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),0);

    }
}



void MainWindow::on_btn_botRotateRight_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        float currentFwd = robot.getSpeedCommand();
        robot.setManualVelocity(Point(currentFwd,0),0);

    }
}

void MainWindow::on_btn_botRotateLeft_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),0);

    }
}

void MainWindow::on_btn_botTurnRight_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        float currentFwd = robot.getSpeedCommand();
        robot.setManualVelocity(Point(currentFwd,0),0);

    }
}


void MainWindow::on_btn_botTurnLeft_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setManualVelocity(Point(0,0),0);

    }
}




















void MainWindow::on_btn_botKick_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(true);

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setKick();
    }
}

void MainWindow::on_btn_botKick_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(false);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setKick(0);
    }
}

void MainWindow::on_btn_botDrible_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(true);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setDribble(true);
    }
}

void MainWindow::on_btn_botDrible_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(false);
        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.setDribble(false);
    }
}


void MainWindow::on_check_botOverride_clicked(bool checked) {
    if (fieldpanel->selectedBot > -1) {

        auto& robot = GuiRobot::proxies[selected_team_id][fieldpanel->selectedBot];
        robot.overridden = checked;
        if (checked) robot.setManualVelocity(Point(0,0),0);

    }// nullcheck
}

void MainWindow::on_btn_override_all_released() {

    for (int i=0; i<teamSize_blue; i++) {
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
void MainWindow::on_check_fieldGrid_clicked(){fieldpanel->refresh = true;}
void MainWindow::on_combo_gridScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    fieldpanel->refresh = true;
}
void MainWindow::on_check_coloredGoals_clicked(){fieldpanel->refresh = true;}
void MainWindow::on_combo_fieldColor_currentIndexChanged(int index){
    Q_UNUSED(index);
    fieldpanel->refresh = true;
}
void MainWindow::on_check_showIDs_stateChanged(int arg1){
    Q_UNUSED(arg1);
    fieldpanel->refresh = true;
}
void MainWindow::on_combo_botScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    fieldpanel->refresh = true;
}

void MainWindow::on_btn_toggleTeamColor_clicked() {
    selected_team_id = 1-selected_team_id;
    robotpanel->updateTeamColors();
    fieldpanel->updateTeamSelected();

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
