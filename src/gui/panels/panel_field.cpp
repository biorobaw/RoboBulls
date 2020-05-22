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
#include "gui/graphics/graphics_ball.h"

PanelField::PanelField(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);

    gView_field->verticalScrollBar()->installEventFilter(this);
    gView_field->setBackgroundBrush(QColor::fromRgb(30,30,30,255));
    check_show_ids->setChecked(true);


    region_drawer = nullptr;
    point_drawer = nullptr;
    dash = (MainWindow*)parent;



    // create a scene and set it in the graphics view
    scene = new QGraphicsScene();
    scene->installEventFilter(this);
    gView_field->setScene(scene);

    // Create field, sideline, and ball graphics
    scene->addItem(field = new GraphicsField());
    scene->addItem(sidelines = new GraphicsOutterField());
    scene->addItem(ball_drawer = new GraphicsBall(false));
    ball_drawer->setZValue(2);
    ball_drawer->show();


    // create robot graphics
    for(int team=0; team<2; team++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++){
            auto label = new GraphicsLabel(team, robot_id );
            gui_bot_labels[team][robot_id] = label;
            label->setZValue(4);
            label->setTransform(QTransform(1,0,0,0,-1,0,0,200,1), false);
            label->show();
            scene->addItem(label);

            auto robot = new GraphicsRobot(team,robot_id,false);
            robot_drawers[team][robot_id] = robot;
            robot->setZValue(1);
            robot->show();
            scene->addItem(robot);

        }


    // connect signals
    connect(slider_zoom, SIGNAL(valueChanged(int)), this, SLOT(zoomField(int)));
    connect(btn_zoom_default, SIGNAL(clicked()), this, SLOT(defaultZoom()));


    // Refreshes graphics view to eliminate glitchiness
    gView_field->show();

    defaultZoom(); // set default zoom after finishing initialization

}

PanelField::~PanelField()
{
}




void PanelField::updateScene() {

    // Grid
    field->grid = check_field_grid->isChecked();
    auto scale = combo_grid_scale->currentText();
    field->gridScale = scale == "200²" ? 100 : scale == "500²" ? 250 : 500;
//    field->coloredGoals = check_coloredGoals->isChecked();


    // Colored Goals

    // Updating field/sideline colors
    sidelines->colorScheme = combo_field_color->currentText();
    field->colorScheme = combo_field_color->currentText();

    // Ball Scale
    auto bs_str = combo_ball_scale->currentText();
    ball_drawer->setScale(bs_str == "150%" ? 1.5 : bs_str == "120%" ? 1.2 : 1.0);


    // Tranformation matrix for robot ID labels

//    flipLabel.setMatrix();

    auto s_str = combo_robot_scale->currentText();
    float robot_scale = s_str == "150%" ? 1.5 : s_str == "120%" ? 1.2 : 1.0;

    // Updating objects in scene
    for(int team=0 ; team < 2; team++)
        for(int robot_id=0; robot_id < MAX_ROBOTS_PER_TEAM; robot_id++){

            auto& gui_r = robot_drawers[team][robot_id];
            gui_r->setScale(robot_scale);


            auto* gui_l = gui_bot_labels[team][robot_id];

            gui_l->hidden = !check_show_ids->isChecked();

        }


    // Keeping camera centered
    // Centering camera on double-clicked bot
    if (centeredBotID!=nullptr) gView_field->centerOn(centeredBotID);
    // Printing debug lines
    drawLine();
    drawPoint();
    drawRegion();
    updateLineQueue();

    gView_field->update();


    // TODO:
    // there where issues with the conditional refresh, thus we are refreshing all
    // then we will improve it
    if (refresh || true) {
        gView_field->hide();
        gView_field->show();
        refresh = false;
    }

}
void PanelField::scanForSelection() {
    bool newSelection = true;   // possibly pointless
//    doubleClickScan();
//    cameraMoveScan();
    // booleans (possibly uneccessarily)
//    fieldClickScan();
//    panelBotClickScan();
//    fieldBotClickScan();

    if (newSelection) {
        dash->panel_selected_robot->update_selected_robot();
    } else { return; }
}


void PanelField::scanForScrollModifier() {
    // CTRL modifer for field scrolling
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        gView_field->setDragMode(QGraphicsView::ScrollHandDrag);
        justScrolled = true;
    } else {
        gView_field->setDragMode(QGraphicsView::NoDrag);
        if (justScrolled) {
            justScrolled = false;
            refresh = true;
        }
    }
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
        int newLines = lineAPoints.size() - lineQueue.size();
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
            lineQueue.push_front(newLine);
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
    if (lineQueue.size() > 0) {
        for (unsigned int i=0; i<lineQueue.size(); i++) {
            if (lineQueue[i] != NULL) {
                // refreshing scene to prevent visual glitches
                refresh = true;
                // Deleting old lines
                if (lineQueue[i]->age == 0) {
                    lineQueue[i]->hide();
                    delete lineQueue[i];
                    lineQueue.erase(lineQueue.begin()+i);
                }
            }
        }
        // Clearing our data deques
        lineAPoints.clear();
        lineBPoints.clear();
        lineLifeSpans.clear();
    }
}

//void PanelField::doubleClickScan() {
//    // Scanning for double-click selection
//    int team_id = dash->getSelectedTeamId();
//    for (int i=0; i<dash->teamSize_blue; i++) {
//        auto drawer = robot_drawers[team_id][i];
//        auto r = drawer->robot;
//        if (r->hasProxy()) {
//            if (r->doubleClicked)  {
//                r->doubleClicked = false;
//                centeredBotID = drawer;
//                centerViewOnBot();
//                zoomField(20);
//                // dash->guiPrint("Focused on Robot " + std::to_string(i));
//                break;
//            }
//        }//nullcheck
//    }//end for
//}

//void PanelField::cameraMoveScan() {
//    // Scrolling the camera removes centeredOn but not selection
//    int team_id = dash->getSelectedTeamId();
//    if (justScrolled) {
//        for (int i=0; i<dash->teamSize_blue; i++) {
//            robot_drawers[team_id][i]->robot->doubleClicked = false;

//        }
//        centeredBotID = nullptr;
//    }
//}

//bool PanelField::fieldClickScan() {
//    // Field/Sidelines clicked removes centeredOn and selection
//    if (field->Pressed == true) {
//        field->highlighted = true;
//        field->Pressed = false;
//    }
//    if (sidelines->Pressed == true) {
//        sidelines->highlighted = true;
//        sidelines->Pressed = false;
//    }

//    if (field->highlighted || sidelines->highlighted) {
//        int team_id = dash->getSelectedTeamId();
//        for (int i=0; i<dash->teamSize_blue; i++) {
//            auto r = robot_drawers[team_id][i]->robot;
//            if (r->hasProxy()) {
//                r->highlighted = false;
//                r->doubleClicked = false;
//                // r->setSelected(false);
//            }//nullcheck
//        }
//        field->highlighted = false;
//        sidelines->highlighted = false;
//        selectedBot = -1;
//        centeredBotID = nullptr;

//    }
//     return true;
//    return true;
//}
//using std::cout;
//bool PanelField::panelBotClickScan() {
//    int team_id = dash->getSelectedTeamId();
//    cout << "scan2 - 1\n";
//    for (int i=0; i<6; i++) {
//        cout << "scan2 - 2 - " << i << std::endl;
//        auto drawer = dash->robotpanel->robotIcon[i];
//        if (drawer->isSelected()) {
//            drawer->setSelected(false);
//            if(selectRobot(team_id,i)) return true;
//        }
//    }
//    cout << "scan2 - 3\n";
//     return false;
//}

//bool PanelField::fieldBotClickScan() {
//    int team_id = dash->getSelectedTeamId();
//    for (int i=0; i<6; i++) {
//        auto drawer = robot_drawers[team_id][i];
//        if(drawer->isSelected()){
//            drawer->setSelected(false);
//            if(selectRobot(team_id,i)) return true;
//        }
//    }
//     return false;
//}

void PanelField::selectRobot(int team, int robot){
//    auto& r = robot_drawers[team][robot]->robot;
//    if (r->hasProxy()) {
//        selectedBot = robot;
//        dash->robotpanel->scrollToSelBot(robot);
//        for (int j=0; j<dash->teamSize_blue; j++) {
//            auto r_j = robot_drawers[team][j]->robot;
//            r_j->highlighted = false;
//        }
//        r->highlighted = true;
//        refresh = true;
//        // Refresh GUI
//        for (int r=0; r<dash->teamSize_blue; r++) {
//            dash->robotpanel->botIconFrames[robot]->update();
//        }
//        dash->panel_selected_robot->update_selected_robot(team,robot);
//        return true;
//    }
//    return false;
}

void PanelField::zoomField(int zoom) {
    slider_zoom->setValue(zoom);
    double zoomScale = zoom *.01;
    gView_field->setTransform(QTransform::fromScale(zoomScale, zoomScale));
    gView_field->scale(1, -1);
    gView_field->rotate(currentFieldAngle);
}

void PanelField::defaultZoom() {
    // Removing robot focus
    centeredBotID = nullptr;

    currentFieldAngle = 0;
    zoomField(11);
//    zoom_slider->setValue(11);
//    dash->gView_field->hide();
    gView_field->centerOn(sidelines);
}



bool PanelField::eventFilter(QObject* obj, QEvent* e){
    if( obj == gView_field->verticalScrollBar() &&
            e->type() == QEvent::Wheel &&
            QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){

//        auto mouse = gView_field->mapFromGlobal(QCursor::pos());
//        auto mouse_relative = gView_field->mapToScene(mouse);
//        gView_field->centerOn(mouse_relative.rx(), mouse_relative.ry());

        int delta = ((QWheelEvent*)e)->delta() > 0 ? 2 : -2;
        slider_zoom->setValue(slider_zoom->value()+delta);

        return true;
    } else if ( obj == scene && e->type() == QEvent::GraphicsSceneMouseMove ){
        auto mouse = gView_field->mapFromGlobal(QCursor::pos());
        emit field_mouse_moved(gView_field->mapToScene(mouse) - QPointF(100,100));
    }

    return false;
}


void PanelField::setHidePaths(bool val){
    hide_paths = val;
}



// Field graphical settings which need to be refreshed when changed
void PanelField::on_check_fieldGrid_clicked(){
    refresh = true;
}
void PanelField::on_combo_gridScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    refresh = true;
}
void PanelField::on_check_coloredGoals_clicked(){
    refresh = true;
}
void PanelField::on_combo_fieldColor_currentIndexChanged(int index){
    Q_UNUSED(index);
    refresh = true;
}
void PanelField::on_check_showIDs_stateChanged(int arg1){
    Q_UNUSED(arg1);
    refresh = true;
}
void PanelField::on_combo_botScale_currentIndexChanged(int index){
    Q_UNUSED(index);
    refresh = true;
}

