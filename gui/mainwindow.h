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

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include "guirobot.h"
#include "guifield.h"
#include "guiball.h"
#include "guisidelines.h"
#include "guibotlabel.h"
#include "communication/refcomm.h"
#include "guidrawline.h"
#include "communication/nxtrobcomm.h"
#include "movement/move.h"
#include "guiscene.h"
//#include "robotpanel.h"

class RobotPanel;   // Forward Declaration prevents future problems from two<-->way references

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();

public:
    static MainWindow* getMainWindow();
    bool guiOverride = false;           // For the purpose of overriding gamemodel's commands with our own
    int botOverride = -1;               // the id of the currently overridden bot
    int selectedBot = -1;               // the ID of the currently selected bot
    std::vector<bool> overriddenBots;   // keeps track of whether each bot is overridden
    QString getBotCoord(int id);        // Returns the specified robot's x/y position as a QString
    QString getBotOrientString(int id);
    double  getBotOrientDouble(bool myTeam, int id);
    int getBotCoordX(bool myTeam, int id);
    int getBotCoordY(bool myTeam,int id);
    int     getBotSpeed(std::vector<QLabel*> c, int id);
    QString getBallCoord();
    int  getBallCoordX();
    int  getBallCoordY();
    QString getRemTime();
    void scanForSelection();
    int getVelocity(int id);
    // Debug functions
    void printBehavior(int botID, string behavior, bool append);    // puts the given bot's given string into a vector and prints it when the bot is selected
    void drawLine( int originX, int originY, int endX, int endY );
    void guiPrint(string output);
    void guiPrintRobot(int robotID, string output);
    void updateBallInfo();
    // For getting milliseconds
    int frequency_of_primes (int n);
    int getClock();
    int getSpeed(QGraphicsItem* p, double o);
    void moveBot();
    // Key Bindings
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    // Mouse stuff
    int getMouseCoordX();
    int getMouseCoordY();
    void centerViewOnBot();
    void setMyVelocity();
    void setGuiOverride();
    // 10/23/14
    Ui::MainWindow *ui;
    int teamSize = 6;
    GameModel *gamemodel;


protected:
    Robot *robot;
    GuiComm *guicomm;
    GuiRobot *guirobot;
    RefComm *refcom;
    GuiDrawLine *guidrawline;
    NXTRobComm *nxtrobcomm;


signals:
    void labelPrinted();
    void printLabel();
    bool sceneReady();


private:
    RobotPanel * robotPanel;

    void setupKeyShortcuts();
    // moved from public
    explicit MainWindow(QWidget *parent = 0);
    // my pointer
    static MainWindow *mw;
    QList<GuiComm*> threads;
    GuiScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    // Behavior vector
//    std::vector<QString*> botBehavior[6];
    QString botBehavior[6];

    QGraphicsScene *selectedBotScene;
    // Field
    GuiRobot *robot0;
    GuiRobot *robot1;
    GuiRobot *robot2;
    GuiRobot *robot3;
    GuiRobot *robot4;
    GuiRobot *robot5;
    std::vector<GuiRobot*> guiTeam;
    GuiRobot *robot0Y;
    GuiRobot *robot1Y;
    GuiRobot *robot2Y;
    GuiRobot *robot3Y;
    GuiRobot *robot4Y;
    GuiRobot *robot5Y;
    std::vector<GuiRobot*> guiTeamY;
    GuiBotLabel *botLabel0;
    GuiBotLabel *botLabel1;
    GuiBotLabel *botLabel2;
    GuiBotLabel *botLabel3;
    GuiBotLabel *botLabel4;
    GuiBotLabel *botLabel5;
    std::vector<GuiBotLabel*> guiLabels;
    GuiBotLabel *botLabel0Y;
    GuiBotLabel *botLabel1Y;
    GuiBotLabel *botLabel2Y;
    GuiBotLabel *botLabel3Y;
    GuiBotLabel *botLabel4Y;
    GuiBotLabel *botLabel5Y;
    std::vector<GuiBotLabel*> guiLabelsY;
    // Camera
    int centeredBotID = -1;
    // Keeps track of latest string received by guiPrint()
    QString guiOutput = "...";
    QString guiOutputRobot = "...";

    GuiField *field;
    GuiSidelines *sidelines;
    GuiBall *ball;
    // pointer to MainWindow
    static MainWindow* window;

    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    // For controlling bots manually
    int myVelocity;

    long double currentTimeMS = 0;
    double ballOrigin = 0;
    double bot0Origin = 0;
    int currentFieldAngle = 0;
    int currentFieldScrollH;



private slots:
    void on_btn_connectGui_clicked();
    void launch(int value);
    void updateScene();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void zoomField(int scale);
    void defaultZoom();
    void setUpScene();
    void on_check_fieldGrid_clicked();
    void on_combo_gridScale_currentIndexChanged(int index);
    void on_check_coloredGoals_clicked();
    void on_combo_fieldColor_currentIndexChanged(int index);
    void on_check_showIDs_stateChanged(int arg1);
    void on_combo_botScale_currentIndexChanged(int index);
    void field_setDragMode();
    void updateSelectedBotPanel(int id);
    void on_btn_botForward_pressed();
    void on_btn_botForward_released();
    void on_btn_botTurnRight_pressed();
    void on_btn_botTurnRight_released();
    void on_btn_botReverse_pressed();
    void on_btn_botReverse_released();
    void on_btn_botTurnLeft_pressed();
    void on_btn_botTurnLeft_released();
    void on_btn_botKick_pressed();
    void on_btn_botKick_released();
    void on_btn_botDrible_pressed();
    void on_btn_botDrible_released();
    void on_check_botOverride_clicked(bool checked);
    void on_btn_override_all_released();
    void on_btn_override_none_released();
    void toggleIconVisible();

};

#endif // MAINWINDOW_H


