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

using namespace std;


//void MainWindow::printLabel(/*std::string label0*/) {     // DESTROY?
//    gameModelUpdated();
//}




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    gamemodel = GameModel::getModel();
    //gamemodel->setMainWindow(this);

    // Create Threads, the parameters are the timer value, and parent.
    // threads is declated as QList<GuiComm*> threads;
    // create threads, and append them to the threads list, so that
    // threads can be accessed for making connections, and to start
    // and stop threads
    threads.append(new GuiComm(99, this));
    threads.append(new GuiComm(20, this));
    threads.append(new GuiComm(30, this));
    threads.append(new GuiComm(40, this));
    threads.append(new GuiComm(50, this));
    threads.append(new GuiComm(60, this));

    // Connect each Widget to correcponding thread
//    connect(threads[0], SIGNAL(valueChanged(int))
//            , this, SLOT(test(int)));

    connect(threads[0], SIGNAL(valueChanged(int))
                , this, SLOT(launch(int)));

    connect(threads[1], SIGNAL(valueChanged(int))
            , ui->progressBar, SLOT(setValue(int)));

    connect(threads[2], SIGNAL(valueChanged(int))
            , ui->dial, SLOT(setValue(int)));

    connect(threads[3], SIGNAL(valueChanged(int))
            , ui->lcdNumber, SLOT(display(int)));

    connect(threads[4], SIGNAL(valueChanged(int))
            , ui->horizontalSlider, SLOT(setValue(int)));

    connect(threads[5], SIGNAL(valueChanged(int))
            , ui->verticalSlider, SLOT(setValue(int)));

}

void MainWindow::launch(int value)
{
    ui->label->setText(QString("Current Thread Processing Status : %1").arg(value));
    gameModelUpdated();
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


void MainWindow::myclicked() {
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

double MainWindow::getBotCoordX(int id) {
    double x  = 0.00;
    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (team.at(id) != NULL) {
        x = gamemodel->find(id, team)->getRobotPosition().x;
    }
    //cout << x << " \n";
    return x;
}

double MainWindow::getBotCoordY(int id) {
    double y  = 0.00;
    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (team.at(id) != NULL) {
        y = gamemodel->find(id, team)->getRobotPosition().y;
    }
    //cout << y << " \n";
    return y;
}

QString MainWindow::getBotOrient(int id) {
    QString qOrient = "no connection";
    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (team.at(0) != NULL){
        std::string orientRob = std::to_string(gamemodel->find(id, team)->getOrientation() );
        qOrient = QString::fromStdString(orientRob);
    }//end if
    return qOrient;

}

double MainWindow::getBotOrientDouble(int id) {
    double o  = 0.00;
    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (team.at(id) != NULL) {
        o = gamemodel->find(id, team)->getOrientation();
    }
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
    // Default icons vector
    std::vector<QImage> bot_defaults(5);
    bot_defaults[0] = bot0_default;
    bot_defaults[1] = bot1_default;
    bot_defaults[2] = bot2_default;
    bot_defaults[3] = bot3_default;
    bot_defaults[4] = bot4_default;
    // Has Ball icon
    QImage hasBall;
    hasBall.load(":/images/ball.png");


    std::vector<Robot*> team = gamemodel->getMyTeam();

    if (gamemodel->find(id, team)->hasBall) {
        //cout << "hasBall TRUE \n";
        botStatusPic = hasBall;
    }
    if (gamemodel->find(id, team)->drible) {
        botStatusPic = hasBall;
    }
    if ( gamemodel->find(id, team)->hasBeh ) {
        //cout << id << " hasBeh TRUE \n";
        botStatusPic = bot_defaults[id];
    } else {
        //cout << id << " hasBeh FALSE \n";
        //botStatusPic.load(":/images/process-stop.png");
    }
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

    int teamSize = 5;
    // X Coordinate vector
    std::vector<QLCDNumber*> botXcoords(teamSize);
    botXcoords[0] = ui->lcd_coordX_0;
    botXcoords[1] = ui->lcd_coordX_1;
    botXcoords[2] = ui->lcd_coordX_2;
    botXcoords[3] = ui->lcd_coordX_3;
    botXcoords[4] = ui->lcd_coordX_4;
    // Y Coordinate vector
    std::vector<QLCDNumber*> botYcoords(teamSize);
    botYcoords[0] = ui->lcd_coordY_0;
    botYcoords[1] = ui->lcd_coordY_1;
    botYcoords[2] = ui->lcd_coordY_2;
    botYcoords[3] = ui->lcd_coordY_3;
    botYcoords[4] = ui->lcd_coordY_4;
    // Orientation vector
    std::vector<QLCDNumber*> botOrients(teamSize);
    botOrients[0] = ui->lcd_orient_0;
    botOrients[1] = ui->lcd_orient_1;
    botOrients[2] = ui->lcd_orient_2;
    botOrients[3] = ui->lcd_orient_3;
    botOrients[4] = ui->lcd_orient_4;
    // Icon vector
    std::vector<QLabel*> botIcons(5);
    botIcons[0] = ui->output_icon_0;
    botIcons[1] = ui->output_icon_1;
    botIcons[2] = ui->output_icon_2;
    botIcons[3] = ui->output_icon_3;
    botIcons[4] = ui->output_icon_4;

    // Printing to GUI
    for (int i=0; i<teamSize; i++) {
        botIcons[i]->setPixmap(QPixmap::fromImage(getStatusImg(i)));
        botXcoords[i]->display(getBotCoordX(i));
        botYcoords[i]->display(getBotCoordY(i));
        botOrients[i]->display(getBotOrientDouble(i));
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


void MainWindow::gameModelUpdated() {
    if (SIMULATED) {
        ui->menuDashboard->setTitle("Simulation");
    } else {
        ui->menuDashboard->setTitle("Camera");
    }
    // Print-to-GUI functions
    printBots();
    printBall();
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
