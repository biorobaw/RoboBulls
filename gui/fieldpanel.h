#ifndef FIELDPANEL_H
#define FIELDPANEL_H

#include <iostream>
#include "guirobot.h"
#include "mainwindow.h"
#include "guiball.h"
#include "guisidelines.h"
#include "guiscene.h"


class FieldPanel : public QObject   // Some kind of Q type required for cross-class signals & slots
{
    Q_OBJECT                        // this macro required for cross-class signals & slots

public:
    FieldPanel(MainWindow * mw);
    MainWindow * dash;
    void setUpScene();
    void updateScene();
    void scanForSelection();

//private:
    GuiScene *scene;
// Objects in scene
    GuiField *field;
    GuiSidelines *sidelines;
    GuiBall *ball;
    // Our robots
    GuiRobot *robot0;
    GuiRobot *robot1;
    GuiRobot *robot2;
    GuiRobot *robot3;
    GuiRobot *robot4;
    GuiRobot *robot5;
    std::vector<GuiRobot*> guiTeam;
    GuiBotLabel *botLabel0;
    GuiBotLabel *botLabel1;
    GuiBotLabel *botLabel2;
    GuiBotLabel *botLabel3;
    GuiBotLabel *botLabel4;
    GuiBotLabel *botLabel5;
    std::vector<GuiBotLabel*> guiLabels;
    // Opponents' robots
    GuiRobot *robot0Y;
    GuiRobot *robot1Y;
    GuiRobot *robot2Y;
    GuiRobot *robot3Y;
    GuiRobot *robot4Y;
    GuiRobot *robot5Y;
    std::vector<GuiRobot*> guiTeamY;
    GuiBotLabel *botLabel0Y;
    GuiBotLabel *botLabel1Y;
    GuiBotLabel *botLabel2Y;
    GuiBotLabel *botLabel3Y;
    GuiBotLabel *botLabel4Y;
    GuiBotLabel *botLabel5Y;
    std::vector<GuiBotLabel*> guiLabelsY;
    // "Camera" type functions for manipulating QGraphicsView
    int centeredBotID = -1;
    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    int currentFieldAngle = 0;
    // User interaction
    int selectedBot = -1;               // the ID of the currently selected bot

protected:


public slots:
    void zoomField(int scale);
    void defaultZoom();

};

#endif // FIELDPANEL_H
