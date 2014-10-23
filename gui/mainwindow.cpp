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
#include "robotpanel.h"

// Global static pointer used to ensure only a single instance of the class.
MainWindow* MainWindow::mw = NULL;

using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    gamemodel = GameModel::getModel();

    // Setting up GUI; not enabling thread until we're done
    ui->btn_connectGui->setEnabled(false);
    setUpScene();
    defaultZoom();
    robotPanel = new RobotPanel();
    robotPanel->setupBotPanel(this);
    setupKeyShortcuts();
    ui->btn_connectGui->setEnabled(true);


    // Create Threads, the parameters are the timer value, and parent.
    // threads is declated as QList<GuiComm*> threads;
    // create threads, and append them to the threads list, so that
    // threads can be accessed for making connections, and to start
    // and stop threads
    threads.append(new GuiComm(30, this));
//    threads.append(new GuiComm(30, this));

    // Connect each Widget to correcponding thread
    connect(threads[0], SIGNAL(valueChanged(int))
            , this, SLOT(launch(int)));

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


//    ui->label->setText(QString("Current Thread Processing Status : %1").arg(value));

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
    setMyVelocity();
    setGuiOverride();
    updateScene();
    robotPanel->updateBotPanel(this);
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


void MainWindow::scanForSelection() {
    bool newSelection = true;

    // Scanning for double-click selection
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
            if (robotPanel->botIcons[i]->doubleClicked || guiTeam[i]->doubleClicked) {
                robotPanel->botIcons[i]->doubleClicked = false;
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
                robotPanel->botIcons[i]->doubleClicked = false;
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
                robotPanel->botIcons[i]->highlighted = false;
                robotPanel->botIcons[i]->setSelected(false);
                robotPanel->botIcons[i]->doubleClicked = false;
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
            if (robotPanel->botIcons[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<teamSize; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    robotPanel->botIcons[j]->highlighted = false;
                    robotPanel->botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;

                }
//                field->highlighted = false;
                robotPanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<teamSize; r++) {
                    robotPanel->botIconFrames[i]->update();
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
                    robotPanel->botIcons[j]->highlighted = false;
                    robotPanel->botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;
                }
//                field->highlighted = false;
                robotPanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<teamSize; r++) {
                    robotPanel->botIconFrames[i]->update();
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

    printBehavior(id,"Wheels: " + to_string(LF) + " & " + to_string(RF), true);
    return velocity;
}

void MainWindow::printBehavior(int botID, string behavior, bool append)
{
//    ui->text_primeBot->setTextColor(Qt::white);
//    QString b;
//    if (append == false) {
//        b = QString::fromStdString(behavior) + "\n";
//        botBehavior[botID] = b;
////        cout << botBehavior[botID].toStdString();
//    } else {
//        b = QString::fromStdString(behavior) + "\n";
////        botBehavior[botID] += b;
//        botBehavior[botID].append(b);
//    }


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

void MainWindow::guiPrintRobot(int robotID, string output) {
//    guiOutput.insert(0, QString::fromStdString(output));
    ui->text_primeBot->setTextColor(Qt::white);
    if (guiOutputRobot.toStdString() == output) {

    } else {
        // recording this string
        guiOutputRobot = QString::fromStdString(output);
        // converting received string to QString for printing
        QString msg = QString::fromStdString(output);
        botBehavior[robotID].append(msg);
//        QString msg = QString::fromStdString(output);
//        ui->text_primeBot->append(msg);
//        // Scrolling to bottom of text box
//        QScrollBar *sb = ui->text_primeBot->verticalScrollBar();
//        sb->setValue(sb->maximum());
    }
}


void MainWindow::setUpScene()
{

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
                guiTeam[i]->setZValue(3);
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
                guiLabels[i]->setZValue(4);
                guiLabels[i]->setX(getBotCoordX(true,i));
                guiLabels[i]->setY(getBotCoordY(true,i));
                if (ui->check_showIDs->isChecked()) {
                    guiLabels[i]->hidden = false;
                }else{
                    guiLabels[i]->hidden = true;
                }
                // drawLine TEST
//                drawLine(getBotCoordX(true, 0),getBotCoordY(true, 0), Movement::Move.pathQueue[i].x, Movement::Move.pathQueue[i].y );
//                ui->gView_field->update();

            }
            // Yellow Team
            if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
                guiTeamY[i]->setX(getBotCoordX(false, i));
                guiTeamY[i]->setY(getBotCoordY(false, i));
                guiTeamY[i]->setZValue(3);
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
                guiLabelsY[i]->setZValue(4);
                guiLabelsY[i]->setX(getBotCoordX(false,i));
                guiLabelsY[i]->setY(getBotCoordY(false,i));
                if (ui->check_showIDs->isChecked()) {
                    guiLabelsY[i]->hidden = false;
                }else{
                    guiLabelsY[i]->hidden = true;
                }
            }
        }

    // Keeping camera centered
    centerViewOnBot();

}//end updateScene

void MainWindow::on_btn_connectGui_clicked()
{
    if(ui->btn_connectGui->text() == "Connect")
    {
        ui->btn_connectGui->setText("Disconnect");
        for(int i = 0; i < threads.count(); i++)
            threads[i]->start();
    }
    else
    {
        ui->btn_connectGui->setText("Connect");
        for(int i = 0; i < threads.count(); i++)
            threads[i]->exit(0);
    }
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
    x = gamemodel->find(id, team)->getRobotPosition().x;
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
    y = gamemodel->find(id, team)->getRobotPosition().y;
    return y;
}

QString MainWindow::getBotOrientString(int id) {
    QString qOrient = "no connection";
    std::vector<Robot*> team = gamemodel->getMyTeam();
    std::string sOrient;
    double  dRads = 8888;
    int     iRads = 8888;
    dRads = gamemodel->find(id, team)->getOrientation(); // angle in radians
    dRads *= (180/M_PI);
    iRads = dRads;
    sOrient = std::to_string(iRads);
    qOrient = QString::fromStdString(sOrient);
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
    o = gamemodel->find(id, team)->getOrientation();
    o *= (180/M_PI);
    return o;
}


int MainWindow::getBotSpeed(std::vector<QLabel*> c, int id) {
    int s = 0;
    return s;
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

    robotPanel->ballIcon->color = ui->combo_ballColor->currentText();
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
    // Robot binds
    if (selectedBot > -1) {
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
            // Hide/show bot icon
//            case Qt::Key_Delete:
//                cout << "delete \n";
//                break;
//            case Qt::Key_0:
//                selectedBot = 0;
//                break;
//            case Qt::Key_1:
//                robot1->setSelected(true);
//                break;
            // Joystick test

        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    // Robot binds
    if (selectedBot > -1) {
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
//        cout << "Shift \n";
        myVelocity = 100;
    } else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true) {
        cout << "Alt \n";
        myVelocity = 25;
    } else {
        myVelocity = 50;
    }
}

void MainWindow::setGuiOverride() {
    // Required for Override to work with Vision
    if (ui->check_botOverride->isChecked()) {
        guiOverride = true;
    } else {
        guiOverride = false;
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

//    QShortcut *spaceBar = new QShortcut(this);
//    spaceBar->setKey(Qt::Key_Space);

    // Connecting key signals to their respective slots
    connect(enter, SIGNAL(activated()), this, SLOT(on_btn_connectGui_clicked()));
    connect(backspace, SIGNAL(activated()), this, SLOT(on_btn_connectGui_clicked()));
    connect(o, SIGNAL(activated()), ui->check_botOverride, SLOT(click()));
    connect(delKey, SIGNAL(activated()), this, SLOT(toggleIconVisible()));
    //    connect(delKey, SIGNAL(activated()), this, SLOT(on_btn_connectGui_clicked()));
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
        ui->text_primeBot->setText(botBehavior[id]);
        QScrollBar *sb = ui->text_primeBot->verticalScrollBar();
        sb->setValue(sb->maximum());
        ui->gView_robot_prime->setScene(robotPanel->botIconSelScenes[id]);
    }

}

void MainWindow::on_btn_botForward_pressed() {
    if (selectedBot > -1 && ui->check_botOverride->isChecked()) {

        ui->btn_botForward->setDown(true);
        setMyVelocity();
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
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setL(0);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setR(0);
        gamemodel->find(selectedBot, gamemodel->getMyTeam())->setB(0);
    } else {
        overriddenBots[selectedBot] = false;
    }
}

void MainWindow::on_btn_override_all_released() {
    for (int i=0; i<overriddenBots.size(); i++) {
        overriddenBots[i] = true;
    }
    // stopping all bots, so they don't fly off at their current velocities
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
            gamemodel->find(i, gamemodel->getMyTeam())->setL(0);
            gamemodel->find(i, gamemodel->getMyTeam())->setR(0);
            gamemodel->find(i, gamemodel->getMyTeam())->setB(0);
        }//nullcheck
    }
}

void MainWindow::on_btn_override_none_released() {
    for (int i=0; i<overriddenBots.size(); i++) {
        overriddenBots[i] = false;
    }
}

void MainWindow::toggleIconVisible()
{
    if (selectedBot > -1) {
        if (guiTeam[selectedBot]->enabled) {
            guiTeam[selectedBot]->enabled = false;
            robotPanel->botIcons[selectedBot]->enabled = false;
            robotPanel->botIcons[selectedBot]->setOpacity(.3);
            guiTeam[selectedBot]->setOpacity(.3);
        } else {
            guiTeam[selectedBot]->enabled = true;
            robotPanel->botIcons[selectedBot]->enabled = true;
            robotPanel->botIcons[selectedBot]->setOpacity(1);
            guiTeam[selectedBot]->setOpacity(1);

        }
    }
}
