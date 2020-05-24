#include "panel_field.h"
#include "gui/main_window.h"
#include "gui/graphics/graphics_outter_field.h"
#include "gui/graphics/graphics_field.h"
#include "gui/graphics/graphics_robot_label.h"
#include "gui/data/gui_ball.h"
#include "gui/graphics/graphics_robot.h"
#include "gui/graphics/graphics_line.h"
#include "gui/graphics/graphics_points.h"
#include "gui/graphics/graphics_polygon.h"
#include <QScrollBar>
#include <QWheelEvent>
#include "gui/graphics/graphics_ball.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include "gui/data/gui_robot.h"

using std::cout, std::endl;

PanelField::PanelField(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);


    // FIRST CREATE SCENE AND GRAPHICS
    region_drawer = nullptr;
    point_drawer = nullptr;

    scene = new QGraphicsScene;
    gView_field->setScene(scene);
    gView_field->setBackgroundBrush(QColor::fromRgb(30,30,30,255));

    scene->addItem(outter_field_drawer = new GraphicsOutterField());
    scene->addItem(field_drawer = new GraphicsField());
    scene->addItem(ball_drawer = new GraphicsBall(false));

    // create robot graphics and their labels
    for(int team=0; team<2; team++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++){
            auto label = new GraphicsLabel(team, robot_id );
            scene->addItem(robot_labels_drawers[team][robot_id] = label);

            auto robot = new GraphicsRobot(this,team,robot_id,false);
            scene->addItem(robot_drawers[team][robot_id] = robot);

        }




    // THEN CONNECT ALL SIGNALS AND EVENT MANAGERS

    gView_field->verticalScrollBar()->installEventFilter(this);
    scene->installEventFilter(this);
    for(int team=0; team<2; team++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++){
            connect(GuiRobot::get(team,robot_id),SIGNAL(doubleClicked(GuiRobot*)),this,SLOT(setFollowingRobot(GuiRobot*)));
        }

    connect(slider_zoom, SIGNAL(valueChanged(int)), this, SLOT(zoomField(int)));
    connect(btn_zoom_default, SIGNAL(clicked()), this, SLOT(defaultZoom()));
    connect(gView_field->verticalScrollBar(),SIGNAL(actionTriggered(int)),this,SLOT(clearFollowing()));
    connect(gView_field->horizontalScrollBar(),SIGNAL(actionTriggered(int)),this,SLOT(clearFollowing()));


    // FINALLY SET DEFAULT VALUES AND SHOW PANEL
//    check_show_ids->setChecked(true);
    defaultZoom(); // set default zoom after finishing initialization
    gView_field->show();




}

PanelField::~PanelField()
{
}




void PanelField::updateScene() {


    // Keeping camera centered
    // Centering camera on double-clicked bot

    // Printing debug lines
    drawLine();
    drawPoint();
    drawRegion();
    updateLineQueue();

    // the following robot doesn't match the selected robot, it has been deselected:
    // else, if following a robot center the camera
    if(followingRobot && GuiRobot::get_selected_robot() != followingRobot->getRobot()){
        clearFollowing();
    } else if(followingRobot!=nullptr) {
        gView_field->centerOn(followingRobot);
    }

    // here we invalidate whole scene, ideally we should only invalidate
    // areas around objects that are changing (like robot)
    gView_field->invalidateScene();


}

void PanelField::setupLine(Point start, Point stop, double seconds) {
    // Adding data from game's thread to deques for future reference by our thread
    if (seconds > 0 && !hide_paths) {
        lineAPoints.push_front(start);
        lineBPoints.push_front(stop);
        lineLifeSpans.push_front(seconds);
    }
}

void PanelField::setupPoint(Point p) {
    // Adding data from game's thread to deques for future reference by our thread
    std::lock_guard<std::mutex> points_guard(points_mutex);
    simple_points.push_front(p);
}

void PanelField::setupRegion(std::vector<Point> p_vec) {
    // Adding data from game's thread to deques for future reference by our thread
    region_points.insert(region_points.begin(), p_vec.begin(), p_vec.end());
}

void PanelField::drawLine() {
    // Creating a new line based on received specs
    if (lineAPoints.size() > 0) {
        int startIter = 0;
        int newLines = lineAPoints.size() - line_drawers.size();
        if (newLines > 0) {
            startIter = lineAPoints.size() - newLines;
        }
        for (unsigned int i=startIter; i<lineAPoints.size(); i++)
        {
            GraphicsLine * newLine = new GraphicsLine();
            newLine->x1 = lineAPoints[i].x;
            newLine->y1 = lineAPoints[i].y;
            newLine->x2 = lineBPoints[i].x;
            newLine->y2 = lineBPoints[i].y;
            newLine->lifeSpan = lineLifeSpans[i];
            newLine->setZValue(3);
            newLine->setX(100);
            newLine->setY(100);
            newLine->ageLine();

            // adding our line to the scene and to the aging queue
            scene->addItem(newLine);
            line_drawers.push_front(newLine);
        }
    }
}

void PanelField::drawPoint() {
    if(point_drawer == nullptr)
        point_drawer = new GraphicsPoints();
    else
    {
        delete point_drawer;
        point_drawer = new GraphicsPoints();
    }

    std::lock_guard<std::mutex> points_guard(points_mutex);

    for (Point p : simple_points)
        point_drawer->points.push_back(p);
    simple_points.clear();

    point_drawer->setZValue(2);
    point_drawer->setX(100);
    point_drawer->setY(100);
    scene->addItem(point_drawer);
}

void PanelField::drawRegion() {
    if(region_drawer == nullptr)
        region_drawer = new GraphicsPolygon();
    else
    {
        delete region_drawer;
        region_drawer = new GraphicsPolygon();
    }

    for (Point p : region_points)
        region_drawer->Q_polygon.push_back(QPoint(p.x, p.y));

    region_points.clear();

    region_drawer->setZValue(2);
    region_drawer->setX(100);
    region_drawer->setY(100);
    scene->addItem(region_drawer);
}

void PanelField::updateLineQueue() {
    // Refreshing the field if there are lines to draw
    if (line_drawers.size() > 0) {
        for (unsigned int i=0; i<line_drawers.size(); i++) {
            if (line_drawers[i] != NULL) {
                // refreshing scene to prevent visual glitches
                refresh = true;
                // Deleting old lines
                if (line_drawers[i]->age == 0) {
                    line_drawers[i]->hide();
                    delete line_drawers[i];
                    line_drawers.erase(line_drawers.begin()+i);
                }
            }
        }
        // Clearing our data deques
        lineAPoints.clear();
        lineBPoints.clear();
        lineLifeSpans.clear();
    }
}


void PanelField::setHidePaths(bool val){
    hide_paths = val;
}





// =================== EVENT PROCESSING ================================
// =====================================================================

void PanelField::setDragMode(QGraphicsView::DragMode mode){
    gView_field->setDragMode(mode);

}


bool PanelField::eventFilter(QObject* obj, QEvent* e){



    if( obj == gView_field->verticalScrollBar() ){
        // control + wheel = zoom (instead of scroll, thus we filter the event)
        if(e->type() == QEvent::Wheel && QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){

            int delta = ((QWheelEvent*)e)->delta() > 0 ? 2 : -2;
            slider_zoom->setValue(slider_zoom->value()+delta);
            return true; // return true to filter the event and prevent scrolling

        }

    } else if ( obj == scene ) {
        // mouse moves = signal current scene coordinates
        if(e->type() == QEvent::GraphicsSceneMouseMove){
            auto mouse = gView_field->mapFromGlobal(QCursor::pos());
            emit field_mouse_moved(gView_field->mapToScene(mouse) - QPointF(100,100));

        // if mouse press
        } else if(e->type()==QEvent::GraphicsSceneMousePress){

            // if control: clear only following robot, otherwise clear selected robot
            if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
                clearFollowing();
            else GuiRobot::clearSelected();
        }

    }


    return false;
}







// =================== OBJECT SLOTS ====================================
// =====================================================================


void PanelField::setFollowingRobot(GuiRobot* robot){
    cout << (robot == nullptr ? "Stop following robot..." : "Start following robot...") <<endl;
    if(robot){
        auto drawer = robot_drawers[robot->team][robot->id];
        if( followingRobot != drawer){
            zoomField(20);
            followingRobot = drawer;
        }

    } else followingRobot=nullptr;
}

void PanelField::clearFollowing(){
    setFollowingRobot(nullptr);
}

void PanelField::zoomField(int zoom) {
    slider_zoom->setValue(zoom);
    double zoomScale = zoom *.01;
    gView_field->setTransform(QTransform::fromScale(zoomScale, zoomScale));
    gView_field->scale(1, -1);
    gView_field->rotate(currentFieldAngle);
}

void PanelField::defaultZoom() {
    currentFieldAngle = 0;
    zoomField(11);
    gView_field->centerOn(outter_field_drawer);
}





// ================ COMPONENT SLOTS ====================================
// =====================================================================



// ROBOT RELATED

void PanelField::on_check_show_ids_stateChanged(int arg1)
{
    cout <<" SHOW ID: " << arg1 <<endl;
    for(int team=0 ; team < 2; team++)
        for(int robot_id=0; robot_id < MAX_ROBOTS_PER_TEAM; robot_id++)
            robot_labels_drawers[team][robot_id]->hidden = arg1==0;
}

void PanelField::on_combo_robot_scale_currentIndexChanged(const QString &arg1)
{
    float robot_scale = arg1 == "150%" ? 1.5 : arg1 == "120%" ? 1.2 : 1.0;
    for(int team=0 ; team < 2; team++)
        for(int robot_id=0; robot_id < MAX_ROBOTS_PER_TEAM; robot_id++)
            robot_drawers[team][robot_id]->setScale(robot_scale);
}

// BALL RELATED

void PanelField::on_combo_ball_color_currentIndexChanged(const QString &arg1)
{
    ball_drawer->setColor(arg1);
}

void PanelField::on_combo_ball_scale_currentIndexChanged(const QString &arg1)
{
    ball_drawer->setScale(arg1 == "150%" ? 1.5 : arg1 == "120%" ? 1.2 : 1.0);
}


// FIELD RELATED

void PanelField::on_check_field_grid_stateChanged(int arg1)
{
    field_drawer->grid = arg1 != 0;
}

void PanelField::on_combo_grid_scale_currentIndexChanged(const QString &arg1)
{
    field_drawer->gridScale = arg1 == "200²" ? 100 : arg1 == "500²" ? 250 : 500;
}



void PanelField::on_combo_field_color_currentIndexChanged(const QString &arg1)
{
    outter_field_drawer->colorScheme = arg1;
    field_drawer->colorScheme = arg1;
}




//    if(e->type() == QGraphicsSceneDragDropEvent::){
//        cout << "drag enter scene\n";
//    }

//    QString name = obj == gView_field ? "gview" :
//                   obj == scene ? "scene" :
//                   obj == gView_field->verticalScrollBar() ? "scroll bar" :
//                   obj == gView_field->viewport() ? "view port" : "other";

//    auto t = e->type();
//    if(t != QEvent::Hide & t!= QEvent::MetaCall & t!= QEvent::Paint & t!= QEvent::WindowActivate &&
//            obj == scene)
//        print_event(name,e);





//void print_event(QString base, QEvent* e){

//    qDebug() << "BASE: " << base << endl;
//    qDebug() << "Event: " << e << " " << e->type() << endl;

//    if(e->type() == QEvent::DragEnter){
//        qDebug() << "drag enter scene\n";
//    }
//    if(e->type() == QEvent::DragMove){
//        qDebug() << "drag move scene\n";
//    }
//    if(e->type() == QEvent::Drop){
//        qDebug() << "drag drop scene\n";
//    }
//    if(e->type() == QEvent::DragLeave){
//        qDebug() << "drag leave scene\n";
//    }

//    if(e->type() == QEvent::GraphicsSceneDragEnter){
//        qDebug() << "drag g enter scene\n";
//    }
//    if(e->type() == QEvent::GraphicsSceneDragMove){
//        qDebug() << "drag g move scene\n";
//    }
//    if(e->type() == QEvent::GraphicsSceneDragLeave){
//        qDebug() << "drag g leave scene\n";
//    }
//    if(e->type() == QEvent::DragResponse){
//        qDebug() << "drag response scene\n";
//    }
//    if(e->type()==QEvent::MouseButtonPress){
//        qDebug() << "Mouse press!!" << endl;
//    }


//}
