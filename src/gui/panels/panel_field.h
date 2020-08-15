#ifndef PANE_LFIELD_H
#define PANE_LFIELD_H

#include "ui_panel_field.h"
#include <QFrame>

#include "model/constants.h"
#include "utilities/point.h"
#include <deque>
#include <mutex>

using std::deque;
class GraphicsRobot;
class GraphicsField;
class GraphicsOutterField;
class GraphicsRobotLabel;
class GraphicsLine;
class GraphicsPoints;
class GraphicsPolygon;
class GraphicsBall;
class GuiRobot;

// Class defines the frame used by main_window  to display the robots and ball in the field plus other graphics
// This class extends the frame defined in panel_field.ui

class PanelField : public QFrame, public Ui::PanelField
{
    Q_OBJECT

public:
    explicit PanelField(QWidget *parent = nullptr);
    ~PanelField();


    void updateScene(); // updates the information displayed by the panel

    void setDragMode(QGraphicsView::DragMode mode); // used to allow dragging the scene
    void setHidePaths(bool val); // determines whether to show or not the lines added by gui_interface


public slots:
    void zoomField(int scale); // zooms the camera
    void defaultZoom(); // resets the zoom to default value

    // following slots are meant to be accessible by gui_interface to allow drawing on screen
    void setupLine(Point start, Point stop, double seconds); // adds a line to be drawn
    void setupPoint(Point p); // adds a point to be drawn
    void setupRegion(std::vector<Point>); // sets the region to be drawn

signals:
    void field_mouse_moved(QPointF);

private:

    // scene and graphic objects to be drawn
    QGraphicsScene*       scene;
    GraphicsBall*         ball_drawer;
    GraphicsRobot*        robot_drawers[2][MAX_ROBOTS_PER_TEAM];
    GraphicsRobotLabel*        robot_labels_drawers[2][MAX_ROBOTS_PER_TEAM];
    GraphicsField*        field_drawer;
    GraphicsOutterField*  outter_field_drawer;
    GraphicsPoints*       point_drawer;
    deque<GraphicsLine*>  line_drawers;
    GraphicsPolygon*      region_drawer;

    // drawing functinos
    void drawLine(); // adds newly added lines to the drawing scene
    void drawPoint(); // adds newly added points to the drawing scene
    void drawRegion(); // adds the stored polygon to the scene
    void updateLineQueue(); // removes aged lines


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
    int currentFieldAngle = 0; // used to rotate the view, no longer used, left it in case it needs to be reimplemented


    // User interaction
    bool eventFilter(QObject* obj, QEvent* e); // filters child events that need to be processed by this frame


private slots:

    void setFollowingRobot(GuiRobot* robot); // sets a robot to be followed by the 'camera'
    void clearFollowing(); // clears the robot being followed



    void on_combo_robot_scale_currentIndexChanged(const QString &arg1);
    void on_combo_ball_scale_currentIndexChanged(const QString &arg1);
    void on_check_show_ids_stateChanged(int arg1);
    void on_combo_ball_color_currentIndexChanged(const QString &arg1);
    void on_check_field_grid_stateChanged(int arg1);
    void on_combo_grid_scale_currentIndexChanged(const QString &arg1);
    void on_combo_field_color_currentIndexChanged(const QString &arg1);

};

#endif // PANE_LFIELD_H
