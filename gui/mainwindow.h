#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QFrame>
#include "model/gamemodel.h"
#include "model/robot.h"
#include <QLabel>

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include "communication/refcomm.h"
#include "communication/nxtrobcomm.h"
#include "movement/move.h"
// Helper classes
//#include "guirobot.h"
#include "guifield.h"
#include "guiball.h"
#include "guibotlabel.h"
#include "guidrawline.h"
#include "guicomm.h"
#include "guiscene.h"

class RobotPanel;   // Forward Declaration prevents future problems from two<-->way references
class FieldPanel;
class SelRobotPanel;
class ObjectPosition;
class GuiRobot;
//class GuiField;
//class GuiBall;
//class GuiBotLabel;
//class GuiDrawLine;
//class GuiScene;
//class GuiComm;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();

public:
    // Class pointers
    Ui::MainWindow *ui;
    GameModel *gamemodel;
    RobotPanel * robotpanel;
    FieldPanel * fieldpanel;
    SelRobotPanel * selrobotpanel;
    ObjectPosition * objectPos;

    static MainWindow* getMainWindow();
    QString getRemTime();
    void scanForSelection();
    int getVelocity(int id);
    // Debug functions
    void drawLine( int originX, int originY, int endX, int endY );
    void guiPrint(string output);
    void updateBallInfo();
    // For getting milliseconds
    int frequency_of_primes (int n);
    int getClock();
    int getSpeed(QGraphicsItem* p, double o);
    void moveBot();
    // Key Bindings
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void centerViewOnBot();
    void setMyVelocity();
    // 10/23/14
    int teamSize = 6;
    bool guiOverride = false;           // For the purpose of overriding gamemodel's commands with our own
    int botOverride = -1;               // the id of the currently overridden bot
    std::vector<bool> overriddenBots;   // keeps track of whether each bot is overridden
    QString guiOutputRobot = "...";
    QString botBehavior[6];
    void guiPrintRobot(int robotID, std::string output);


protected:
    Robot *robot;
    GuiComm *guicomm;
    GuiRobot *guirobot;
    RefComm *refcom;
    GuiDrawLine *guidrawline;
    NXTRobComm *nxtrobcomm;


signals:
    bool sceneReady();


private:

    void setupKeyShortcuts();
    // moved from public
    explicit MainWindow(QWidget *parent = 0);
    // my pointer
    static MainWindow *mw;
    QList<GuiComm*> threads;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;

    QGraphicsScene *selectedBotScene;
    // Keeps track of latest string received by guiPrint()
    QString guiOutput = "...";

    // pointer to MainWindow
    static MainWindow* window;

    // For controlling bots manually
    int myVelocity;

    long double currentTimeMS = 0;
    double ballOrigin = 0;  // delete?
    double bot0Origin = 0;  // delete?
    int currentFieldScrollH;    // delete?

private slots:
    void launch(int value);
    void on_btn_connectGui_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
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
    // Field
    void on_check_fieldGrid_clicked();
    void on_combo_gridScale_currentIndexChanged(int index);
    void on_check_coloredGoals_clicked();
    void on_combo_fieldColor_currentIndexChanged(int index);
    void on_check_showIDs_stateChanged(int arg1);
    void on_combo_botScale_currentIndexChanged(int index);


};

#endif // MAINWINDOW_H


