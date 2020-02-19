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
#include "robotpanel.h"
#include "selrobotpanel.h"
#include "objectposition.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamepanel.h"
#include "fieldpanel.h"
#include "guifield.h"
#include "guiball.h"
#include "guibotlabel.h"
#include "guidrawline.h"
#include "guicomm.h"
#include "guiscene.h"
#include "guiinterface.h"
#include "guirobot.h"
#include "joystick.h"
#include "communication/robcomm.h"

// Project classes
#include "model/game_state.h"
#include "model/robot.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Setting up GUI; not enabling thread until we're done
    ui->btn_connectGui->setEnabled(false);
    // Creating helper classes (order is important)
    objectPos       = new ObjectPosition(this);
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
    objectPos->setupPastBotPoints();
    ui->btn_connectGui->setEnabled(true);
    MainWindow::resize(1400,900);
    setFocusOnField();

    // Time, in milliseconds, before GUI autoconnects to project; increase value if needed
    QTimer::singleShot(1000, this, SLOT(on_btn_connectGui_clicked()));

    guimodel = new GuiComm(50,this);

    // coreLoop
    connect(guimodel, SIGNAL(valueChanged(int)), this, SLOT(coreLoop(int)));

    // Zoom slider
    connect(ui->zoom_slider, SIGNAL(valueChanged(int)), fieldpanel, SLOT(zoomField(int)));

    // Default zoom button
    connect(ui->zoom_default, SIGNAL(clicked()), fieldpanel, SLOT(defaultZoom()));

    // Start joystick thread
    JoyStick::listen();

    //All rboots overridden by default
    on_btn_override_all_released();
}

void MainWindow::coreLoop(int tick) {
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
    updateBallInfo();
    clockLoop(tick);

    // Actuate joystick commands
    JoyStick::setCommands();
}

void MainWindow::clockLoop(int tick) {
    // Clock-dependent stuff
    gamepanel->guiClock(tick);
    // These three functions are used for bot speed getting;
    // ...their order is VERY important
    objectPos->getNewBotPoints();
    objectPos->getBotSpeeds();
    objectPos->getOldSpeeds();
    objectPos->getPastBotPoints();
//    objectPos->updateBotSpeedsRecord();
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
    ui->text_guiPrint->setTextColor(Qt::white);
    if (guiOutput.toStdString() == output) {

    } else {
        // recording this string
        guiOutput = QString::fromStdString(output);
        // converting received string to QString for printing
        QString msg = QString::fromStdString(output);
        ui->text_guiPrint->append(msg);
        // Scrolling to bottom of text box
        QScrollBar *sb = ui->text_guiPrint->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}

QString MainWindow::getRemTime() {
    QString t;  // return value
    std::string time = std::to_string(GameState::getRemainingTime());
    t = QString::fromStdString(time);

    return t;
}

void MainWindow::updateBallInfo() {
    ui->lcd_coordX_ball->display(objectPos->getBallCoordX());
    ui->lcd_coordY_ball->display(objectPos->getBallCoordY());

    robotpanel->ballIcon->color = ui->combo_ballColor->currentText();
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
            fieldpanel->gui_robots[selected_team_id][0]->setSelected(true);
            robotpanel->robotIcon[0]->setSelected(true);
            robotpanel->scrollToSelBot(0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][0]->doubleClicked = true; }
            break;

        case Qt::Key_1:
            fieldpanel->gui_robots[selected_team_id][1]->setSelected(true);
            robotpanel->robotIcon[1]->setSelected(true);
            robotpanel->scrollToSelBot(1);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][1]->doubleClicked = true; }
            break;

        case Qt::Key_2:
            fieldpanel->gui_robots[selected_team_id][2]->setSelected(true);
            robotpanel->robotIcon[2]->setSelected(true);
            robotpanel->scrollToSelBot(2);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][2]->doubleClicked = true; }
            break;

        case Qt::Key_3:
            fieldpanel->gui_robots[selected_team_id][3]->setSelected(true);
            robotpanel->robotIcon[3]->setSelected(true);
            robotpanel->scrollToSelBot(3);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][3]->doubleClicked = true; }
            break;

        case Qt::Key_4:
            fieldpanel->gui_robots[selected_team_id][4]->setSelected(true);
            robotpanel->robotIcon[4]->setSelected(true);
            robotpanel->scrollToSelBot(4);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][4]->doubleClicked = true; }
            break;

        case Qt::Key_5:
            fieldpanel->gui_robots[selected_team_id][5]->setSelected(true);
            robotpanel->robotIcon[5]->setSelected(true);
            robotpanel->scrollToSelBot(5);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][5]->doubleClicked = true; }
            break;

        case Qt::Key_6:
            fieldpanel->gui_robots[selected_team_id][6]->setSelected(true);
            robotpanel->robotIcon[6]->setSelected(true);
            robotpanel->scrollToSelBot(6);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][6]->doubleClicked = true; }
            break;

        case Qt::Key_7:
            fieldpanel->gui_robots[selected_team_id][7]->setSelected(true);
            robotpanel->robotIcon[7]->setSelected(true);
            robotpanel->scrollToSelBot(7);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][7]->doubleClicked = true; }
            break;

        case Qt::Key_8:
            fieldpanel->gui_robots[selected_team_id][8]->setSelected(true);
            robotpanel->robotIcon[8]->setSelected(true);
            robotpanel->scrollToSelBot(8);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][8]->doubleClicked = true; }
            break;

        case Qt::Key_9:
            fieldpanel->gui_robots[selected_team_id][9]->setSelected(true);
            robotpanel->robotIcon[9]->setSelected(true);
            robotpanel->scrollToSelBot(9);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][9]->doubleClicked = true; }
            break;

        case Qt::Key_0:
            fieldpanel->gui_robots[selected_team_id][0]->setSelected(true);
            robotpanel->robotIcon[0]->setSelected(true);
            robotpanel->scrollToSelBot(0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->gui_robots[selected_team_id][0]->doubleClicked = true; }
            break;

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
        guimodel->start();
    } else {
        ui->btn_connectGui->setText("Connect");
        guimodel->exit(0);
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
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        int currentFwd = objectPos->getVelocity(fieldpanel->selectedBot);
        std::cout << "currentFwd" << currentFwd<< std::endl;
        selected_robot->setL(currentFwd);
        selected_robot->setR(currentFwd);
        if (currentFwd <= 0) {
            selected_robot->setL(currentFwd+myVelocity);
            selected_robot->setR(currentFwd+myVelocity);
            selected_robot->setXVel(myVelocity);
        }
    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botForward_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        ui->btn_botForward->setDown(false);
        r->setL(0);
        r->setR(0);
        r->setXVel(0);
    }
}

void MainWindow::on_btn_botTurnRight_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        ui->btn_botTurnRight->setDown(true);
        r->setL(r->getL() + myVelocity/2);//----------->>>
        r->setR(r->getR() - myVelocity/2);
//        r->setAngVel(myVelocity);
        r->setYVel(myVelocity);
        std::cout << "botTurnRight_pressed" << std::endl;
    }

}
void MainWindow::on_btn_botRotateRight_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        ui->btn_botTurnRight->setDown(true);
        r->setL(r->getL() + myVelocity/2);//----------->>>
        r->setR(r->getR() - myVelocity/2);
        r->setAngVel(myVelocity);
//        r->setYVel(myVelocity);
        std::cout << "botTurnRight_pressed" << std::endl;
    }
}
void MainWindow::on_btn_botTurnRight_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);
        float currentFwd = objectPos->getVelocity(fieldpanel->selectedBot);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(currentFwd);
        selected_robot->setR(currentFwd);
        selected_robot->setYVel(0);
        std::cout << "botTurnRight_released" << std::endl;
    }
}

void MainWindow::on_btn_botRotateRight_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);
        float currentFwd = objectPos->getVelocity(fieldpanel->selectedBot);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(currentFwd);
        selected_robot->setR(currentFwd);
        selected_robot->setAngVel(0);
        std::cout << "botTurnRight_released" << std::endl;
    }
}

void MainWindow::on_btn_botTurnLeft_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        ui->btn_botTurnLeft->setDown(true);
        r->setL(r->getL() - myVelocity/2);
        r->setR(r->getR() + myVelocity/2);
//        r->setAngVel(-myVelocity);
        r->setYVel(-myVelocity);
    }

}

void MainWindow::on_btn_botRotateLeft_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        ui->btn_botTurnLeft->setDown(true);
        r->setL(r->getL() - myVelocity/2);
        r->setR(r->getR() + myVelocity/2);
        r->setAngVel(-myVelocity);
//        r->setYVel(-myVelocity);
    }
}
void MainWindow::on_btn_botTurnLeft_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        float currentVel = objectPos->getVelocity(fieldpanel->selectedBot);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(currentVel);
        selected_robot->setR(currentVel);
        selected_robot->setYVel(0);
    }
}

void MainWindow::on_btn_botRotateLeft_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        float currentVel = objectPos->getVelocity(fieldpanel->selectedBot);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(currentVel);
        selected_robot->setR(currentVel);
        selected_robot->setAngVel(0);
    }
}
void MainWindow::on_btn_botReverse_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(true);
        int currentVel = objectPos->getVelocity(fieldpanel->selectedBot);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(currentVel);
        selected_robot->setR(currentVel);
        if (currentVel >= 0) {
            selected_robot->setL(currentVel-myVelocity);
            selected_robot->setR(currentVel-myVelocity);
            selected_robot->setXVel(-myVelocity);
        }
    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botReverse_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(false);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setL(0);
        selected_robot->setR(0);
        selected_robot->setXVel(0);
    }
}

void MainWindow::on_btn_botKick_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(true);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setKick();
        fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->kicking = true;
    }
}

void MainWindow::on_btn_botKick_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(false);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setKick(0);
        fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->kicking = false;
    }
}

void MainWindow::on_btn_botDrible_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(true);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setDribble(true);
        fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->dribling = true;
    }
}

void MainWindow::on_btn_botDrible_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(false);
        auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
        selected_robot->setDribble(false);
        fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->dribling = false;
    }
}


void MainWindow::on_check_botOverride_clicked(bool checked) {
    if (fieldpanel->selectedBot > -1) {
        if (checked) {
            // Keeping track of how many bots are overridden
            overriddenBots[selected_team_id][fieldpanel->selectedBot] = true;
            // Telling robot QObjects to change color
            robotpanel->robotIcon[fieldpanel->selectedBot]->overridden = true;
            fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->overridden = true;
            // Stopping overridden bots in their tracks
            auto* selected_robot = getSelectedTeam()->getRobot(fieldpanel->selectedBot);
            selected_robot->setL(0);
            selected_robot->setR(0);
            selected_robot->setB(0);
            selected_robot->setXVel(0);
            selected_robot->setYVel(0);
            selected_robot->setAngVel(0);
        } else {
            overriddenBots[selected_team_id][fieldpanel->selectedBot] = false;
            robotpanel->robotIcon[fieldpanel->selectedBot]->overridden = false;
            fieldpanel->gui_robots[selected_team_id][fieldpanel->selectedBot]->overridden = false;
        }
    }// nullcheck
}

void MainWindow::on_btn_override_all_released() {
    for (unsigned int i=0; i< MAX_ROBOTS; i++) {
        // Keeping track of how many bots are overridden
        overriddenBots[selected_team_id][i] = true;
    }
    for (int i=0; i<teamSize_blue; i++) {
        // Telling robot QObjects to change color
        overriddenBots[selected_team_id][i] = true;
        robotpanel->robotIcon[i]->overridden = true;
        fieldpanel->gui_robots[selected_team_id][i]->overridden = true;
        // stopping all bots, so they don't fly off at their current velocities
        Robot* robot = getSelectedTeam()->getRobot(i);
        if(robot != NULL) {
            robot->setL(0);
            robot->setR(0);
            robot->setB(0);
            robot->setXVel(0);
            robot->setYVel(0);
            robot->setAngVel(0);
        }
    }
}

void MainWindow::on_btn_override_none_released() {
    for (unsigned int i=0; i<MAX_ROBOTS; i++) {
        overriddenBots[selected_team_id][i] = false;
        // Telling robot QObjects to change color
        robotpanel->robotIcon[i]->overridden = false;
        fieldpanel->gui_robots[selected_team_id][i]->overridden = false;
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
    return selected_team_id == TEAM_BLUE ? "Blue" : "Yellow";
}
Team* MainWindow::getSelectedTeam(){
    return Team::getTeam(selected_team_id);
}
