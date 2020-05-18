#ifndef PANE_LFIELD_H
#define PANE_LFIELD_H

#include <QFrame>

#include "ui_panel_field.h"
#include "model/constants.h"
#include "utilities/point.h"
#include <deque>
#include <mutex>


using std::deque;
class GuiRobotDrawer;
class GuiField;
class GuiSidelines;
class GuiBotLabel;
class GuiDrawLine;
class GuiDrawPoint;
class GuiDrawRegion;
class MainWindow;

class PanelField : public QFrame, public Ui::PanelField
{
    Q_OBJECT

public:
    explicit PanelField(QWidget *parent = nullptr);
    ~PanelField();


    void updateScene();
    void scanForSelection();
    void centerViewOnBot();
    void scanForScrollModifier();
    void updateLineQueue();

    void updateTeamSelected();


//private:
    QGraphicsScene *scene;
// Objects in scene
    GuiField *field;
    GuiSidelines *sidelines;

    // gui robots : 2 teams each of at most
    GuiBotLabel* gui_bot_labels[2][MAX_ROBOTS_PER_TEAM];
    GuiRobotDrawer* robot_drawers[2][MAX_ROBOTS_PER_TEAM];


    // "Camera" type functions for manipulating QGraphicsView
    GuiRobotDrawer* centeredBotID = nullptr;
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



public slots:
    void zoomField(int scale);
    void defaultZoom();
    void drawLine();
    void drawPoint();
    void drawRegion();
    void setupLine(Point start, Point stop, double seconds);
    void setupPoint(Point p);
    void setupRegion(std::vector<Point>);

signals:
    void field_mouse_moved(QPointF);

private:
    MainWindow * dash;
    deque<GuiDrawLine*> lineQueue;
    GuiDrawPoint* point_drawer;
    GuiDrawRegion* region_drawer;
    void doubleClickScan();
    void cameraMoveScan();
    bool fieldClickScan();
    bool panelBotClickScan();
    bool fieldBotClickScan();
    bool selectRobot(int team, int robot);

    bool eventFilter(QObject* obj, QEvent* e);

};

#endif // PANE_LFIELD_H
