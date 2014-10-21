#define _USE_MATH_DEFINES
// to not generate gamemodel: comment out contents of strategy/strategycontroller.cpp->gameModelUpdated()
// colision notification
// different field & robot scales based on SIMULATED
// make bearing dial gray if robot is NULL
// communication/nxtrobcomm.cpp - sets velocity to zero
#include <math.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "model/gamemodel.h"
#include "model/robot.h"
#include "communication/visioncomm.h"
#include <string>
#include "utilities/point.h"
#include "model/robot.h"
#include "sys/wait.h"
#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include "guicomm.h"
#include <QLCDNumber>
#include "include/config/simulated.h"
#include <time.h>
#include <math.h>
#include "include/config/simulated.h"
// QGraphicsView
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QShortcut>
#include <QMenu>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMap>
#include <QCursor>
#include "communication/refcomm.h"
#include "guidrawline.h"
#include "communication/nxtrobcomm.h"
#include "movement/move.h"
#include <QScrollBar>

// Global static pointer used to ensure a single isntance of the class.
MainWindow* MainWindow::mw = NULL;

using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    gamemodel = GameModel::getModel();


    ui->pushButton->setEnabled(false);
    setUpScene();
    setupBotPanel();
    ui->pushButton->setEnabled(true);

//    guiOverride = false;

    defaultZoom();

    // Create Threads, the parameters are the timer value, and parent.
    // threads is declated as QList<GuiComm*> threads;
    // create threads, and append them to the threads list, so that
    // threads can be accessed for making connections, and to start
    // and stop threads
    threads.append(new GuiComm(30, this));
//    threads.append(new GuiComm(30, this));
//    cout << "threads.append \n";

    // Connect each Widget to correcponding thread
    connect(threads[0], SIGNAL(valueChanged(int))
            , this, SLOT(launch(int)));
//    cout << "connect(threads[0] \n";

//    connect(threads[1], SIGNAL(valueChanged(int))
//            , this, SLOT(updateScene()));

    // Zoom slider
    connect(ui->zoom_slider, SIGNAL(valueChanged(int))
            , this, SLOT(zoomField(int)));
    // Default zoom button
    connect(ui->zoom_default, SIGNAL(clicked())
            , this, SLOT(defaultZoom()));
}

void MainWindow::launch(int value)
{
    if (SIMULATED) {
        ui->menuDashboard->setTitle("Simulation");
    } else {
        ui->menuDashboard->setTitle("Camera");
    }


    ui->label->setText(QString("Current Thread Processing Status : %1").arg(value));

    // CTRL modifer for field scrolling
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        ui->gView_field->setDragMode(QGraphicsView::ScrollHandDrag);
        justScrolled = true;
    } else {
        ui->gView_field->setDragMode(QGraphicsView::NoDrag);
        if (justScrolled) {
            justScrolled = false;
            refresh = true;
        }
    }
    // Updating GUI
    setGuiOverride();
    setMyVelocity();    //
    updateScene();
    updateBotPanel();
    updateBallInfo();
    scanForSelection();
}

void MainWindow::zoomField(int zoom) {
    ui->zoom_slider->setValue(zoom);
    double zoomScale = zoom *.01;
    ui->gView_field->setTransform(QTransform::fromScale(zoomScale, zoomScale));
    ui->gView_field->scale(1, -1);
    ui->gView_field->rotate(currentFieldAngle);
}


void MainWindow::defaultZoom() {
    // Removing robot focus
    centeredBotID = -1;

    currentFieldAngle = 0;
    zoomField(11);
    ui->zoom_slider->setValue(11);
//    ui->gView_field->hide();
    ui->gView_field->centerOn(sidelines);
}

void MainWindow::setupBotPanel()
{
    // Bot Frames
    botFrames.push_back(ui->frame_robot_0);
    botFrames.push_back(ui->frame_robot_1);
    botFrames.push_back(ui->frame_robot_2);
    botFrames.push_back(ui->frame_robot_3);
    botFrames.push_back(ui->frame_robot_4);
    botFrames.push_back(ui->frame_robot_5);
    // Title label vector
    botTitle.push_back(ui->title_robPanel_0);
    botTitle.push_back(ui->title_robPanel_1);
    botTitle.push_back(ui->title_robPanel_2);
    botTitle.push_back(ui->title_robPanel_3);
    botTitle.push_back(ui->title_robPanel_4);
    botTitle.push_back(ui->title_robPanel_5);
    // Velocity dials
    velocityDials.push_back(ui->dial_botSpeed_0);
    velocityDials.push_back(ui->dial_botSpeed_1);
    velocityDials.push_back(ui->dial_botSpeed_2);
    velocityDials.push_back(ui->dial_botSpeed_3);
    velocityDials.push_back(ui->dial_botSpeed_4);
    velocityDials.push_back(ui->dial_botSpeed_5);
    // X Coordinate vector
    botXcoords.push_back(ui->lcd_coordX_0);
    botXcoords.push_back(ui->lcd_coordX_1);
    botXcoords.push_back(ui->lcd_coordX_2);
    botXcoords.push_back(ui->lcd_coordX_3);
    botXcoords.push_back(ui->lcd_coordX_4);
    botXcoords.push_back(ui->lcd_coordX_5);
    // Y Coordinate vector
    botYcoords.push_back(ui->lcd_coordY_0);
    botYcoords.push_back(ui->lcd_coordY_1);
    botYcoords.push_back(ui->lcd_coordY_2);
    botYcoords.push_back(ui->lcd_coordY_3);
    botYcoords.push_back(ui->lcd_coordY_4);
    botYcoords.push_back(ui->lcd_coordY_5);
    // Orientation vector
    botOrients.push_back(ui->dial_botOrient_0);
    botOrients.push_back(ui->dial_botOrient_1);
    botOrients.push_back(ui->dial_botOrient_2);
    botOrients.push_back(ui->dial_botOrient_3);
    botOrients.push_back(ui->dial_botOrient_4);
    botOrients.push_back(ui->dial_botOrient_5);
    // Icons vector
    robotIcon0 = new GuiRobot;
    robotIcon1 = new GuiRobot;
    robotIcon2 = new GuiRobot;
    robotIcon3 = new GuiRobot;
    robotIcon4 = new GuiRobot;
    robotIcon5 = new GuiRobot;
    botIcons.push_back(robotIcon0);
    botIcons.push_back(robotIcon1);
    botIcons.push_back(robotIcon2);
    botIcons.push_back(robotIcon3);
    botIcons.push_back(robotIcon4);
    botIcons.push_back(robotIcon5);
    // Icon scenes vector
    scene_botIcon_0 = new QGraphicsScene;
    scene_botIcon_1 = new QGraphicsScene;
    scene_botIcon_2 = new QGraphicsScene;
    scene_botIcon_3 = new QGraphicsScene;
    scene_botIcon_4 = new QGraphicsScene;
    scene_botIcon_5 = new QGraphicsScene;
    botIconScenes.push_back(scene_botIcon_0);
    botIconScenes.push_back(scene_botIcon_1);
    botIconScenes.push_back(scene_botIcon_2);
    botIconScenes.push_back(scene_botIcon_3);
    botIconScenes.push_back(scene_botIcon_4);
    botIconScenes.push_back(scene_botIcon_5);
    // Icon frames vector
    botIconFrames.push_back(ui->gView_robot_0);
    botIconFrames.push_back(ui->gView_robot_1);
    botIconFrames.push_back(ui->gView_robot_2);
    botIconFrames.push_back(ui->gView_robot_3);
    botIconFrames.push_back(ui->gView_robot_4);
    botIconFrames.push_back(ui->gView_robot_5);
    // Selected bot icons
    robotIcon0Sel = new GuiRobot;
    robotIcon1Sel = new GuiRobot;
    robotIcon2Sel = new GuiRobot;
    robotIcon3Sel = new GuiRobot;
    robotIcon4Sel = new GuiRobot;
    robotIcon5Sel = new GuiRobot;
    botIconsSelected.push_back(robotIcon0Sel);
    botIconsSelected.push_back(robotIcon1Sel);
    botIconsSelected.push_back(robotIcon2Sel);
    botIconsSelected.push_back(robotIcon3Sel);
    botIconsSelected.push_back(robotIcon4Sel);
    botIconsSelected.push_back(robotIcon5Sel);
    scene_botIconSel_0 = new QGraphicsScene;
    scene_botIconSel_1 = new QGraphicsScene;
    scene_botIconSel_2 = new QGraphicsScene;
    scene_botIconSel_3 = new QGraphicsScene;
    scene_botIconSel_4 = new QGraphicsScene;
    scene_botIconSel_5 = new QGraphicsScene;
    botIconSelScenes.push_back(scene_botIconSel_0);
    botIconSelScenes.push_back(scene_botIconSel_1);
    botIconSelScenes.push_back(scene_botIconSel_2);
    botIconSelScenes.push_back(scene_botIconSel_3);
    botIconSelScenes.push_back(scene_botIconSel_4);
    botIconSelScenes.push_back(scene_botIconSel_5);
    // Ball GUI icon
    ballIcon = new GuiBall();
    scene_ballIcon = new QGraphicsScene;

    // relevant vectors filled & connected; initial details added to GUI
    for (int i=0; i<teamSize; i++) {
        botIcons[i]->id = i;
        botIcons[i]->icon = true;
        botIconsSelected[i]->id = i;
        botIconsSelected[i]->icon = true;
        botIconSelScenes[i]->addItem(botIconsSelected[i]);
        botIconFrames[i]->scale(.2, .2);
        botIconFrames[i]->scale(1,-1);
        botIconFrames[i]->rotate(90);
        botIconScenes[i]->addItem(botIcons[i]);
        botIconFrames[i]->setScene(botIconScenes[i]);
        botIconFrames[i]->hide();
        velocityDials[i]->setValue(0);
        overriddenBots.push_back(false);    // creating each element, and setting to false
        cout << "overriddenBots.size: " << overriddenBots.size() << "\n";

    }
    // putting ball icon into GUI
    scene_ballIcon->addItem(ballIcon);
    ui->gView_ball->setScene(scene_ballIcon);
    ui->gView_ball->scale(.2, .2);
    ui->gView_ball->scale(1, -1);
    ui->gView_ball->hide();

    // Formatting selected bot panel
    ui->gView_robot_prime->scale(.4, .4);
    ui->gView_robot_prime->scale(1,-1);
    ui->gView_robot_prime->rotate(90);

}//setupBotPanel

void MainWindow::updateBotPanel() {
    // Printing current bot info to Robot Panels
    for (int i=0; i<teamSize; i++) {
        botTitle[i]->setText("Robot " + QString::number(i));
        // Nullcheck
        if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
            botXcoords[i]->display(getBotCoordX(true, i));
            botYcoords[i]->display(getBotCoordY(true, i));
            botOrients[i]->setValue(getBotOrientDouble(true, i));
//            if (nxtrobcomm->gui_kick == 1) {
//                printBehavior(i,"KICK!", true);
//            }
            botIcons[i]->setX(0);
            botIcons[i]->setY(0);
            botIcons[i]->setZValue(2);
            if (botIconFrames[i]->isVisible() == false) {
                botIconFrames[i]->show();
            }

            velocityDials[i]->setValue(getVelocity(i));
            printBehavior(i, to_string(getVelocity(i)), true);
//            cout << to_string(getVelocity(0)) << "\n";
            // dynamic velocity dial colors
            if (velocityDials[i]->value() > 0) {            // forward
                velocityDials[i]->setStyleSheet("background-color: rgb(0, 200, 0);");

            } else if (velocityDials[i]->value() < 0) {     // reverse
                velocityDials[i]->setStyleSheet("background-color: rgb(200, 0, 0);");

            } else {                                        // motionless
                velocityDials[i]->setStyleSheet("background-color: rgb(150, 150, 150);");
            }

            botIconFrames[i]->update();
        }//nullcheck
    }
    // Selected Bot Panel velocity dial
    if (ui->dial_botSpeed->value() > 0) {
        ui->dial_botSpeed->setStyleSheet("background-color: rgb(0, 200, 0);");
        ui->lcd_botSpeed->setStyleSheet("background-color: rgb(0, 100, 0);");
    } else if (ui->dial_botSpeed->value() < 0 ) {
        ui->dial_botSpeed->setStyleSheet("background-color: rgb(200, 0, 0);");
        ui->lcd_botSpeed->setStyleSheet("background-color: rgb(100, 0, 0);");
    } else {
        ui->dial_botSpeed->setStyleSheet("background-color: rgb(150, 150, 150);");
        ui->lcd_botSpeed->setStyleSheet("background-color: rgb(100, 100, 100);");
    }
    // Robot override checkbox
    if (selectedBot > -1) {
        ui->check_botOverride->setEnabled(true);
        ui->check_botOverride->show();
        if (overriddenBots[selectedBot] == true) {
            ui->check_botOverride->setChecked(true);
        } else {
            ui->check_botOverride->setChecked(false);
        }
    } else {
        ui->check_botOverride->setEnabled(false);
        ui->check_botOverride->hide();
    }


    // Mouse point
    ui->lcd_coordX_cursor->display(getMouseCoordX());
    ui->lcd_coordY_cursor->display(getMouseCoordY());

}

void MainWindow::scanForSelection() {
    bool newSelection = true;

    // Scanning for double-click selection
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
            if (botIcons[i]->doubleClicked || guiTeam[i]->doubleClicked) {
                botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
                centeredBotID = i;
                centerViewOnBot();
                zoomField(20);
                guiPrint("Focused on Robot " + to_string(centeredBotID));
                break;
            }
        }//nullcheck
    }//end for

    // Scrolling the camera removes centeredOn but not selection
    if (justScrolled) {
        for (int i=0; i<teamSize; i++) {
            if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
                botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
            }//nullcheck
        }
        centeredBotID = -1;
    }
    // Field/Sidelines clicked removes centeredOn and selection
    if (field->Pressed == true) {
        field->highlighted = true;
        field->Pressed = false;
    }
    if (sidelines->Pressed == true) {
        sidelines->highlighted = true;
        sidelines->Pressed = false;
    }
    if (field->highlighted || sidelines->highlighted) {
        for (int i=0; i<teamSize; i++) {
            if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
                guiTeam[i]->highlighted = false;
                guiTeam[i]->setSelected(false);
                botIcons[i]->highlighted = false;
                botIcons[i]->setSelected(false);
                botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
            }//nullcheck
        }
        field->highlighted = false;
        sidelines->highlighted = false;
        selectedBot = -1;
        centeredBotID = -1;
        newSelection = true;
    }

    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
        // Bots on the panel clicked
            if (botIcons[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<teamSize; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    botIcons[j]->highlighted = false;
                    botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;

                }
//                field->highlighted = false;
                botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<teamSize; r++) {
                    botIconFrames[i]->update();
                }
                ui->gView_robot_prime->hide();
                ui->gView_robot_prime->show();
                newSelection = true;
            }
            // Bots on the field clicked
            if (guiTeam[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<teamSize; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    botIcons[j]->highlighted = false;
                    botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;
                }
//                field->highlighted = false;
                botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<teamSize; r++) {
                    botIconFrames[i]->update();
                }
                ui->gView_robot_prime->hide();
                ui->gView_robot_prime->show();
                newSelection = true;
            }
        }//null check
    }//for loop
    if (newSelection) {
        updateSelectedBotPanel(selectedBot);
    } else { return; }

}

int MainWindow::getVelocity(int id) {
    int velocity = 0;
    int wheels = 0;
    int LF = 0;
    int RF = 0;
    int LB = 0;
    int RB = 0;

    if ( gamemodel->find(id, gamemodel->getMyTeam())->type() == fourWheelOmni ) {
        printBehavior(id,"fourWheelOmni",false);
//        if (SIMULATED) {
            LF = gamemodel->find(id, gamemodel->getMyTeam())->getLF();
            RF = gamemodel->find(id, gamemodel->getMyTeam())->getRF();
            LB = gamemodel->find(id, gamemodel->getMyTeam())->getLB();
            RB = gamemodel->find(id, gamemodel->getMyTeam())->getRB();
//        } else {
//            LF = nxtrobcomm->gui_left_front;
//            RF = nxtrobcomm->gui_right_front;
//            LB = nxtrobcomm->gui_left_back;
//            RB = nxtrobcomm->gui_right_back;
//        }
//        cout << "4wheel Robot " << id << ": " << LF << ", " << RF << "\n";
            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
            velocity += LB;
            wheels++;
            velocity += RB;
            wheels++;
    } else if ( gamemodel->find(id, gamemodel->getMyTeam())->type() == differential ) {
        printBehavior(id,"differential",false);
//        if (SIMULATED) {
            LF = gamemodel->find(id, gamemodel->getMyTeam())->getL();
            RF = gamemodel->find(id, gamemodel->getMyTeam())->getR();
//        } else {
//            LF = nxtrobcomm->gui_left;
//            RF = nxtrobcomm->gui_right;
//        }
//        cout << "diff Robot " << id << ": " << LF << ", " << RF << "\n";
            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
    } else if ( gamemodel->find(id, gamemodel->getMyTeam())->type() == threeWheelOmni ) {
        printBehavior(id,"threeWheelOmni",false);
//        if (SIMULATED) {
            LF = gamemodel->find(id, gamemodel->getMyTeam())->getLF();
            RF = gamemodel->find(id, gamemodel->getMyTeam())->getRF();
//        } else {
//            LF = nxtrobcomm->gui_left_front;
//            RF = nxtrobcomm->gui_right_front;
//        }
        int b = gamemodel->find(id, gamemodel->getMyTeam())->getB();
//        cout << "3wheel Robot " << id << ": " << LF << ", " << RF << "\n";

            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
            velocity += b;
            wheels++;
    }

    if (velocity != 0 && wheels != 0)
        velocity /= wheels;

    printBehavior(id,"Velocities: " + to_string(LF) + " & " + to_string(RF), true);
    return velocity;
}

void MainWindow::printBehavior(int botID, string behavior, bool append)
{
    ui->text_primeBot->setTextColor(Qt::white);
    QString b;
    if (append == false) {
        b = QString::fromStdString(behavior) + "\n";
        botBehavior[botID] = b;
//        cout << botBehavior[botID].toStdString();
    } else {
        b = QString::fromStdString(behavior) + "\n";
//        botBehavior[botID] += b;
        botBehavior[botID].insert(0, b);
    }


}

void MainWindow::drawLine(int originX, int originY, int endX, int endY) {
//    guidrawline = new GuiDrawLine();
//    guidrawline->setZValue(3);
//    guidrawline->setX(100);
//    guidrawline->setY(100);
//    scene->addItem(guidrawline);

        guidrawline->x1 = originX;
        guidrawline->y1 = originY;
        guidrawline->x2 = endX;
        guidrawline->y2 = endY;
        ui->gView_field->update();
}

void MainWindow::guiPrint(string output) {
//    guiOutput.insert(0, QString::fromStdString(output));
    ui->text_guiPrint->setTextColor(Qt::white);
    guiOutput += QString::fromStdString(output) + "\n";
    ui->text_guiPrint->setText(guiOutput);
    QScrollBar *sb = ui->text_guiPrint->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::setUpScene()
{
    QShortcut *enter = new QShortcut(this);
    enter->setKey(Qt::Key_Enter);

    QShortcut *spaceBar = new QShortcut(this);
    spaceBar->setKey(Qt::Key_Space);

    QShortcut *o = new QShortcut(this);
    o->setKey(Qt::Key_O);

//    QShortcut *w = new QShortcut(this);
//    w->setKey(Qt::Key_W);
//    QShortcut *a = new QShortcut(this);
//    a->setKey(Qt::Key_A);
//    QShortcut *s = new QShortcut(this);
//    s->setKey(Qt::Key_S);
//    QShortcut *d = new QShortcut(this);
//    d->setKey(Qt::Key_D);

//    QKeyEvent *wPressed = new QKeyEvent(QEvent::KeyPress, Qt::Key_W, 0, "w", false, 1);
//    this->keyPressEvent(wPressed);

//    w->setAutoRepeat(false);

//    connect(w, SIGNAL(activated()), this, SLOT(on_btn_botForward_pressed()));
//    connect(d, SIGNAL(activated()), this, SLOT(on_btn_botTurnRight_pressed()));
//    connect(s, SIGNAL(activated()), this, SLOT(on_btn_botReverse_pressed()));
//    connect(a, SIGNAL(activated()), this, SLOT(on_btn_botTurnLeft_pressed()));


    connect(spaceBar, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
    connect(o, SIGNAL(activated()), ui->check_botOverride, SLOT(click()));

    scene = new GuiScene();

    // Creating the sidelines
    sidelines = new GuiSidelines();
    sidelines->setX(-4000);         // Offsets are 50% of the item's height & width
    sidelines->setY(-3000);
    sidelines->setZValue(0);

    // Creating the field
    field = new GuiField();
    field->setX(-2900);
    field->setY(-1900); // Y seems to be 100 off (?)
    field->setZValue(1);
    field->grid = false;
//    field->highlighted = false;
//    field->Pressed = false;

    // Creating the ball
    ball = new GuiBall;
    ball->setToolTip("Ball");

    //Bot Labels - blue team
    botLabel0 = new GuiBotLabel;
    botLabel1 = new GuiBotLabel;
    botLabel2 = new GuiBotLabel;
    botLabel3 = new GuiBotLabel;
    botLabel4 = new GuiBotLabel;
    botLabel5 = new GuiBotLabel;
    guiLabels.push_back(botLabel0);
    guiLabels.push_back(botLabel1);
    guiLabels.push_back(botLabel2);
    guiLabels.push_back(botLabel3);
    guiLabels.push_back(botLabel4);
    guiLabels.push_back(botLabel5);

    // Bot Labels - yellow team
    botLabel0Y = new GuiBotLabel;
    botLabel1Y = new GuiBotLabel;
    botLabel2Y = new GuiBotLabel;
    botLabel3Y = new GuiBotLabel;
    botLabel4Y = new GuiBotLabel;
    botLabel5Y = new GuiBotLabel;
    guiLabelsY.push_back(botLabel0Y);
    guiLabelsY.push_back(botLabel1Y);
    guiLabelsY.push_back(botLabel2Y);
    guiLabelsY.push_back(botLabel3Y);
    guiLabelsY.push_back(botLabel4Y);
    guiLabelsY.push_back(botLabel5Y);

    // Blue Team Robots
    robot0 = new GuiRobot();
    robot1 = new GuiRobot();
    robot2 = new GuiRobot();
    robot3 = new GuiRobot();
    robot4 = new GuiRobot();
    robot5 = new GuiRobot();
    guiTeam.push_back(robot0);
    guiTeam.push_back(robot1);
    guiTeam.push_back(robot2);
    guiTeam.push_back(robot3);
    guiTeam.push_back(robot4);
    guiTeam.push_back(robot5);

    // Yellow Team
    robot0Y = new GuiRobot();
    robot1Y = new GuiRobot();
    robot2Y = new GuiRobot();
    robot3Y = new GuiRobot();
    robot4Y = new GuiRobot();
    robot5Y = new GuiRobot();
    guiTeamY.push_back(robot0Y);
    guiTeamY.push_back(robot1Y);
    guiTeamY.push_back(robot2Y);
    guiTeamY.push_back(robot3Y);
    guiTeamY.push_back(robot4Y);
    guiTeamY.push_back(robot5Y);

    // Adding the previous gui items to the scene
    scene->addItem(field);
    scene->addItem(botLabel0);
    scene->addItem(botLabel1);
    scene->addItem(botLabel2);
    scene->addItem(botLabel3);
    scene->addItem(botLabel4);
    scene->addItem(botLabel5);
    scene->addItem(botLabel0Y);
    scene->addItem(botLabel1Y);
    scene->addItem(botLabel2Y);
    scene->addItem(botLabel3Y);
    scene->addItem(botLabel4Y);
    scene->addItem(botLabel5Y);
    scene->addItem(robot0);
    scene->addItem(robot1);
    scene->addItem(robot2);
    scene->addItem(robot3);
    scene->addItem(robot4);
    scene->addItem(robot5);
    scene->addItem(robot0Y);
    scene->addItem(robot1Y);
    scene->addItem(robot2Y);
    scene->addItem(robot3Y);
    scene->addItem(robot4Y);
    scene->addItem(robot5Y);
    scene->addItem(sidelines);
    scene->addItem(ball);


    // Setting static details for robots
    for (int i=0; i<teamSize; i++) {
        // Blue team
        guiTeam[i]->id = i;
        guiTeam[i]->setToolTip("Robot " + QString::number(i));
        guiTeam[i]->myTeam = true;
        guiLabels[i]->id = i;
        guiLabels[i]->myTeam = true;
        guiLabels[i]->setScale(2.5);

        // Yellow team
        guiTeamY[i]->id = i;
        guiTeamY[i]->setToolTip("Robot " + QString::number(i));
        guiTeamY[i]->myTeam = false;
        guiLabelsY[i]->id = i;
        guiLabelsY[i]->myTeam = false;
        guiLabelsY[i]->setScale(2.5);
    }//end for

    // Drawing debug line (optional)
//    guidrawline = new GuiDrawLine();
//    guidrawline->setZValue(4);
//    guidrawline->setX(100);
//    guidrawline->setY(100);
//    scene->addItem(guidrawline);

    // Raising the curtains...
    ui->gView_field->setScene(scene);

    // Refreshes graphics view to eliminate glitchiness
    ui->gView_field->hide();
    ui->gView_field->show();

}// setupScene

void MainWindow::updateScene() {
//    ui->gView_field->hide();
    if (refresh) {
        ui->gView_field->hide();
        ui->gView_field->show();
        refresh = false;
    }

    // Grid
    if (ui->check_fieldGrid->isChecked()) {
        field->grid = true;
    }else{ field->grid = false; };
    if (ui->combo_gridScale->currentText() == "200²") {
        field->gridScale = 100;
    } else if (ui->combo_gridScale->currentText() == "500²") {
        field->gridScale = 250;
    } else if (ui->combo_gridScale->currentText() == "1000²") {
        field->gridScale = 500; // 1000x1000 unit sectors
    }
    // Colored Goals
    if (ui->check_coloredGoals->isChecked()) {
        field->coloredGoals = true;
    }else{ field->coloredGoals = false;};

    // Updating field/sideline colors
    sidelines->colorScheme = ui->combo_fieldColor->currentText();
    field->colorScheme = ui->combo_fieldColor->currentText();

    // updating the ball
        ball->setX(getBallCoordX());
        ball->setY(getBallCoordY());
        ball->setZValue(2);
        // Ball Scale
        if (ui->combo_ballScale->currentText() == "100%"){
            ball->setScale(.3);
        } else if (ui->combo_ballScale->currentText() == "120%"){
            ball->setScale(.5);
        } else if (ui->combo_ballScale->currentText() == "150%"){
            ball->setScale(.8);
        }
        ball->color = ui->combo_ballColor->currentText();

        // Tranformation matrix for robot ID labels
        QTransform flipLabel;
        flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);

        // Updating objects in scene
        for (int i=0; i<teamSize; i++) {
            // Blue Team
            if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
                guiTeam[i]->setX(getBotCoordX(true, i));
                guiTeam[i]->setY(getBotCoordY(true, i));
                guiTeam[i]->setZValue(2);
                double angle = getBotOrientDouble(true, i) ;
                guiTeam[i]->setRotation(angle);
                // Action colors (may be better in the button slots)
                if (i != selectedBot) {
                    if (gamemodel->find(i, gamemodel->getMyTeam())->getDrible() ) {
                        guiTeam[i]->dribling = true;
                    } else { guiTeam[i]->dribling = false; }
                    if (gamemodel->find(i, gamemodel->getMyTeam())->getKick() == 1) {
                        guiTeam[i]->kicking = true;
                    } else { guiTeam[i]->kicking = false; }
                }
                // Robot Scale
                if (ui->combo_botScale->currentText() == "100%") {
                    guiTeam[i]->setScale(1);
                } else if (ui->combo_botScale->currentText() == "120%") {
                    guiTeam[i]->setScale(1.2);
                } else if (ui->combo_botScale->currentText() == "150%") {
                    guiTeam[i]->setScale(1.5);
                }
                // labels
                guiLabels[i]->setTransform(flipLabel, false);
                guiLabels[i]->setRotation(currentFieldAngle);
                guiLabels[i]->setZValue(3);
                guiLabels[i]->setX(getBotCoordX(true,i));
                guiLabels[i]->setY(getBotCoordY(true,i));
                if (ui->check_showIDs->isChecked()) {
                    guiLabels[i]->hidden = false;
                }else{
                    guiLabels[i]->hidden = true;
                }
            }
            // Yellow Team
            if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
                guiTeamY[i]->setX(getBotCoordX(false, i));
                guiTeamY[i]->setY(getBotCoordY(false, i));
                guiTeamY[i]->setZValue(2);
                double angleY = getBotOrientDouble(false, i) ;
                guiTeamY[i]->setRotation(angleY);
                // Robot Scale
                if (ui->combo_botScale->currentText() == "100%") {
                    guiTeamY[i]->setScale(1);
                } else if (ui->combo_botScale->currentText() == "120%") {
                    guiTeamY[i]->setScale(1.2);
                } else if (ui->combo_botScale->currentText() == "150%") {
                    guiTeamY[i]->setScale(1.5);
                }
                // labels
                guiLabelsY[i]->setTransform(flipLabel,false);
                guiLabelsY[i]->setRotation(currentFieldAngle);
                guiLabelsY[i]->setZValue(3);
                guiLabelsY[i]->setX(getBotCoordX(false,i));
                guiLabelsY[i]->setY(getBotCoordY(false,i));
                if (ui->check_showIDs->isChecked()) {
                    guiLabelsY[i]->hidden = false;
                }else{
                    guiLabelsY[i]->hidden = true;
                }
            }
        }

    // updating Blue Labels
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
//            QTransform flipLabel;
//            flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);
//            guiLabels[i]->setTransform(flipLabel, false);
//            guiLabels[i]->setRotation(currentFieldAngle);
//            guiLabels[i]->setZValue(3);
//            guiLabels[i]->setX(getBotCoordX(true,i));
//            guiLabels[i]->setY(getBotCoordY(true,i));
//            if (ui->check_showIDs->isChecked()) {
//                guiLabels[i]->hidden = false;
//            }else{
//                guiLabels[i]->hidden = true;
//            }
        }
    }
    // updating Yellow Labels
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
//            QTransform flipLabel;
//            flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);
//            guiLabelsY[i]->setTransform(flipLabel,false);
//            guiLabelsY[i]->setRotation(currentFieldAngle);
//            guiLabelsY[i]->setZValue(3);
//            guiLabelsY[i]->setX(getBotCoordX(false,i));
//            guiLabelsY[i]->setY(getBotCoordY(false,i));
//            if (ui->check_showIDs->isChecked()) {
//                guiLabelsY[i]->hidden = false;
//            }else{
//                guiLabelsY[i]->hidden = true;
//            }
        }
    }
    // Bot moving TEST
//    Point botPoint;
//    botPoint.x = 0;
//    botPoint.y = 0;
//    gamemodel->find(0,gamemodel->getMyTeam())->setRobotPosition(botPoint);
    // updating Blue Robots
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
//            guiTeam[i]->setX(getBotCoordX(true, i));
//            guiTeam[i]->setY(getBotCoordY(true, i));
//            guiTeam[i]->setZValue(2);
//            double angle = getBotOrientDouble(true, i) ;
//            guiTeam[i]->setRotation(angle);
//            // Action colors (may be better in the button slots)
//            if (i != selectedBot) {
//                if (gamemodel->find(i, gamemodel->getMyTeam())->getDrible() ) {
//                    guiTeam[i]->dribling = true;
//                } else { guiTeam[i]->dribling = false; }
//                if (gamemodel->find(i, gamemodel->getMyTeam())->getKick() == 1) {
//                    guiTeam[i]->kicking = true;
//                } else { guiTeam[i]->kicking = false; }
//            }
//            // Robot Scale
//            if (ui->combo_botScale->currentText() == "100%") {
//                guiTeam[i]->setScale(1);
//            } else if (ui->combo_botScale->currentText() == "120%") {
//                guiTeam[i]->setScale(1.2);
//            } else if (ui->combo_botScale->currentText() == "150%") {
//                guiTeam[i]->setScale(1.5);
//            }


        }
    }
    // updating Yellow Robots
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
//            guiTeamY[i]->setX(getBotCoordX(false, i));
//            guiTeamY[i]->setY(getBotCoordY(false, i));
//            guiTeamY[i]->setZValue(2);
//            double angleY = getBotOrientDouble(false, i) ;
//            guiTeamY[i]->setRotation(angleY);
//            // Robot Scale
//            if (ui->combo_botScale->currentText() == "100%") {
//                guiTeamY[i]->setScale(1);
//            } else if (ui->combo_botScale->currentText() == "120%") {
//                guiTeamY[i]->setScale(1.2);
//            } else if (ui->combo_botScale->currentText() == "150%") {
//                guiTeamY[i]->setScale(1.5);
//            }
        }
    }

    if (ball->isSelected()) {
        cout << "Ball selected \n";
    }
    // Keeping camera centered
    centerViewOnBot();
    // drawLine TEST
//    drawLine(getBotCoordX(true, 0),getBotCoordY(true, 0), 0, 0 );
//    ui->gView_field->update();

}//end updateScene

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "Connect")
    {
        ui->pushButton->setText("Disconnect");
        for(int i = 0; i < threads.count(); i++)
            threads[i]->start();
    }
    else
    {
        ui->pushButton->setText("Connect");
        for(int i = 0; i < threads.count(); i++)
            threads[i]->exit(0);
    }
//    cout << "on_pushButton_clicked() \n";
}

QString MainWindow::getBotCoord(int id) {
    QString qPos    = "no connection";
    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (team.at(0) != NULL){
        std::string posRob = gamemodel->find(id, team)->getRobotPosition().toString();
        qPos = QString::fromStdString(posRob);
    }
    return qPos;

}

int MainWindow::getBotCoordX(bool myTeam, int id) {
    int x  = 0.00;
    std::vector<Robot*> team;
    if (myTeam) {
        team = gamemodel->getMyTeam();
    } else {
        team = gamemodel->getOponentTeam();
    }

//    if (gamemodel->find(id, team) != NULL) {
        x = gamemodel->find(id, team)->getRobotPosition().x;
//    }
    //cout << x << " \n";
    return x;
}

int MainWindow::getBotCoordY(bool myTeam, int id) {
    int y  = 0000;
    std::vector<Robot*> team;
    if (myTeam) {
        team = gamemodel->getMyTeam();
    } else {
        team = gamemodel->getOponentTeam();
    }

//    if (team.at(id) != NULL) {
        y = gamemodel->find(id, team)->getRobotPosition().y;
//    }
    //cout << y << " \n";
    return y;
}

QString MainWindow::getBotOrientString(int id) {
    QString qOrient = "no connection";
    std::vector<Robot*> team = gamemodel->getMyTeam();
    std::string sOrient;
    double  dRads = 8888;
    int     iRads = 8888;
//    if (team.at(0) != NULL){
        dRads = gamemodel->find(id, team)->getOrientation(); // angle in radians
        dRads *= (180/M_PI);
        iRads = dRads;
        sOrient = std::to_string(iRads);
        qOrient = QString::fromStdString(sOrient);
//    }//end if
    return qOrient;

}

double MainWindow::getBotOrientDouble(bool myTeam, int id) {
    double o  = 8888;
    std::vector<Robot*> team;
    if (myTeam) {
        team = gamemodel->getMyTeam();
    } else {
        team = gamemodel->getOponentTeam();
    }

//    if (team.at(id) != NULL) {
        o = gamemodel->find(id, team)->getOrientation();
        o *= (180/M_PI);
//    }
    //cout << y << " \n";
    return o;
}

QImage MainWindow::getStatusImg(int id) {

    // Return value
    QImage botStatusPic;
    botStatusPic.load(":/images/ball.png");
    // Default icons
    QImage bot0_default;
    bot0_default.load(":/images/0.png");
    QImage bot1_default;
    bot1_default.load(":/images/1.png");
    QImage bot2_default;
    bot2_default.load(":/images/2.png");
    QImage bot3_default;
    bot3_default.load(":/images/3.png");
    QImage bot4_default;
    bot4_default.load(":/images/4.png");
    QImage bot5_default;
    bot5_default.load(":/images/5.png");
    // Default icons vector
    std::vector<QImage> bot_defaults(teamSize);
    bot_defaults[0] = bot0_default;
    bot_defaults[1] = bot1_default;
    bot_defaults[2] = bot2_default;
    bot_defaults[3] = bot3_default;
    bot_defaults[4] = bot4_default;
    bot_defaults[5] = bot5_default;
    // Has Ball icon
    QImage hasBall;
    hasBall.load(":/images/ball.png");


    std::vector<Robot*> team = gamemodel->getMyTeam();


//    if (gamemodel->find(id, team)->hasBall) {
//        //cout << "hasBall TRUE \n";
//        botStatusPic = hasBall;
//    }
//    if (gamemodel->find(id, team)->drible) {
//        botStatusPic = hasBall;
//    }
//    if ( gamemodel->find(id, team) != NULL ) {
        //cout << id << " hasBeh TRUE \n";
        botStatusPic = bot_defaults[id];
//    } else {
//        //cout << id << " hasBeh FALSE \n";
//        botStatusPic.load(":/images/process-stop.png");
//    }
//    cout << id << ": " << gamemodel->find(id, team)->getCurrentBeh() << "\n";
    return botStatusPic;
}

int MainWindow::getBotSpeed(std::vector<QLabel*> c, int id) {
    int s = 0;
    return s;
}

bool MainWindow::hasChangedQString(QLabel* l, QString s) {
    QString old = l->text();

    if (old == s) {
        return false;
    } else {
        return true;
    }
}

bool MainWindow::hasChangedQImage(QLabel* l, QImage s) {
    QImage old = l->pixmap()->toImage();

    if (old == s) {
        cout << "same image - NO CHANGE \n";
        return false;
    } else {
        cout << "diff image - CHANGING \n";
        return true;
    }
}

QString MainWindow::getBallCoord() {
    QString b;  // return value
    std::string posBallXY = gamemodel->getBallPoint().toString();
    b = QString::fromStdString(posBallXY);

    return b;
}

int MainWindow::getBallCoordX() {
    int b;
    b = gamemodel->getBallPoint().x;
//    cout << "ball X: " << b << "\n";

    return b;
}

int MainWindow::getBallCoordY() {
    int b;
    b = gamemodel->getBallPoint().y;
    return b;
}


QString MainWindow::getRemTime() {
    QString t;  // return value
    std::string time = std::to_string(gamemodel->getRemainingTime());
    t = QString::fromStdString(time);

    return t;
}


void MainWindow::updateBallInfo() {
    ui->lcd_coordX_ball->display(getBallCoordX());
    ui->lcd_coordY_ball->display(getBallCoordY());

    ballIcon->color = ui->combo_ballColor->currentText();
    ui->gView_ball->update();
    // Displaying ball icon
    if (ui->gView_ball->isHidden()) {
        ui->gView_ball->show();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int rAngle = -45;
    ui->gView_field->rotate(rAngle);
    currentFieldAngle += rAngle;
}

void MainWindow::on_pushButton_3_clicked()
{
    int lAngle = 45;
    ui->gView_field->rotate(lAngle);
    currentFieldAngle += lAngle;
}

int MainWindow::frequency_of_primes (int n) {
  int i,j;
  int freq=n-1;
  for (i=2; i<=n; ++i) for (j=sqrt(i);j>1;--j) if (i%j==0) {--freq; break;}
  return freq;
}

int MainWindow::getClock()
{
    time_t now;
     struct tm newyear;
     double seconds;

     time(&now);  /* get current time; same as: now = time(NULL)  */

     newyear = *localtime(&now);

     newyear.tm_hour = 0; newyear.tm_min = 0; newyear.tm_sec = 0;
     newyear.tm_mon = 0;  newyear.tm_mday = 1;

     seconds = difftime(now,mktime(&newyear));

     printf ("%.f seconds since new year in the current timezone.\n", seconds);

     return 0;
}



int MainWindow::getSpeed(QGraphicsItem *p, double o)
{
    // Worked, then mysteriously started crashing at the second line (p reference)
    int speed = 0;
    double currentPos = p->y() / p->x();
    speed = currentPos - o;
    o = currentPos;
    speed *= 10;
    if (speed < 0) {
        speed *= -1;
    }
//    cout << "ball speed: " << speed << "\n";
    return speed;

}

void MainWindow::moveBot() {

}

void MainWindow::keyPressEvent(QKeyEvent *event) {
//    if (event->count() == 1) {
//        keys = "one";
        switch(event->key()) {
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

            case Qt::Key_K:
                on_btn_botKick_pressed();
                break;
            case Qt::Key_J:
                on_btn_botDrible_pressed();
                break;

        }
        //    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {

    switch(event->key()) {
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

        case Qt::Key_K:
            on_btn_botKick_released();
            break;
        case Qt::Key_J:
            on_btn_botDrible_released();
            break;
    }
}

int MainWindow::getMouseCoordX() {
    int x = scene->mousePoint.x()-100;
    return x;
}

int MainWindow::getMouseCoordY() {
    int y = scene->mousePoint.y()-100;
    return y;
}

void MainWindow::centerViewOnBot() {
    // Centering camera on double-clicked bot
    if (centeredBotID > -1) {
        ui->gView_field->centerOn(guiTeam[centeredBotID]);

    }
}

void MainWindow::setMyVelocity() {
    if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true) {
        myVelocity = 100;
    } else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true) {
        myVelocity = 25;
    } else {
        myVelocity = 50;
    }
}

void MainWindow::setGuiOverride() {
    if (ui->check_botOverride->isChecked()) {
        guiOverride = true;
    } else {
        guiOverride = false;
    }

}

MainWindow::~MainWindow()
{
    //delete ui;
    while(threads.count() > 0)
    {
        QThread* thread = threads.takeFirst();
        if(thread->isRunning())
            thread->exit(0);
    }
}

MainWindow *MainWindow::getWindow()
{
//    if (window == NULL)
//        window = new MainWindow();

//    return window;

}

MainWindow *MainWindow::getMainWindow() {
    if (mw == NULL) {
        mw = new MainWindow();
    }

    return mw;
}




// Field graphical settins which need to be refreshed when changed
void MainWindow::on_check_fieldGrid_clicked(){refresh = true;}
void MainWindow::on_combo_gridScale_currentIndexChanged(int index){refresh = true;}
void MainWindow::on_check_coloredGoals_clicked(){refresh = true;}
void MainWindow::on_combo_fieldColor_currentIndexChanged(int index){refresh = true;}
void MainWindow::on_check_showIDs_stateChanged(int arg1){refresh = true;}
void MainWindow::on_combo_botScale_currentIndexChanged(int index){refresh = true;}

void MainWindow::field_setDragMode() {
//    if (ui->gView_field->dragMode() == QGraphicsView::NoDrag) {
//        ui->gView_field->setDragMode(QGraphicsView::ScrollHandDrag);
//    } else {
//        ui->gView_field->setDragMode(QGraphicsView::NoDrag);
//    }
}

void MainWindow::updateSelectedBotPanel(int id)
{
//    cout << "updatedSelectedBotPanel; value: " << id << "\n";
    int v = 0;
    if (id == -1) {
        ui->gView_robot_prime->hide();
        ui->dial_botSpeed->setValue(0);
        ui->lcd_botSpeed->display(0);
        ui->lcd_orient_prime->display("0");
        ui->lcd_coordX_prime->display("0");
        ui->lcd_coordY_prime->display("0");
        ui->dial_botOrient_prime->setValue(0);
        ui->box_primeBot->setTitle(" ");
        ui->text_primeBot->setText(" ");
//        for (int i=0; i<teamSize; i++) {
//            if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
//                if (botFrames[i]->isHidden()) {
//                    botFrames[i]->show();
//                }
//            }
//        }

    } else {
        v = getVelocity(id);
        ui->gView_robot_prime->show();
        ui->dial_botSpeed->setValue(v);
        ui->lcd_botSpeed->display(v);
        ui->lcd_orient_prime->display(getBotOrientString(id));
        ui->lcd_coordX_prime->display(getBotCoordX(true, id));
        ui->lcd_coordY_prime->display(getBotCoordY(true,id));
        ui->dial_botOrient_prime->setValue(getBotOrientDouble(true, id));
        ui->box_primeBot->setTitle("Robot " + QString::number(id));
        if (botBehavior[id] != ui->text_primeBot->toPlainText()) {
            ui->text_primeBot->setText(botBehavior[id]);
        }
//        QString test = ui->dial_botOrient_prime->styleSheet();
//        if (ui->text_primeBot->toPlainText() != test ){
//            ui->text_primeBot->setText(test);
//        }
//        ui->dial_botOrient_prime->setStyleSheet();
//        gamemodel->find(id,gamemodel->getMyTeam())->getki
//        for (int i=0; i<teamSize; i++) {
//            if (gamemodel->find(id, gamemodel->getMyTeam()) != NULL) {
//                if (botFrames[i]->isHidden()) {
//                    botFrames[i]->show();
//                }
//            }
//        }
//        if (botFrames[id]->isHidden() == false) {
//            botFrames[id]->hide();
//        }
        ui->gView_robot_prime->setScene(botIconSelScenes[id]);
    }

}

void MainWindow::on_btn_botForward_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {

        ui->btn_botForward->setDown(true);

        int currentFwd = getVelocity(selectedBot);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentFwd);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentFwd);
//        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setB(currentFwd);
//        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setLF(currentFwd);
//        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setLB(currentFwd);
//        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setRF(currentFwd);
//        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setRB(currentFwd);
        if (currentFwd <= 0) {
            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentFwd+myVelocity);
            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentFwd+myVelocity);
//            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setLF(currentFwd+50);
//            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setLB(currentFwd+50);
//            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setRF(currentFwd+50);
//            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setRB(currentFwd+50);
        }
//        cout << "avg velocity: " << getVelocity(selectedBot) << "\n";
//        cout << "left velocity: " << gamemodel->find(selectedBot, gamemodel->getMyTeam())->getL() << "\n";
//        cout << "right velocity: "<< gamemodel->find(selectedBot, gamemodel->getMyTeam())->getR() << "\n";

    }
}

void MainWindow::on_btn_botForward_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botForward->setDown(false);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(0);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(0);
    }
}

void MainWindow::on_btn_botTurnRight_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(true);
        int currentVel = getVelocity(selectedBot);
        float currentL = gamemodel->find(selectedBot, gamemodel->getMyTeam())->getL();
        float currentR = gamemodel->find(selectedBot, gamemodel->getMyTeam())->getR();
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentL+myVelocity/2);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentR-myVelocity/2);
    }
}

void MainWindow::on_btn_botTurnRight_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnRight->setDown(false);
        float currentFwd = getVelocity(selectedBot);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentFwd);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentFwd);
    }
}

void MainWindow::on_btn_botTurnLeft_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(true);
        int currentVel = getVelocity(selectedBot);
        int currentL = gamemodel->find(selectedBot, gamemodel->getMyTeam())->getL();
        int currentR = gamemodel->find(selectedBot, gamemodel->getMyTeam())->getR();
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentL-myVelocity/2);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentR+myVelocity/2);
    }
}

void MainWindow::on_btn_botTurnLeft_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botTurnLeft->setDown(false);
        float currentVel = getVelocity(selectedBot);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentVel);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentVel);
    }
}

void MainWindow::on_btn_botReverse_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(true);
        int currentVel = getVelocity(selectedBot);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentVel);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentVel);
        if (currentVel >= 0) {
            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(currentVel-myVelocity);
            gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(currentVel-myVelocity);
        }
    }
}

void MainWindow::on_btn_botReverse_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botReverse->setDown(false);
        int currentVel = getVelocity(selectedBot);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(0);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(0);
    }
}

void MainWindow::on_btn_botKick_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(true);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setKick(true);
        guiTeam[selectedBot]->kicking = true;
    }
}

void MainWindow::on_btn_botKick_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botKick->setDown(false);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setKick(false);
        guiTeam[selectedBot]->kicking = false;
    }
}

void MainWindow::on_btn_botDrible_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(true);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setDrible(true);
        guiTeam[selectedBot]->dribling = true;
    }
}

void MainWindow::on_btn_botDrible_released() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {
        ui->btn_botDrible->setDown(false);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setDrible(false);
        guiTeam[selectedBot]->dribling = false;
    }
}


void MainWindow::on_check_botOverride_clicked(bool checked) {
    if (checked) {
        overriddenBots[selectedBot] = true;
    } else {
        overriddenBots[selectedBot] = false;
    }
}

void MainWindow::on_check_botOverride_all_clicked(bool checked) {
    if (checked) {  // overrides all bots' software control
        for (int i=0; i<overriddenBots.size(); i++) {
            overriddenBots[i] = true;
        }
    } else {    // returns software control to all bots
        for (int i=0; i<overriddenBots.size(); i++) {
            overriddenBots[i] = false;
        }

    }
}
