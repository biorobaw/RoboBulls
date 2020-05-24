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
class GuiRobot;

class PanelField : public QFrame, public Ui::PanelField
{
    Q_OBJECT

public:
    explicit PanelField(QWidget *parent = nullptr);
    ~PanelField();


    void updateScene();
    void setHidePaths(bool val);

    void setDragMode(QGraphicsView::DragMode mode);

//    void scanForSelection();
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
//    MainWindow * dash;

    // scene and graphic objects
    QGraphicsScene*       scene;
    GraphicsBall*         ball_drawer;
    GraphicsRobot*        robot_drawers[2][MAX_ROBOTS_PER_TEAM];
    GraphicsLabel*        robot_labels_drawers[2][MAX_ROBOTS_PER_TEAM];
    GraphicsField*        field_drawer;
    GraphicsOutterField*  outter_field_drawer;
    GraphicsPoints*       point_drawer;
    deque<GraphicsLine*>  line_drawers;
    GraphicsPolygon*      region_drawer;



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
    GraphicsRobot* followingRobot = nullptr;
    bool refresh = true;   // set this to true whenever a change to the field is made to refresh on next frame.
    bool justScrolled = false;
    int currentFieldAngle = 0;


    // User interaction

    //    void doubleClickScan();
//    void cameraMoveScan();
//    bool fieldClickScan();
//    bool panelBotClickScan();
//    bool fieldBotClickScan();



    bool eventFilter(QObject* obj, QEvent* e);


private slots:

    void setFollowingRobot(GuiRobot* robot);
    void clearFollowing();



    void on_combo_robot_scale_currentIndexChanged(const QString &arg1);
    void on_combo_ball_scale_currentIndexChanged(const QString &arg1);
    void on_check_show_ids_stateChanged(int arg1);
    void on_combo_ball_color_currentIndexChanged(const QString &arg1);
    void on_check_field_grid_stateChanged(int arg1);
    void on_combo_grid_scale_currentIndexChanged(const QString &arg1);
    void on_combo_field_color_currentIndexChanged(const QString &arg1);

};

#endif // PANE_LFIELD_H
