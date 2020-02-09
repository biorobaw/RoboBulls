#ifndef FIELDPANEL_H
#define FIELDPANEL_H

#include <deque>
#include <mutex>
#include "guisidelines.h"
#include "src/utilities/point.h"
#include "guidrawpoint.h"
#include "guidrawregion.h"
#include "src/model/team.h"

using std::deque;
class GuiRobot;
class GuiBall;
class GuiSidelines;
class GuiScene;
class GuiBotLabel;
class GuiField;
class GuiDrawLine;
class MainWindow;

//FieldPanel is the view container of the field objects on the right

class FieldPanel : public QObject   // Some kind of Q type required for cross-class signals & slots
{
    Q_OBJECT                        // this macro required for cross-class signals & slots

public:
    FieldPanel(MainWindow * mw);
    MainWindow * dash;
    void setupScene();
    void updateScene();
    void scanForSelection();
    void centerViewOnBot();
    void scanForScrollModifier();
    void updateLineQueue();
    void hidePrimeBotPanel();

    void updateTeamSelected();


//private:
    GuiScene *scene;
// Objects in scene
    GuiField *field;
    GuiSidelines *sidelines;
    GuiBall *ball;

    // gui robots : 2 teams each of at most
    GuiRobot* gui_robots[2][MAX_ROBOTS];
    GuiBotLabel* gui_bot_labels[2][MAX_ROBOTS];


    // "Camera" type functions for manipulating QGraphicsView
    GuiRobot* centeredBotID = nullptr;
    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    int currentFieldAngle = 0;


    // User interaction
    // the ID of the currently selected bot
    int selectedBot= -1;


    // for drawLine
    Point linePointA = Point(0,0);
    Point linePointB = Point(0,0);
    double lineLifeSpan = 0;
    bool needLine = false;

    // Line drawing information
    deque<Point> lineAPoints;
    deque<Point> lineBPoints;
    deque<double> lineLifeSpans;

    // Point drawing information
    deque<Point> simple_points;
    std::mutex points_mutex;

    // Region drawing information
    deque<Point> region_points;

    bool hidePaths = false;

private:
    deque<GuiDrawLine*> lineQueue;
    GuiDrawPoint* point_drawer;
    GuiDrawRegion* region_drawer;
    void doubleClickScan();
    void cameraMoveScan();
    bool fieldClickScan();
    bool panelBotClickScan();
    bool fieldBotClickScan();


public slots:
    void zoomField(int scale);
    void defaultZoom();
    void drawLine();
    void drawPoint();
    void drawRegion();
    void setupLine(Point start, Point stop, double seconds);
    void setupPoint(Point p);
    void setupRegion(std::vector<Point>);

};

#endif // FIELDPANEL_H
