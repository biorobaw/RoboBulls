#ifndef ROBOTPANEL_H
#define ROBOTPANEL_H

#include <iostream>
//#include "guirobot.h"
#include "mainwindow.h"
//#include "guiball.h"
#include <QLCDNumber>
#include <QDial>
#include <QGraphicsView>
#include <QMainWindow>
#include "model/gamemodel.h"
//#include "fieldpanel.h"

class GuiRobot;
class GuiBall;
class FieldPanel;

class RobotPanel : public QObject   // Some kind of Q type required for cross-class signals & slots
{
    Q_OBJECT                        // this macro required for cross-class signals & slots

public:
    RobotPanel(MainWindow * mw);
    MainWindow * dash;

    void setupBotPanel();
    void updateBotPanel();


//private:
    // Robot Panel
    std::vector<QLCDNumber*> botXcoords;
    std::vector<QLCDNumber*> botYcoords;
    std::vector<QDial*> botOrients;
    std::vector<QFrame*> botFrames;
    std::vector<QLabel*> botTitle;
    std::vector<QDial*> velocityDials;
    std::vector<QDial*> speedDials;
    // Bot icon graphics views

    std::vector<QGraphicsView*> botIconFrames;
    // Bot individual icon scenes
    QGraphicsScene *scene_botIcon_0;
    QGraphicsScene *scene_botIcon_1;
    QGraphicsScene *scene_botIcon_2;
    QGraphicsScene *scene_botIcon_3;
    QGraphicsScene *scene_botIcon_4;
    QGraphicsScene *scene_botIcon_5;
    QGraphicsScene *scene_botIcon_6;
    QGraphicsScene *scene_botIcon_7;
    QGraphicsScene *scene_botIcon_8;
    QGraphicsScene *scene_botIcon_9;

    std::vector<QGraphicsScene*> botIconScenes;
    // Bot selected icon scenes
    QGraphicsScene *scene_botIconSel_0;
    QGraphicsScene *scene_botIconSel_1;
    QGraphicsScene *scene_botIconSel_2;
    QGraphicsScene *scene_botIconSel_3;
    QGraphicsScene *scene_botIconSel_4;
    QGraphicsScene *scene_botIconSel_5;
    QGraphicsScene *scene_botIconSel_6;
    QGraphicsScene *scene_botIconSel_7;
    QGraphicsScene *scene_botIconSel_8;
    QGraphicsScene *scene_botIconSel_9;

    std::vector<QGraphicsScene*> botIconSelScenes;
    // Bot icons
    GuiRobot *robotIcon0;
    GuiRobot *robotIcon1;
    GuiRobot *robotIcon2;
    GuiRobot *robotIcon3;
    GuiRobot *robotIcon4;
    GuiRobot *robotIcon5;
    GuiRobot *robotIcon6;
    GuiRobot *robotIcon7;
    GuiRobot *robotIcon8;
    GuiRobot *robotIcon9;

    std::vector<GuiRobot*> botIcons;
    // Bot icons for the selected bot panel
    GuiRobot *robotIcon0Sel;
    GuiRobot *robotIcon1Sel;
    GuiRobot *robotIcon2Sel;
    GuiRobot *robotIcon3Sel;
    GuiRobot *robotIcon4Sel;
    GuiRobot *robotIcon5Sel;
    GuiRobot *robotIcon6Sel;
    GuiRobot *robotIcon7Sel;
    GuiRobot *robotIcon8Sel;
    GuiRobot *robotIcon9Sel;

    std::vector<GuiRobot*> botIconsSelected;
    // Ball Icon (eventually move to diff class)
    GuiBall *ballIcon;
    QGraphicsScene *scene_ballIcon;

public slots:
    void toggleIconVisible();

private:
    static RobotPanel* robotpanel;

protected:
    GameModel *gamemodel;
    FieldPanel *fieldpanel;


};

#endif // ROBOTPANEL_H
