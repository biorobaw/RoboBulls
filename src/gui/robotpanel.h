#ifndef ROBOTPANEL_H
#define ROBOTPANEL_H

#include <QObject>
#include <vector>
#include <string>
#include "src/model/team.h"

class QDial;
class QFrame;
class QLabel;
class QLCDNumber;
class QGraphicsView;
class QGraphicsScene;
class GuiRobot;
class GuiBall;
class FieldPanel;
class MainWindow;
class GameState;

//RobotPanel is the sidebar of robot display slots

class RobotPanel : public QObject   // Some kind of Q type required for cross-class signals & slots
{
    Q_OBJECT                        // this macro required for cross-class signals & slots

public:
    RobotPanel(MainWindow * mw);
    MainWindow * dash;

    void setupBotPanel();
    void updateBotPanel();
    void scrollToSelBot(int id);
    void updateTeamColors();
    const std::string getGameState();
    const std::string getCurrStrategy();


//private:
    // Robot Panel
    std::vector<QLCDNumber*> botXcoords;
    std::vector<QLCDNumber*> botYcoords;
    std::vector<QDial*> botOrients;
    std::vector<QFrame*> botFrames;
    std::vector<QLabel*> botTitle;
    std::vector<QDial*> velocityDials;
    std::vector<QLabel*> currBehavior;
    QLabel* gState;
    QLabel* ballVel;
    QLabel* ballAccel;
    QLabel* ballMove;
    QLabel* currStrategy;
    QLabel* blueGoal;
    QLabel* yellGoal;
    QLabel* timeRem;
//  std::vector<QDial*> speedDials;         Will erase after I add the current behavior to where the speed dial used to be


    // Bot icon graphics views

    std::vector<QGraphicsView*> botIconFrames;
    // Bot individual icon scenes
    QGraphicsScene* scene_botIcon[MAX_ROBOTS];

    // Bot selected icon scenes
    QGraphicsScene* scene_botIconSel[MAX_ROBOTS];

    // Bot icons
    GuiRobot* robotIcon[MAX_ROBOTS];
    GuiRobot* robotIconSel[MAX_ROBOTS];

    //TODO: Ball Icon (eventually move to diff class)
    GuiBall *ballIcon;
    QGraphicsScene *scene_ballIcon;

public slots:
    void toggleIconVisible();

private:
    static RobotPanel* robotpanel;

protected:
    GameState *gamemodel;
    FieldPanel *fieldpanel;
};

#endif // ROBOTPANEL_H
