#include "fieldpanel.h"
#include "objectposition.h"
#include "robotpanel.h"
#include "selrobotpanel.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "guiinterface.h"

//Helper classes
#include "guirobot.h"
#include "guiscene.h"
#include "guiball.h"
#include "guibotlabel.h"
#include "guidrawline.h"
#include "guidrawpoint.h"
#include "guidrawregion.h"
#include "guifield.h"
#include "src/model/game_state.h"
#include "src/model/field.h"
#include <vector>


FieldPanel::FieldPanel(MainWindow * mw) {
    dash = mw;
    region_drawer = nullptr;
    point_drawer = nullptr;
}

void FieldPanel::setupScene() {

    dash->ui->gView_field->setBackgroundBrush(QColor::fromRgb(30,30,30,255));

    scene = new GuiScene();

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
//    field->highlighted = false;
//    field->Pressed = false;

    // Creating the ball
    ball = new GuiBall;
    ball->setToolTip("Ball");



    //Bot Labels - blue team
    for(int team=0; team<2; team++)
        for(int robot_id=0; robot_id<MAX_ROBOTS; robot_id++){
            gui_robots[team][robot_id] = new GuiRobot(dash, team, robot_id);
            gui_bot_labels[team][robot_id] = new GuiBotLabel(team, robot_id );

        }


    // Add elements to scene:
    scene->addItem(field);
    // add bots and then bot labels
    for(int color=0; color<2; color++)
        for(int robot_id=0; robot_id<MAX_ROBOTS; robot_id++)
            scene->addItem(gui_robots[color][robot_id]);
    for(int color=0; color<2; color++)
        for(int robot_id=0; robot_id<MAX_ROBOTS; robot_id++)
            scene->addItem(gui_bot_labels[color][robot_id]);
    scene->addItem(sidelines);
    scene->addItem(ball);


    // Turning on Bot IDs by default
    dash->ui->check_showIDs->setChecked(true);

    // Raising the curtains...
    dash->ui->gView_field->setScene(scene);

    // Refreshes graphics view to eliminate glitchiness
    dash->ui->gView_field->hide();
    dash->ui->gView_field->show();


}// setupScene

void FieldPanel::updateScene() {
    if (refresh) {
        dash->ui->gView_field->hide();
        dash->ui->gView_field->show();
        refresh = false;
    }

    // Grid
    field->grid = dash->ui->check_fieldGrid->isChecked();

    auto scale = dash->ui->combo_gridScale->currentText();
    if ( scale == "200²")      field->gridScale = 100;
    else if (scale == "500²")  field->gridScale = 250;
    else if (scale == "1000²") field->gridScale = 500; // 1000x1000 unit sectors


    // Colored Goals
    field->coloredGoals = dash->ui->check_coloredGoals->isChecked();

    // Updating field/sideline colors
    sidelines->colorScheme = dash->ui->combo_fieldColor->currentText();
    field->colorScheme = dash->ui->combo_fieldColor->currentText();

    // updating the ball
        ball->setX(dash->objectPos->getBallCoordX());
        ball->setY(dash->objectPos->getBallCoordY());
        ball->setZValue(2);

        // Ball Scale
        auto ball_scale = dash->ui->combo_ballScale->currentText();
        if (ball_scale == "100%")      ball->setScale(.3);
        else if (ball_scale == "120%") ball->setScale(.5);
        else if (ball_scale == "150%") ball->setScale(.8);

        ball->color = dash->ui->combo_ballColor->currentText();

        // Tranformation matrix for robot ID labels
        QTransform flipLabel;
        flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);

    // Updating objects in scene
        for(int team=0 ; team < 2; team++)
            for(int robot_id=0; robot_id < MAX_ROBOTS; robot_id++){

                auto* gui_r = gui_robots[team][robot_id];
                auto* gui_l = gui_bot_labels[team][robot_id];
                auto* roboti = Team::getTeam(team)->getRobot(robot_id);

                if ( roboti != NULL) {
                    gui_r->show();

                    gui_r->setX(roboti->getPosition().x);
                    gui_r->setY(roboti->getPosition().y);
                    gui_r->setZValue(1);
                    gui_r->setRotation(dash->objectPos->getBotOrientDouble(roboti));

                    // Action colors (may be better in the button slots)
                    if (robot_id != selectedBot) {
                        gui_r->dribling = roboti->getDribble();
                        gui_r->kicking = roboti->getKick() == 1;
                    }

                    // Robot Scale
                    auto bot_scale = dash->ui->combo_botScale->currentText();
                    if ( bot_scale == "100%") gui_r->setScale(1);
                    else if (bot_scale == "120%") gui_r->setScale(1.2);
                    else if (bot_scale == "150%") gui_r->setScale(1.5);

                    // labels
                    gui_l->show();
                    gui_l->setTransform(flipLabel, false);
                    gui_l->setRotation(currentFieldAngle);
                    gui_l->setZValue(4);
                    gui_l->setX(roboti->getPosition().x);
                    gui_l->setY(roboti->getPosition().y);
                    gui_l->hidden = !dash->ui->check_showIDs->isChecked();

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

    dash->ui->gView_field->update();
}

void FieldPanel::scanForSelection() {
    bool newSelection = true;   // possibly pointless
    doubleClickScan();
    cameraMoveScan();
    // booleans (possibly uneccessarily)
    fieldClickScan();
    panelBotClickScan();
    fieldBotClickScan();

    if (newSelection) {
        dash->selrobotpanel->updateSelectedBotPanel(selectedBot);
    } else { return; }

}

void FieldPanel::centerViewOnBot() {
    // Centering camera on double-clicked bot
    if (centeredBotID!=nullptr) {
        dash->ui->gView_field->centerOn(centeredBotID);
    }
}

void FieldPanel::scanForScrollModifier() {
    // CTRL modifer for field scrolling
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        dash->ui->gView_field->setDragMode(QGraphicsView::ScrollHandDrag);
        justScrolled = true;
    } else {
        dash->ui->gView_field->setDragMode(QGraphicsView::NoDrag);
        if (justScrolled) {
            justScrolled = false;
            refresh = true;
        }
    }
}

void FieldPanel::setupLine(Point start, Point stop, double seconds) {
    // Adding data from game's thread to deques for future reference by our thread
    if (seconds > 0 && !hidePaths) {
        lineAPoints.push_front(start);
        lineBPoints.push_front(stop);
        lineLifeSpans.push_front(seconds);
    }
}

void FieldPanel::setupPoint(Point p) {
    // Adding data from game's thread to deques for future reference by our thread
    std::lock_guard<std::mutex> points_guard(points_mutex);
    simple_points.push_front(p);
}

void FieldPanel::setupRegion(std::vector<Point> p_vec) {
    // Adding data from game's thread to deques for future reference by our thread
    region_points.insert(region_points.begin(), p_vec.begin(), p_vec.end());
}

void FieldPanel::drawLine() {
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

void FieldPanel::drawPoint() {
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

void FieldPanel::drawRegion() {
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

void FieldPanel::updateLineQueue() {
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

void FieldPanel::doubleClickScan() {
    // Scanning for double-click selection
    int team_id = dash->getSelectedTeamId();
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->getSelectedTeam()->getRobot(i) != NULL) {
            if (dash->robotpanel->robotIcon[i]->doubleClicked || gui_robots[team_id][i]->doubleClicked)  {
                dash->robotpanel->robotIcon[i]->doubleClicked = false;
                gui_robots[team_id][i]->doubleClicked = false;
                centeredBotID = gui_robots[team_id][i];
                centerViewOnBot();
                zoomField(20);
                dash->guiPrint("Focused on Robot " + std::to_string(i));
                break;
            }
        }//nullcheck
    }//end for
}

void FieldPanel::cameraMoveScan() {
    // Scrolling the camera removes centeredOn but not selection
    int team_id = dash->getSelectedTeamId();
    if (justScrolled) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->getSelectedTeam()->getRobot(i) != NULL) {
                dash->robotpanel->robotIcon[i]->doubleClicked = false;
                gui_robots[team_id][i]->doubleClicked = false;
            }//nullcheck
        }
        centeredBotID = nullptr;
    }
}

bool FieldPanel::fieldClickScan() {
    // Field/Sidelines clicked removes centeredOn and selection
    if (field->Pressed == true) {
        field->highlighted = true;
        field->Pressed = false;
    }
    if (sidelines->Pressed == true) {
        sidelines->highlighted = true;
        sidelines->Pressed = false;
    }

    int team_id = dash->getSelectedTeamId();
    if (field->highlighted || sidelines->highlighted) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->getSelectedTeam()->getRobot(i) != NULL) {
                gui_robots[team_id][i]->highlighted = false;
                gui_robots[team_id][i]->setSelected(false);
                dash->robotpanel->robotIcon[i]->highlighted = false;
                dash->robotpanel->robotIcon[i]->setSelected(false);
                dash->robotpanel->robotIcon[i]->doubleClicked = false;
                gui_robots[team_id][i]->doubleClicked = false;
            }//nullcheck
        }
        field->highlighted = false;
        sidelines->highlighted = false;
        selectedBot = -1;
        centeredBotID = nullptr;

    }
     return true;
}

bool FieldPanel::panelBotClickScan() {
    int team_id = dash->getSelectedTeamId();
    for (int i=0; i<6; i++) {
        if (dash->getSelectedTeam()->getRobot(i) != NULL) {
            // Bots on the panel clicked
            if (dash->robotpanel->robotIcon[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<dash->teamSize_blue; j++) {
                    gui_robots[team_id][j]->highlighted = false;
                    gui_robots[team_id][j]->setSelected(false);
                    dash->robotpanel->robotIcon[j]->highlighted = false;
                    dash->robotpanel->robotIcon[j]->setSelected(false);

                }
                dash->robotpanel->robotIcon[i]->highlighted = true;
                gui_robots[team_id][i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<6; r++) {
                    dash->robotpanel->botIconFrames[i]->update();
                }
                dash->ui->gView_robot_prime->hide();
                dash->ui->gView_robot_prime->show();
//                newSelection = true;
                return true;
            }
        }
    }
     return true;
}

bool FieldPanel::fieldBotClickScan() {
    int team_id = dash->getSelectedTeamId();
    for (int i=0; i<6; i++) {
        if (dash->getSelectedTeam()->getRobot(i) != NULL) {
            // Bots on the field clicked
            if (gui_robots[team_id][i]->isSelected()) {
                selectedBot = i;
                dash->robotpanel->scrollToSelBot(i);
                for (int j=0; j<dash->teamSize_blue; j++) {
                    gui_robots[team_id][j]->highlighted = false;
                    gui_robots[team_id][j]->setSelected(false);
                    dash->robotpanel->robotIcon[j]->highlighted = false;
                    dash->robotpanel->robotIcon[j]->setSelected(false);
                }
                dash->robotpanel->robotIcon[i]->highlighted = true;
                gui_robots[team_id][i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<dash->teamSize_blue; r++) {
                    dash->robotpanel->botIconFrames[i]->update();
                }
                dash->ui->gView_robot_prime->hide();
                dash->ui->gView_robot_prime->show();
//                newSelection = true;
                return true;
            }
        }
    }
     return true;
}

void FieldPanel::zoomField(int zoom) {
    dash->ui->zoom_slider->setValue(zoom);
    double zoomScale = zoom *.01;
    dash->ui->gView_field->setTransform(QTransform::fromScale(zoomScale, zoomScale));
    dash->ui->gView_field->scale(1, -1);
    dash->ui->gView_field->rotate(currentFieldAngle);
}

void FieldPanel::defaultZoom() {
    // Removing robot focus
    centeredBotID = nullptr;

    currentFieldAngle = 0;
    zoomField(11);
    dash->ui->zoom_slider->setValue(11);
//    dash->ui->gView_field->hide();
    dash->ui->gView_field->centerOn(sidelines);
}

void FieldPanel::hidePrimeBotPanel()
{
    dash->ui->frame_primeBot->hide();
    selectedBot = -1;
}

void FieldPanel::updateTeamSelected(){
    if(selectedBot==-1) return;

    int team_id = dash->getSelectedTeamId();

    // deselect bot in old team, select bot in new team
    gui_robots[1-team_id][selectedBot]->highlighted =false;
    gui_robots[team_id][selectedBot]->highlighted =true;


}

