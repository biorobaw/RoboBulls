#ifndef FIELDPANEL_H
#define FIELDPANEL_H

#include <deque>
#include <mutex>
#include "guisidelines.h"
#include "utilities/point.h"
#include "guidrawpoint.h"
#include "guidrawregion.h"

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
    GuiRobot *robot6;
    GuiRobot *robot7;
    GuiRobot *robot8;
    GuiRobot *robot9;
    std::vector<GuiRobot*> guiTeam;
    // ID labels
    GuiBotLabel *botLabel0;
    GuiBotLabel *botLabel1;
    GuiBotLabel *botLabel2;
    GuiBotLabel *botLabel3;
    GuiBotLabel *botLabel4;
    GuiBotLabel *botLabel5;
    GuiBotLabel *botLabel6;
    GuiBotLabel *botLabel7;
    GuiBotLabel *botLabel8;
    GuiBotLabel *botLabel9;
    std::vector<GuiBotLabel*> guiLabels;
    // Opponents' robots
    GuiRobot *robot0Y;
    GuiRobot *robot1Y;
    GuiRobot *robot2Y;
    GuiRobot *robot3Y;
    GuiRobot *robot4Y;
    GuiRobot *robot5Y;
    GuiRobot *robot6Y;
    GuiRobot *robot7Y;
    GuiRobot *robot8Y;
    GuiRobot *robot9Y;
    std::vector<GuiRobot*> guiTeamY;
    GuiBotLabel *botLabel0Y;
    GuiBotLabel *botLabel1Y;
    GuiBotLabel *botLabel2Y;
    GuiBotLabel *botLabel3Y;
    GuiBotLabel *botLabel4Y;
    GuiBotLabel *botLabel5Y;
    GuiBotLabel *botLabel6Y;
    GuiBotLabel *botLabel7Y;
    GuiBotLabel *botLabel8Y;
    GuiBotLabel *botLabel9Y;
    std::vector<GuiBotLabel*> guiLabelsY;
    // "Camera" type functions for manipulating QGraphicsView
    int centeredBotID = -1;
    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    int currentFieldAngle = 0;
    // User interaction
    int selectedBot= -1;

    // the ID of the currently selected bot

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
