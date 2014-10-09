#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QFrame>
#include "model/gamemodel.h"
#include "model/robot.h"
#include <QLabel>
#include "guicomm.h"

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

//Field
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include "guirobot.h"
#include "guifield.h"
#include "guiball.h"
#include "guisidelines.h"
#include "guibotlabel.h"

//class GuiComm;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    //Q_INVOKABLE static void printLabel(); DESTROY?
    QString getBotCoord(int id); // Returns the specified robot's x/y position as a QString
    QString getBotOrientString(int id);
    double  getBotOrientDouble(bool myTeam, int id);
    int getBotCoordX(bool myTeam, int id);
    int getBotCoordY(bool myTeam,int id);
    QImage  getStatusImg(int id);
    bool    hasChangedQString(QLabel* l, QString s);
    bool    hasChangedQImage(QLabel* l, QImage s);
    int     getBotSpeed(std::vector<QLabel*> c, int id);
    QString getBallCoord();
    double  getBallCoordX();
    double  getBallCoordY();
    QString getRemTime();
    void printBots();
    void printBall();
    void gameModelUpdated();
    // For getting milliseconds
    int frequency_of_primes (int n);
    int getSpeed(QGraphicsItem* p, double o);


protected:
    GameModel *gamemodel;
    Robot *robot;
    GuiComm *guicomm;
    GuiRobot *guirobot;


signals:
    void labelPrinted();
    void printLabel();
    bool sceneReady();


private:
    Ui::MainWindow *ui;
    QList<GuiComm*> threads;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    int teamSize = 6;
    GuiRobot *robot0;
    GuiRobot *robot1;
    GuiRobot *robot2;
    GuiRobot *robot3;
    GuiRobot *robot4;
    GuiRobot *robot5;
    std::vector<GuiRobot*> guiTeam;
//    GuiRobot* guiTeam[6] {
//        robot0,
//        robot1,
//        robot2,
//        robot3,
//        robot4,
//        robot5
//    };
    GuiRobot *robot0Y;
    GuiRobot *robot1Y;
    GuiRobot *robot2Y;
    GuiRobot *robot3Y;
    GuiRobot *robot4Y;
    GuiRobot *robot5Y;
    std::vector<GuiRobot*> guiTeamY;
//    GuiRobot* guiTeamY[6] {
//        robot0Y,
//        robot1Y,
//        robot2Y,
//        robot3Y,
//        robot4Y,
//        robot5Y
//    };
    GuiBotLabel *botLabel0;
    GuiBotLabel *botLabel1;
    GuiBotLabel *botLabel2;
    GuiBotLabel *botLabel3;
    GuiBotLabel *botLabel4;
    GuiBotLabel *botLabel5;
    std::vector<GuiBotLabel*> guiLabels;
//    GuiBotLabel* guiLabels[6] = {
//        botLabel0,
//        botLabel1,
//        botLabel2,
//        botLabel3,
//        botLabel4,
//        botLabel5
//    };
    GuiBotLabel *botLabel0Y;
    GuiBotLabel *botLabel1Y;
    GuiBotLabel *botLabel2Y;
    GuiBotLabel *botLabel3Y;
    GuiBotLabel *botLabel4Y;
    GuiBotLabel *botLabel5Y;
    std::vector<GuiBotLabel*> guiLabelsY;
//    GuiBotLabel* guiLabelsY[6] = {
//        botLabel0Y,
//        botLabel1Y,
//        botLabel2Y,
//        botLabel3Y,
//        botLabel4Y,
//        botLabel5Y
//    };

    GuiField *field;
    GuiSidelines *sidelines;
    GuiBall *ball;

    long double currentTimeMS = 0;
    double ballOrigin = 0;
    double bot0Origin = 0;
    int currentFieldAngle = 0;
    int currentFieldScrollH;


private slots:
    void updatePointer();
    void on_pushButton_clicked();
    void launch(int value);
    void updateScene();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void zoomField(int scale);
    void defaultZoom();
    void createPointer();
    void setUpScene();
};

#endif // MAINWINDOW_H
