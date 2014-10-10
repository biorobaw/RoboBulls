#define _USE_MATH_DEFINES
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
// QGraphicsView
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    gamemodel = GameModel::getModel();

    setUpScene();

    defaultZoom();

    // Create Threads, the parameters are the timer value, and parent.
    // threads is declated as QList<GuiComm*> threads;
    // create threads, and append them to the threads list, so that
    // threads can be accessed for making connections, and to start
    // and stop threads
    threads.append(new GuiComm(50, this));
//    threads.append(new GuiComm(30, this));
//    threads.append(new GuiComm(30, this));
//    threads.append(new GuiComm(40, this));
//    threads.append(new GuiComm(50, this));
//    threads.append(new GuiComm(60, this));

    // Connect each Widget to correcponding thread
//    connect(threads[0], SIGNAL(valueChanged(int))
//            , this, SLOT(test(int)));

    connect(threads[0], SIGNAL(valueChanged(int))
            , this, SLOT(launch(int)));

//    connect(threads[1], SIGNAL(valueChanged(int))
//            , this, SLOT(updateScene()));

//    connect(threads[2], SIGNAL(valueChanged(int))
//            , this, SLOT(updateFieldBots()));

//    connect(threads[3], SIGNAL(valueChanged(int))
//            , ui->lcdNumber, SLOT(display(int)));

//    connect(threads[4], SIGNAL(valueChanged(int))
//            , ui->horizontalSlider, SLOT(setValue(int)));

//    connect(threads[5], SIGNAL(valueChanged(int))
//            , ui->verticalSlider, SLOT(setValue(int)));

    connect(ui->zoom_slider, SIGNAL(valueChanged(int))
            , this, SLOT(zoomField(int)));

    connect(ui->zoom_default, SIGNAL(clicked())
            , this, SLOT(defaultZoom()));


//    connect(ui->slider_fieldRotate, SIGNAL(valueChanged(int))
//            , this, SLOT(rotateField(int)));

//    connect(ui->pushButton, SIGNAL(clicked())
//            , this, SLOT(setup()));

}
void MainWindow::launch(int value)
{
    ui->label->setText(QString("Current Thread Processing Status : %1").arg(value));
//    gameModelUpdated();
    updateScene();
    printBots();
    printBall();
}

void MainWindow::zoomField(int zoom) {
    double zoomScale = zoom *.01;
    ui->gView_field->setTransform(QTransform::fromScale(zoomScale, zoomScale));
    ui->gView_field->scale(1, -1);
    ui->gView_field->rotate(currentFieldAngle);
}


void MainWindow::defaultZoom() {
    currentFieldAngle = 0;
    zoomField(11);
    ui->zoom_slider->setValue(11);
//    ui->gView_field->hide();
    ui->gView_field->centerOn(sidelines);
}

void MainWindow::updateScene() {
    ui->gView_field->hide();
    ui->gView_field->show();

    // Grid
    if (ui->check_fieldGrid->isChecked()) {
        field->grid = true;
    }else{ field->grid = false; };
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
    // Updating blue labels
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
            QTransform flipLabel;
            flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);
            guiLabels[i]->setTransform(flipLabel, false);
            guiLabels[i]->setRotation(currentFieldAngle);
            guiLabels[i]->setZValue(3);
            guiLabels[i]->setX(getBotCoordX(true,i));
            guiLabels[i]->setY(getBotCoordY(true,i));
            guiLabels[i]->id = i;
            if (ui->check_showIDs->isChecked()) {
                guiLabels[i]->hidden = false;
            }else{
                guiLabels[i]->hidden = true;
            }
            guiLabels[i]->myTeam = true;
            guiLabels[i]->setScale(2.5);
        }
    }

    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
            QTransform flipLabel;
            flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);
            guiLabelsY[i]->setTransform(flipLabel,false);
            guiLabelsY[i]->setRotation(currentFieldAngle);
            guiLabelsY[i]->setZValue(3);
            guiLabelsY[i]->setX(getBotCoordX(false,i));
            guiLabelsY[i]->setY(getBotCoordY(false,i));
            guiLabelsY[i]->id = i;
            if (ui->check_showIDs->isChecked()) {
                guiLabelsY[i]->hidden = false;
            }else{
                guiLabelsY[i]->hidden = true;
            }
            guiLabelsY[i]->myTeam = false;
            guiLabelsY[i]->setScale(2.5);
        }
    }

    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getMyTeam()) != NULL) {
            guiTeam[i]->setX(getBotCoordX(true, i));
            guiTeam[i]->setY(getBotCoordY(true, i));
            double angle = getBotOrientDouble(true, i) ;
            guiTeam[i]->setRotation(angle);
            guiTeam[i]->setZValue(2);
            guiTeam[i]->setScale(1.2);
            guiTeam[i]->id = i;
            guiTeam[i]->setToolTip("Robot " + QString::number(i));
            guiTeam[i]->myTeam = true;
            // Robot Scale
            if (ui->combo_botScale->currentText() == "100%") {
                guiTeam[i]->setScale(1);
            } else if (ui->combo_botScale->currentText() == "120%") {
                guiTeam[i]->setScale(1.2);
            } else if (ui->combo_botScale->currentText() == "150%") {
                guiTeam[i]->setScale(1.5);
            }
        }
    }

    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i, gamemodel->getOponentTeam()) != NULL) {
            guiTeamY[i]->setX(getBotCoordX(false, i));
            guiTeamY[i]->setY(getBotCoordY(false, i));
            double angleY = getBotOrientDouble(false, i) ;
            guiTeamY[i]->setRotation(angleY);
            guiTeamY[i]->setZValue(2);
            guiTeamY[i]->setScale(1.2);
            guiTeamY[i]->id = i;
            guiTeamY[i]->setToolTip("Robot " + QString::number(i));
            guiTeamY[i]->myTeam = false;
            // Robot Scale
            if (ui->combo_botScale->currentText() == "100%") {
                guiTeamY[i]->setScale(1);
            } else if (ui->combo_botScale->currentText() == "120%") {
                guiTeamY[i]->setScale(1.2);
            } else if (ui->combo_botScale->currentText() == "150%") {
                guiTeamY[i]->setScale(1.5);
            }
        }
    }
}//end updateScene

void MainWindow::setUpScene()
{
    scene = new QGraphicsScene(this);

    // Creating the sidelines
    sidelines = new GuiSidelines();
    sidelines->setX(-4000);         // Offsets are 50% of the item's height & width
    sidelines->setY(-3000);
    sidelines->setZValue(0);
//    sidelines->plain = true;
    // Creating the field
    field = new GuiField();
    field->setX(-2900);
    field->setY(-1900); // Y seems to be 100 off (?)
    field->setZValue(1);
    field->grid = false;
    // Creating the ball
    ball = new GuiBall;

    // Elaborate hide/show workaround to get the field centered on launch (works with defaultZoom();
    ui->gView_field->setScene(scene);
    ui->gView_field->show();

    //Bot Labels - blue team
    botLabel0 = new GuiBotLabel;
    botLabel1 = new GuiBotLabel;
    botLabel2 = new GuiBotLabel;
    botLabel3 = new GuiBotLabel;
    botLabel4 = new GuiBotLabel;
    botLabel5 = new GuiBotLabel;
    // Putting into blue vector
    guiLabels.push_back(botLabel0);
    guiLabels.push_back(botLabel1);
    guiLabels.push_back(botLabel2);
    guiLabels.push_back(botLabel3);
    guiLabels.push_back(botLabel4);
    guiLabels.push_back(botLabel5);

    scene->addItem(botLabel0);
    scene->addItem(botLabel1);
    scene->addItem(botLabel2);
    scene->addItem(botLabel3);
    scene->addItem(botLabel4);
    scene->addItem(botLabel5);

    // Bot Labels - yellow team
    botLabel0Y = new GuiBotLabel;
    botLabel1Y = new GuiBotLabel;
    botLabel2Y = new GuiBotLabel;
    botLabel3Y = new GuiBotLabel;
    botLabel4Y = new GuiBotLabel;
    botLabel5Y = new GuiBotLabel;

    // Putting into yellow vector
    guiLabelsY.push_back(botLabel0Y);
    guiLabelsY.push_back(botLabel1Y);
    guiLabelsY.push_back(botLabel2Y);
    guiLabelsY.push_back(botLabel3Y);
    guiLabelsY.push_back(botLabel4Y);
    guiLabelsY.push_back(botLabel5Y);

    scene->addItem(botLabel0Y);
    scene->addItem(botLabel1Y);
    scene->addItem(botLabel2Y);
    scene->addItem(botLabel3Y);
    scene->addItem(botLabel4Y);
    scene->addItem(botLabel5Y);

    // Blue Team Robots
    robot0 = new GuiRobot();
    robot1 = new GuiRobot();
    robot2 = new GuiRobot();
    robot3 = new GuiRobot();
    robot4 = new GuiRobot();
    robot5 = new GuiRobot();

    // Putting into blue team vector
    guiTeam.push_back(robot0);
    guiTeam.push_back(robot1);
    guiTeam.push_back(robot2);
    guiTeam.push_back(robot3);
    guiTeam.push_back(robot4);
    guiTeam.push_back(robot5);


    scene->addItem(robot0);
    scene->addItem(robot1);
    scene->addItem(robot2);
    scene->addItem(robot3);
    scene->addItem(robot4);
    scene->addItem(robot5);

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


    scene->addItem(robot0Y);
    scene->addItem(robot1Y);
    scene->addItem(robot2Y);
    scene->addItem(robot3Y);
    scene->addItem(robot4Y);
    scene->addItem(robot5Y);

    scene->addItem(sidelines);
    scene->addItem(field);
    scene->addItem(ball);

    ui->gView_field->hide();
    ui->gView_field->show();
}

void MainWindow::createPointer()
{
//    QGraphicsScene *s;
    //    updatePointer(s);
}


void MainWindow::updatePointer() {

}

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
    //cout << id << ": " << gamemodel->find(id, team)->getCurrentBeh() << "\n";
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

double MainWindow::getBallCoordX() {
    double b;
    b = gamemodel->getBallPoint().x;

    return b;
}

double MainWindow::getBallCoordY() {
    double b;
    b = gamemodel->getBallPoint().y;
    return b;
}


QString MainWindow::getRemTime() {
    QString t;  // return value
    std::string time = std::to_string(gamemodel->getRemainingTime());
    t = QString::fromStdString(time);

    return t;
}

void MainWindow::printBots() {


    // X Coordinate vector
    std::vector<QLCDNumber*> botXcoords(teamSize);
    botXcoords[0] = ui->lcd_coordX_0;
    botXcoords[1] = ui->lcd_coordX_1;
    botXcoords[2] = ui->lcd_coordX_2;
    botXcoords[3] = ui->lcd_coordX_3;
    botXcoords[4] = ui->lcd_coordX_4;
    botXcoords[5] = ui->lcd_coordX_5;
    // Y Coordinate vector
    std::vector<QLCDNumber*> botYcoords(teamSize);
    botYcoords[0] = ui->lcd_coordY_0;
    botYcoords[1] = ui->lcd_coordY_1;
    botYcoords[2] = ui->lcd_coordY_2;
    botYcoords[3] = ui->lcd_coordY_3;
    botYcoords[4] = ui->lcd_coordY_4;
    botYcoords[5] = ui->lcd_coordY_5;
    // Orientation vector
    std::vector<QLCDNumber*> botOrients(teamSize);
    botOrients[0] = ui->lcd_orient_0;
    botOrients[1] = ui->lcd_orient_1;
    botOrients[2] = ui->lcd_orient_2;
    botOrients[3] = ui->lcd_orient_3;
    botOrients[4] = ui->lcd_orient_4;
    botOrients[5] = ui->lcd_orient_5;
    // Icon vector
    std::vector<QLabel*> botIcons(teamSize);
    botIcons[0] = ui->output_icon_0;
    botIcons[1] = ui->output_icon_1;
    botIcons[2] = ui->output_icon_2;
    botIcons[3] = ui->output_icon_3;
    botIcons[4] = ui->output_icon_4;
    botIcons[5] = ui->output_icon_5;
    // Title label vector
    std::vector<QLabel*> botTitle(teamSize);
    botTitle[0] = ui->title_robPanel_0;
    botTitle[1] = ui->title_robPanel_1;
    botTitle[2] = ui->title_robPanel_2;
    botTitle[3] = ui->title_robPanel_3;
    botTitle[4] = ui->title_robPanel_4;
    botTitle[5] = ui->title_robPanel_5;
    // Printing to GUI
    for (int i=0; i<teamSize; i++) {
        if (gamemodel->find(i,gamemodel->getMyTeam()) != NULL) {
            botTitle[i]->setText("Robot " + QString::number(i));
            botIcons[i]->setPixmap(QPixmap::fromImage(getStatusImg(i)));
            botXcoords[i]->display(getBotCoordX(true, i));
            botYcoords[i]->display(getBotCoordY(true, i));
            botOrients[i]->display(getBotOrientString(i));
        }
    }
}

void MainWindow::printBall() {
    ui->lcd_coordX_ball->display(getBallCoordX());
    ui->lcd_coordY_ball->display(getBallCoordY());

    ui->output_remTime->setText(getRemTime());
    QImage ball;
    ball.load(":/images/ball.png");
    ui->label_BallPos->setPixmap(QPixmap::fromImage(ball));
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


void MainWindow::gameModelUpdated() {
    if (SIMULATED) {
        ui->menuDashboard->setTitle("Simulation");
    } else {
        ui->menuDashboard->setTitle("Camera");
    }
    // Print-to-GUI functions
    printBots();
    printBall();
//    updateScene();
//    QCoreApplication::processEvents();

}

int MainWindow::frequency_of_primes (int n) {
  int i,j;
  int freq=n-1;
  for (i=2; i<=n; ++i) for (j=sqrt(i);j>1;--j) if (i%j==0) {--freq; break;}
  return freq;
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


