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

// Project classes
#include "model/gamemodel.h"
#include "model/robot.h"
#include "include/config/team.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gamemodel = GameModel::getModel();


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
    checkTeamColors();
    fieldpanel->setupScene();
    fieldpanel->defaultZoom();
    robotpanel->setupBotPanel();
    selrobotpanel->setupSelRobotPanel();
    robotpanel->updateTeamColors();
    setupKeyShortcuts();
    objectPos->setupPastBotPoints();
    objectPos->setupBotSpeeds();
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

    //Joystick initialization
    joystick::listen();

    //All rboots overridden by default
    on_btn_override_all_released();
}

void MainWindow::handleJoystickInput()
{
    //Joystick updating is a bit different than keyboard. The joy axises can always
    //Be sent to the robot, so it is done here in the main loop
    for(joystick::reading& value: joystick::joystickReadings)
    {
        if(value.id == -1)
            continue;

        Robot* r = gameModel->findMyTeam(value.id);
        if(r->type() == fourWheelOmni) {
            r->setLB(value.LB);
            r->setRB(value.RB);
            r->setRF(value.RF);
            r->setLF(value.LF);
        } else {
            r->setR(value.RF);
            r->setL(value.LF);
            r->setB(value.RB);
        }

        if(value.Kick)
            r->setKick(5);

        if(value.Dribble)
            on_btn_botDrible_pressed();
        else on_btn_botDrible_released();
    }
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

    //Joystick support here
    handleJoystickInput();
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
    objectPos->updateBotSpeedsRecord();
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
    std::string time = std::to_string(gamemodel->getRemainingTime());
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
                case Qt::Key_S:
                    on_btn_botReverse_pressed();
                    break;
                case Qt::Key_D:
                    on_btn_botTurnRight_pressed();
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
            fieldpanel->robot0->setSelected(true);
            robotpanel->robotIcon0->setSelected(true);
            robotpanel->scrollToSelBot(0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot0->doubleClicked = true; }
            break;

        case Qt::Key_1:
            fieldpanel->robot1->setSelected(true);
            robotpanel->robotIcon1->setSelected(true);
            robotpanel->scrollToSelBot(1);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot1->doubleClicked = true; }
            break;

        case Qt::Key_2:
            fieldpanel->robot2->setSelected(true);
            robotpanel->robotIcon2->setSelected(true);
            robotpanel->scrollToSelBot(2);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot2->doubleClicked = true; }
            break;

        case Qt::Key_3:
            fieldpanel->robot3->setSelected(true);
            robotpanel->robotIcon3->setSelected(true);
            robotpanel->scrollToSelBot(3);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot3->doubleClicked = true; }
            break;

        case Qt::Key_4:
            fieldpanel->robot4->setSelected(true);
            robotpanel->robotIcon4->setSelected(true);
            robotpanel->scrollToSelBot(4);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot4->doubleClicked = true; }
            break;

        case Qt::Key_5:
            fieldpanel->robot5->setSelected(true);
            robotpanel->robotIcon5->setSelected(true);
            robotpanel->scrollToSelBot(5);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot5->doubleClicked = true; }
            break;

        case Qt::Key_6:
            fieldpanel->robot6->setSelected(true);
            robotpanel->robotIcon6->setSelected(true);
            robotpanel->scrollToSelBot(6);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot6->doubleClicked = true; }
            break;

        case Qt::Key_7:
            fieldpanel->robot7->setSelected(true);
            robotpanel->robotIcon7->setSelected(true);
            robotpanel->scrollToSelBot(7);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot7->doubleClicked = true; }
            break;

        case Qt::Key_8:
            fieldpanel->robot8->setSelected(true);
            robotpanel->robotIcon8->setSelected(true);
            robotpanel->scrollToSelBot(8);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot8->doubleClicked = true; }
            break;

        case Qt::Key_9:
            fieldpanel->robot9->setSelected(true);
            robotpanel->robotIcon9->setSelected(true);
            robotpanel->scrollToSelBot(9);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot9->doubleClicked = true; }
            break;

        case Qt::Key_0:
            fieldpanel->robot0->setSelected(true);
            robotpanel->robotIcon0->setSelected(true);
            robotpanel->scrollToSelBot(0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                fieldpanel->robot0->doubleClicked = true; }
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
                case Qt::Key_S:
                    on_btn_botReverse_released();
                    break;
                case Qt::Key_D:
                    on_btn_botTurnRight_released();
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

void MainWindow::checkTeamColors() {
    if (OUR_TEAM == TEAM_BLUE) {
        myTeam = "Blue";
    } else if (OUR_TEAM == TEAM_YELLOW) {
        myTeam = "Yellow";
    }
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
        int currentFwd = objectPos->getVelocity(fieldpanel->selectedBot);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentFwd);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentFwd);
        if (currentFwd <= 0) {
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentFwd+myVelocity);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentFwd+myVelocity);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setXVel(myVelocity);
        }
    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botForward_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = gamemodel->findMyTeam(fieldpanel->selectedBot);
        ui->btn_botForward->setDown(false);
        r->setL(0);
        r->setR(0);
        r->setXVel(0);
    }
}

void MainWindow::on_btn_botTurnRight_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = gamemodel->findMyTeam(fieldpanel->selectedBot);
        ui->btn_botTurnRight->setDown(true);
        r->setL(r->getL() + myVelocity/2);
        r->setR(r->getR() - myVelocity/2);
//        r->setAngVel(myVelocity);
        r->setYVel(myVelocity);
    }
}

void MainWindow::on_btn_botTurnRight_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);
        float currentFwd = objectPos->getVelocity(fieldpanel->selectedBot);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentFwd);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentFwd);
        //gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setAngVel(0);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setYVel(0);
    }
}

void MainWindow::on_btn_botTurnLeft_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        Robot* r = gamemodel->findMyTeam(fieldpanel->selectedBot);
        ui->btn_botTurnLeft->setDown(true);
        r->setL(r->getL() - myVelocity/2);
        r->setR(r->getR() + myVelocity/2);
//        r->setAngVel(-myVelocity);
        r->setYVel(-myVelocity);
    }
}

void MainWindow::on_btn_botTurnLeft_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        float currentVel = objectPos->getVelocity(fieldpanel->selectedBot);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentVel);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentVel);
//        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setAngVel(0);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setYVel(0);
    }
}

void MainWindow::on_btn_botReverse_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(true);
        int currentVel = objectPos->getVelocity(fieldpanel->selectedBot);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentVel);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentVel);
        if (currentVel >= 0) {
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(currentVel-myVelocity);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(currentVel-myVelocity);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setXVel(-myVelocity);
        }
    }
    ui->gView_field->scene()->update();
}

void MainWindow::on_btn_botReverse_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(false);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(0);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(0);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setXVel(0);
    }
}

void MainWindow::on_btn_botKick_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(true);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setKick();
        fieldpanel->guiTeam[fieldpanel->selectedBot]->kicking = true;
    }
}

void MainWindow::on_btn_botKick_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(false);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setKick(0);
        fieldpanel->guiTeam[fieldpanel->selectedBot]->kicking = false;
    }
}

void MainWindow::on_btn_botDrible_pressed() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(true);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setDribble(true);
        fieldpanel->guiTeam[fieldpanel->selectedBot]->dribling = true;
    }
}

void MainWindow::on_btn_botDrible_released() {
    if (fieldpanel->selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(false);
        gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setDribble(false);
        fieldpanel->guiTeam[fieldpanel->selectedBot]->dribling = false;
    }
}


void MainWindow::on_check_botOverride_clicked(bool checked) {
    if (fieldpanel->selectedBot > -1) {
        if (checked) {
            // Keeping track of how many bots are overridden
            overriddenBots[fieldpanel->selectedBot] = true;
            // Telling robot QObjects to change color
            robotpanel->botIcons[fieldpanel->selectedBot]->overridden = true;
            fieldpanel->guiTeam[fieldpanel->selectedBot]->overridden = true;
            // Stopping overridden bots in their tracks
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setL(0);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setR(0);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setB(0);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setXVel(0);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setYVel(0);
            gamemodel->find(fieldpanel->selectedBot, gamemodel->getMyTeam())->setAngVel(0);
        } else {
            overriddenBots[fieldpanel->selectedBot] = false;
            robotpanel->botIcons[fieldpanel->selectedBot]->overridden = false;
            fieldpanel->guiTeam[fieldpanel->selectedBot]->overridden = false;
        }
    }// nullcheck
}

void MainWindow::on_btn_override_all_released() {
    for (unsigned int i=0; i<overriddenBots.size(); i++) {
        // Keeping track of how many bots are overridden
        overriddenBots[i] = true;
    }
    for (int i=0; i<teamSize_blue; i++) {
        // Telling robot QObjects to change color
        overriddenBots[i] = true;
        robotpanel->botIcons[i]->overridden = true;
        fieldpanel->guiTeam[i]->overridden = true;
        // stopping all bots, so they don't fly off at their current velocities
        Robot* robot = gameModel->findMyTeam(i);
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
    for (unsigned int i=0; i<overriddenBots.size(); i++) {
        overriddenBots[i] = false;
        // Telling robot QObjects to change color
        robotpanel->botIcons[i]->overridden = false;
        fieldpanel->guiTeam[i]->overridden = false;
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
    if (myTeam == "Blue") {
        myTeam = "Yellow";
    } else if (myTeam == "Yellow") {
        myTeam = "Blue";
    }
    robotpanel->updateTeamColors();
}
