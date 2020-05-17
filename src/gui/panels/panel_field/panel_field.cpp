#include "gui/panels/robotpanel.h"
#include "panel_field.h"
#include "ui_panel_field.h"
#include "gui/mainwindow.h"
#include "gui/guisidelines.h"
#include "gui/data/guifield.h"
#include "gui/data/guibotlabel.h"
#include "gui/data/guiball.h"
#include "gui/utils/gui_robot_drawer.h"
#include "gui/utils/guidrawline.h"
#include "gui/utils/guidrawpoint.h"
#include "gui/utils/guidrawregion.h"

#include "model/field.h"

PanelField::PanelField(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    region_drawer = nullptr;
    point_drawer = nullptr;
    dash = (MainWindow*)parent;


    check_showIDs->setChecked(true);


    gView_field->setBackgroundBrush(QColor::fromRgb(30,30,30,255));
    scene = new QGraphicsScene();

    // Creating the sidelines
    sidelines = new GuiSidelines();
    sidelines->setX(-5200);         // Offsets are 50% of the item's height & width
    sidelines->setY(-3700);
    sidelines->setZValue(0);

    // Creating the field
    field = new GuiField();
    field->setX(-(Field::HALF_FIELD_LENGTH-100));
    field->setY(-(Field::HALF_FIELD_WIDTH-100)); // Y seems to be 100 off (?)
    field->setZValue(1);
    field->grid = false;

    //Bot Labels - blue team
    for(int team=0; team<2; team++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++){
            gui_bot_labels[team][robot_id] = new GuiBotLabel(team, robot_id );
            robot_drawers[team][robot_id] = new GuiRobotDrawer(team,robot_id);

        }

    // Add elements to scene:
    scene->addItem(field);

    // add bots and then bot labels
    for(int color=0; color<2; color++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++)
            scene->addItem(robot_drawers[color][robot_id]);
    for(int color=0; color<2; color++)
        for(int robot_id=0; robot_id<MAX_ROBOTS_PER_TEAM; robot_id++)
            scene->addItem(gui_bot_labels[color][robot_id]);
    scene->addItem(sidelines);
    scene->addItem(&GuiBall::ball);

    gView_field->setScene(scene);

    connect(zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(zoomField(int)));
    connect(zoom_default, SIGNAL(clicked()), this, SLOT(defaultZoom()));


    // Refreshes graphics view to eliminate glitchiness
    gView_field->hide();
    gView_field->show();

}

PanelField::~PanelField()
{
}




void PanelField::updateScene() {
    if (refresh) {
        gView_field->hide();
        gView_field->show();
        refresh = false;
    }

    // Grid
    field->grid = check_fieldGrid->isChecked();

    auto scale = combo_gridScale->currentText();
    if ( scale == "200²")      field->gridScale = 100;
    else if (scale == "500²")  field->gridScale = 250;
    else if (scale == "1000²") field->gridScale = 500; // 1000x1000 unit sectors


    // Colored Goals
    field->coloredGoals = check_coloredGoals->isChecked();

    // Updating field/sideline colors
    sidelines->colorScheme = combo_fieldColor->currentText();
    field->colorScheme = combo_fieldColor->currentText();

    // updating the ball
    auto ball = &GuiBall::ball;
    ball->setZValue(2);

    // Ball Scale
    auto ball_scale = combo_ballScale->currentText();
    if (ball_scale == "100%")      ball->setScale(.3);
    else if (ball_scale == "120%") ball->setScale(.5);
    else if (ball_scale == "150%") ball->setScale(.8);


    // Tranformation matrix for robot ID labels
    QTransform flipLabel;
    flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);

    // Updating objects in scene
    for(int team=0 ; team < 2; team++)
        for(int robot_id=0; robot_id < MAX_ROBOTS_PER_TEAM; robot_id++){

            auto& gui_r = robot_drawers[team][robot_id];
            auto* gui_l = gui_bot_labels[team][robot_id];
//                auto* roboti = RobotTeam::getTeam(team)->getRobot(robot_id);

            if ( gui_r->robot->hasProxy()) {
                gui_r->show();

                gui_r->setX(gui_r->robot->getCurrentPosition().x);
                gui_r->setY(gui_r->robot->getCurrentPosition().y);
                gui_r->setZValue(1);
                gui_r->setRotation(gui_r->robot->getOrientation());


                // Robot Scale
                auto bot_scale = combo_botScale->currentText();
                if ( bot_scale == "100%")     gui_r->setScale(1);
                else if (bot_scale == "120%") gui_r->setScale(1.2);
                else if (bot_scale == "150%") gui_r->setScale(1.5);

                // labels
                gui_l->show();
                gui_l->setTransform(flipLabel, false);
                gui_l->setRotation(currentFieldAngle);
                gui_l->setZValue(4);
                gui_l->setX(gui_r->robot->getCurrentPosition().x);
                gui_l->setY(gui_r->robot->getCurrentPosition().y);
                gui_l->hidden = !check_showIDs->isChecked();

            } else {
                //If there is no robot, we need to hide it.
                gui_r->hide();
                gui_l->hide();
            }

        }


    // Keeping camera centered
    centerViewOnBot();
    // Printing debug lines
    drawLine();
    drawPoint();
    drawRegion();
    updateLineQueue();

    gView_field->update();
}
void PanelField::scanForSelection() {
    bool newSelection = true;   // possibly pointless
    doubleClickScan();
    cameraMoveScan();
    // booleans (possibly uneccessarily)
    fieldClickScan();
    panelBotClickScan();
    fieldBotClickScan();

    if (newSelection) {
        dash->panel_selected_robot->update_selected_robot(dash->getSelectedTeamId(),selectedBot);
    } else { return; }
}

void PanelField::centerViewOnBot() {
    // Centering camera on double-clicked bot
    if (centeredBotID!=nullptr) {
        gView_field->centerOn(centeredBotID);
    }
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
    if (seconds > 0 && !hidePaths) {
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
            GuiDrawLine * newLine = new GuiDrawLine();
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
        point_drawer = new GuiDrawPoint();
    else
    {
        delete point_drawer;
        point_drawer = new GuiDrawPoint();
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
        region_drawer = new GuiDrawRegion();
    else
    {
        delete region_drawer;
        region_drawer = new GuiDrawRegion();
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

void PanelField::doubleClickScan() {
    // Scanning for double-click selection
    int team_id = dash->getSelectedTeamId();
    for (int i=0; i<dash->teamSize_blue; i++) {
        auto drawer = robot_drawers[team_id][i];
        auto r = drawer->robot;
        if (r->hasProxy()) {
            if (r->doubleClicked)  {
                r->doubleClicked = false;
                centeredBotID = drawer;
                centerViewOnBot();
                zoomField(20);
                dash->guiPrint("Focused on Robot " + std::to_string(i));
                break;
            }
        }//nullcheck
    }//end for
}

void PanelField::cameraMoveScan() {
    // Scrolling the camera removes centeredOn but not selection
    int team_id = dash->getSelectedTeamId();
    if (justScrolled) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            robot_drawers[team_id][i]->robot->doubleClicked = false;

        }
        centeredBotID = nullptr;
    }
}

bool PanelField::fieldClickScan() {
    // Field/Sidelines clicked removes centeredOn and selection
    if (field->Pressed == true) {
        field->highlighted = true;
        field->Pressed = false;
    }
    if (sidelines->Pressed == true) {
        sidelines->highlighted = true;
        sidelines->Pressed = false;
    }

    if (field->highlighted || sidelines->highlighted) {
        int team_id = dash->getSelectedTeamId();
        for (int i=0; i<dash->teamSize_blue; i++) {
            auto r = robot_drawers[team_id][i]->robot;
            if (r->hasProxy()) {
                r->highlighted = false;
                r->doubleClicked = false;
                // r->setSelected(false);
            }//nullcheck
        }
        field->highlighted = false;
        sidelines->highlighted = false;
        selectedBot = -1;
        centeredBotID = nullptr;

    }
     return true;
}
using std::cout;
bool PanelField::panelBotClickScan() {
    int team_id = dash->getSelectedTeamId();
    cout << "scan2 - 1\n";
    for (int i=0; i<6; i++) {
        cout << "scan2 - 2 - " << i << std::endl;
        auto drawer = dash->robotpanel->robotIcon[i];
        if (drawer->isSelected()) {
            drawer->setSelected(false);
            if(selectRobot(team_id,i)) return true;
        }
    }
    cout << "scan2 - 3\n";
     return false;
}

bool PanelField::fieldBotClickScan() {
    int team_id = dash->getSelectedTeamId();
    for (int i=0; i<6; i++) {
        auto drawer = robot_drawers[team_id][i];
        if(drawer->isSelected()){
            drawer->setSelected(false);
            if(selectRobot(team_id,i)) return true;
        }
    }
     return false;
}

bool PanelField::selectRobot(int team, int robot){
    auto& r = robot_drawers[team][robot]->robot;
    if (r->hasProxy()) {
        selectedBot = robot;
        dash->robotpanel->scrollToSelBot(robot);
        for (int j=0; j<dash->teamSize_blue; j++) {
            auto r_j = robot_drawers[team][j]->robot;
            r_j->highlighted = false;
        }
        r->highlighted = true;
        refresh = true;
        // Refresh GUI
        for (int r=0; r<dash->teamSize_blue; r++) {
            dash->robotpanel->botIconFrames[robot]->update();
        }
        dash->panel_selected_robot->update_selected_robot(team,robot);
        return true;
    }
    return false;
}

void PanelField::zoomField(int zoom) {
    zoom_slider->setValue(zoom);
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
    zoom_slider->setValue(11);
//    dash->gView_field->hide();
    gView_field->centerOn(sidelines);
}

void PanelField::hidePrimeBotPanel()
{
    dash->panel_selected_robot->hidePanel();
    selectedBot = -1;
}

void PanelField::updateTeamSelected(){
    if(selectedBot==-1) return;

    int team_id = dash->getSelectedTeamId();

    // deselect bot in old team, select bot in new team
    robot_drawers[1-team_id][selectedBot]->robot->highlighted =false;
    robot_drawers[team_id][selectedBot]->robot->highlighted =true;


}
