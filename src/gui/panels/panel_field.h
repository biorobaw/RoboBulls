#ifndef PANE_LFIELD_H
#define PANE_LFIELD_H

#include <QFrame>

#include "ui_panel_field.h"
#include "model/constants.h"
#include "utilities/point.h"
#include <deque>
#include <mutex>


using std::deque;
class GraphicsRobot;
class GraphicsField;
class GraphicsOutterField;
class GraphicsLabel;
class GraphicsLine;
class GraphicsPoints;
class GraphicsPolygon;
class GraphicsBall;
class MainWindow;

class PanelField : public QFrame, public Ui::PanelField
{
    Q_OBJECT

public:
    explicit PanelField(QWidget *parent = nullptr);
    ~PanelField();


    void updateScene();
    void setHidePaths(bool val);



    void scanForSelection();
    void scanForScrollModifier();
    void updateLineQueue();



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

    // scene and graphic objects
    QGraphicsScene*     scene;
    GraphicsRobot*     robot_drawers[2][MAX_ROBOTS_PER_TEAM];
    GraphicsLabel*        gui_bot_labels[2][MAX_ROBOTS_PER_TEAM];
    GraphicsOutterField*       sidelines;
    GraphicsField*           field;
    GraphicsPolygon*      region_drawer;
    GraphicsPoints*       point_drawer;
    GraphicsBall*      ball_drawer;
    deque<GraphicsLine*> lineQueue;




    // DATA FOR DRAWING
    // for drawLine
    Point linePointA = Point(0,0);
    Point linePointB = Point(0,0);
    double lineLifeSpan = 0;
    bool needLine = false;

    // Line drawing information
    deque<Point> lineAPoints;
    deque<Point> lineBPoints;
    deque<double> lineLifeSpans;
    bool hide_paths = false;

    // Point drawing information
    std::mutex points_mutex;
    deque<Point> simple_points;

    // Region drawing information
    deque<Point> region_points;


    // "Camera" type functions for manipulating QGraphicsView
    GraphicsRobot* centeredBotID = nullptr;
    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    int currentFieldAngle = 0;


    // User interaction
    // the ID of the currently selected bot
    int selectedBot= -1;


    //    void doubleClickScan();
//    void cameraMoveScan();
//    bool fieldClickScan();
//    bool panelBotClickScan();
//    bool fieldBotClickScan();




    void selectRobot(int team, int robot);

    bool eventFilter(QObject* obj, QEvent* e);


private slots:
    void on_check_fieldGrid_clicked();
    void on_combo_gridScale_currentIndexChanged(int index);
    void on_check_coloredGoals_clicked();
    void on_combo_fieldColor_currentIndexChanged(int index);
    void on_check_showIDs_stateChanged(int arg1);
    void on_combo_botScale_currentIndexChanged(int index);



};

#endif // PANE_LFIELD_H
