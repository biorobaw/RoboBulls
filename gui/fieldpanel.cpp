#include "fieldpanel.h"
#include "objectposition.h"
#include "robotpanel.h"
#include "selrobotpanel.h"
#include "mainwindow.h"
#include "include/config/team.h"
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
#include "model/gamemodel.h"
#include <vector>

FieldPanel::FieldPanel(MainWindow * mw) {
    dash = mw;
}

void FieldPanel::setupScene() {

    dash->ui->gView_field->setBackgroundBrush(QColor::fromRgb(30,30,30,255));

    scene = new GuiScene();

    // Creating the sidelines
    sidelines = new GuiSidelines();
    sidelines->setX(-4000);         // Offsets are 50% of the item's height & width
    sidelines->setY(-3000);
    sidelines->setZValue(0);

    // Creating the field
    field = new GuiField();
    field->setX(-2900);
    field->setY(-1900); // Y seems to be 100 off (?)
    field->setZValue(1);
    field->grid = false;
//    field->highlighted = false;
//    field->Pressed = false;

    // Creating the ball
    ball = new GuiBall;
    ball->setToolTip("Ball");

    //Bot Labels - blue team
    botLabel0 = new GuiBotLabel(TEAM_BLUE);
    botLabel1 = new GuiBotLabel(TEAM_BLUE);
    botLabel2 = new GuiBotLabel(TEAM_BLUE);
    botLabel3 = new GuiBotLabel(TEAM_BLUE);
    botLabel4 = new GuiBotLabel(TEAM_BLUE);
    botLabel5 = new GuiBotLabel(TEAM_BLUE);
    botLabel6 = new GuiBotLabel(TEAM_BLUE);
    botLabel7 = new GuiBotLabel(TEAM_BLUE);
    botLabel8 = new GuiBotLabel(TEAM_BLUE);
    botLabel9 = new GuiBotLabel(TEAM_BLUE);

    guiLabels.push_back(botLabel0);
    guiLabels.push_back(botLabel1);
    guiLabels.push_back(botLabel2);
    guiLabels.push_back(botLabel3);
    guiLabels.push_back(botLabel4);
    guiLabels.push_back(botLabel5);
    guiLabels.push_back(botLabel6);
    guiLabels.push_back(botLabel7);
    guiLabels.push_back(botLabel8);
    guiLabels.push_back(botLabel9);

    // Bot Labels - yellow team
    botLabel0Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel1Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel2Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel3Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel4Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel5Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel6Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel7Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel8Y = new GuiBotLabel(TEAM_YELLOW);
    botLabel9Y = new GuiBotLabel(TEAM_YELLOW);

    guiLabelsY.push_back(botLabel0Y);
    guiLabelsY.push_back(botLabel1Y);
    guiLabelsY.push_back(botLabel2Y);
    guiLabelsY.push_back(botLabel3Y);
    guiLabelsY.push_back(botLabel4Y);
    guiLabelsY.push_back(botLabel5Y);
    guiLabelsY.push_back(botLabel6Y);
    guiLabelsY.push_back(botLabel7Y);
    guiLabelsY.push_back(botLabel8Y);
    guiLabelsY.push_back(botLabel9Y);

    // Blue Team Robots
    robot0 = new GuiRobot();
    robot1 = new GuiRobot();
    robot2 = new GuiRobot();
    robot3 = new GuiRobot();
    robot4 = new GuiRobot();
    robot5 = new GuiRobot();
    robot6 = new GuiRobot();
    robot7 = new GuiRobot();
    robot8 = new GuiRobot();
    robot9 = new GuiRobot();

    guiTeam.push_back(robot0);
    guiTeam.push_back(robot1);
    guiTeam.push_back(robot2);
    guiTeam.push_back(robot3);
    guiTeam.push_back(robot4);
    guiTeam.push_back(robot5);
//    guiTeam.push_back(robot6);
//    guiTeam.push_back(robot7);
//    guiTeam.push_back(robot8);
//    guiTeam.push_back(robot9);

    // Yellow Team
    robot0Y = new GuiRobot();
    robot1Y = new GuiRobot();
    robot2Y = new GuiRobot();
    robot3Y = new GuiRobot();
    robot4Y = new GuiRobot();
    robot5Y = new GuiRobot();
    robot6Y = new GuiRobot();
    robot7Y = new GuiRobot();
    robot8Y = new GuiRobot();
    robot9Y = new GuiRobot();

    guiTeamY.push_back(robot0Y);
    guiTeamY.push_back(robot1Y);
    guiTeamY.push_back(robot2Y);
    guiTeamY.push_back(robot3Y);
    guiTeamY.push_back(robot4Y);
    guiTeamY.push_back(robot5Y);
//    guiTeamY.push_back(robot6Y);
//    guiTeamY.push_back(robot7Y);
//    guiTeamY.push_back(robot8Y);
//    guiTeamY.push_back(robot9Y);

    // Adding the previous gui items to the scene
    scene->addItem(field);
    scene->addItem(botLabel0);
    scene->addItem(botLabel1);
    scene->addItem(botLabel2);
    scene->addItem(botLabel3);
    scene->addItem(botLabel4);
    scene->addItem(botLabel5);
    scene->addItem(botLabel6);
    scene->addItem(botLabel7);
    scene->addItem(botLabel8);
    scene->addItem(botLabel9);

    scene->addItem(botLabel0Y);
    scene->addItem(botLabel1Y);
    scene->addItem(botLabel2Y);
    scene->addItem(botLabel3Y);
    scene->addItem(botLabel4Y);
    scene->addItem(botLabel5Y);
    scene->addItem(botLabel6Y);
    scene->addItem(botLabel7Y);
    scene->addItem(botLabel8Y);
    scene->addItem(botLabel9Y);

    scene->addItem(robot0);
    scene->addItem(robot1);
    scene->addItem(robot2);
    scene->addItem(robot3);
    scene->addItem(robot4);
    scene->addItem(robot5);
//    scene->addItem(robot6);
//    scene->addItem(robot7);
//    scene->addItem(robot8);
//    scene->addItem(robot9);

    scene->addItem(robot0Y);
    scene->addItem(robot1Y);
    scene->addItem(robot2Y);
    scene->addItem(robot3Y);
    scene->addItem(robot4Y);
    scene->addItem(robot5Y);
//    scene->addItem(robot6Y);
//    scene->addItem(robot7Y);
//    scene->addItem(robot8Y);
//    scene->addItem(robot9Y);

    scene->addItem(sidelines);
    scene->addItem(ball);


    // Setting static details for robots
    for (int i=0; i<dash->teamSize_blue; i++) {
        // Blue team
        guiTeam[i]->id = i;
        guiTeam[i]->setToolTip("Robot " + QString::number(i));
        guiLabels[i]->id = i;
        guiLabels[i]->setScale(2.5);
        if (OUR_TEAM == TEAM_BLUE) {
            guiTeam[i]->mainTeam = true;
            guiLabels[i]->mainTeam = true;
        } else {
            guiTeam[i]->mainTeam = false;
            guiLabels[i]->mainTeam = false;
        }
    }
    for (int i=0; i<dash->teamSize_yellow; i++) {
        // Yellow team
        guiTeamY[i]->id = i;
        guiTeamY[i]->setToolTip("Robot " + QString::number(i));
        guiLabelsY[i]->id = i;
        guiLabelsY[i]->setScale(2.5);
        if (OUR_TEAM == TEAM_YELLOW) {
            guiTeamY[i]->mainTeam = true;
            guiLabelsY[i]->mainTeam = true;
        } else {
            guiTeamY[i]->mainTeam = false;
            guiLabelsY[i]->mainTeam = false;
        }
    }

    //Set the team for the field; used only to draw goal colors correctly
    field->myTeam = (OUR_TEAM == TEAM_BLUE) ? "Blue" : "Yellow";

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
    if (dash->ui->check_fieldGrid->isChecked()) {
        field->grid = true;
    }else{ field->grid = false; };
    if (dash->ui->combo_gridScale->currentText() == "200²") {
        field->gridScale = 100;
    } else if (dash->ui->combo_gridScale->currentText() == "500²") {
        field->gridScale = 250;
    } else if (dash->ui->combo_gridScale->currentText() == "1000²") {
        field->gridScale = 500; // 1000x1000 unit sectors
    }
    // Colored Goals
    if (dash->ui->check_coloredGoals->isChecked()) {
        field->coloredGoals = true;
    }else{ field->coloredGoals = false;};

    // Updating field/sideline colors
    sidelines->colorScheme = dash->ui->combo_fieldColor->currentText();
    field->colorScheme = dash->ui->combo_fieldColor->currentText();

    // updating the ball
        ball->setX(dash->objectPos->getBallCoordX());
        ball->setY(dash->objectPos->getBallCoordY());
        ball->setZValue(2);
        // Ball Scale
        if (dash->ui->combo_ballScale->currentText() == "100%"){
            ball->setScale(.3);
        } else if (dash->ui->combo_ballScale->currentText() == "120%"){
            ball->setScale(.5);
        } else if (dash->ui->combo_ballScale->currentText() == "150%"){
            ball->setScale(.8);
        }
        ball->color = dash->ui->combo_ballColor->currentText();

        // Tranformation matrix for robot ID labels
        QTransform flipLabel;
        flipLabel.setMatrix(1,0,0,0,-1,0,0,200,1);

    // Updating objects in scene
        // Blue Team
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
                guiTeam[i]->show();
                guiLabels[i]->show();
                guiTeam[i]->setX(dash->objectPos->getBotCoordX(true, i));
                guiTeam[i]->setY(dash->objectPos->getBotCoordY(true, i));
                //if (guiTeam[i]->enabled) {
                 //   guiTeam[i]->setZValue(3);
                //} else {
                    guiTeam[i]->setZValue(1);
                //}
                double angle = dash->objectPos->getBotOrientDouble(true, i) ;
                guiTeam[i]->setRotation(angle);
                // Action colors (may be better in the button slots)
                if (i != selectedBot) {
                    if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam())->getDribble() ) {
                        guiTeam[i]->dribling = true;
                    } else { guiTeam[i]->dribling = false; }
                    if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam())->getKick() == 1) {
                        guiTeam[i]->kicking = true;
                    } else { guiTeam[i]->kicking = false; }
                }
                // Robot Scale
                if (dash->ui->combo_botScale->currentText() == "100%") {
                    guiTeam[i]->setScale(1);
                } else if (dash->ui->combo_botScale->currentText() == "120%") {
                    guiTeam[i]->setScale(1.2);
                } else if (dash->ui->combo_botScale->currentText() == "150%") {
                    guiTeam[i]->setScale(1.5);
                }
                // labels
                guiLabels[i]->setTransform(flipLabel, false);
                guiLabels[i]->setRotation(currentFieldAngle);
                guiLabels[i]->setZValue(4);
                guiLabels[i]->setX(dash->objectPos->getBotCoordX(true,i));
                guiLabels[i]->setY(dash->objectPos->getBotCoordY(true,i));
                if (dash->ui->check_showIDs->isChecked()) {
                    guiLabels[i]->hidden = false;
                }else{
                    guiLabels[i]->hidden = true;
                }
            } else {
                //If there is no robot, we need to hide it.
                guiTeam[i]->hide();
                guiLabels[i]->hide();
            }
        }// blue team for loop

        // Yellow Team
        for (int i=0; i<dash->teamSize_yellow; i++) {
            if (dash->gamemodel->find(i, dash->gamemodel->getOppTeam()) != NULL) {
                guiTeamY[i]->show();
                guiLabelsY[i]->show();
                guiTeamY[i]->setX(dash->objectPos->getBotCoordX(false, i));
                guiTeamY[i]->setY(dash->objectPos->getBotCoordY(false, i));
                guiTeamY[i]->setZValue(3);
                double angleY = dash->objectPos->getBotOrientDouble(false, i) ;
                guiTeamY[i]->setRotation(angleY);
                // Robot Scale
                if (dash->ui->combo_botScale->currentText() == "100%") {
                    guiTeamY[i]->setScale(1);
                } else if (dash->ui->combo_botScale->currentText() == "120%") {
                    guiTeamY[i]->setScale(1.2);
                } else if (dash->ui->combo_botScale->currentText() == "150%") {
                    guiTeamY[i]->setScale(1.5);
                }
                // labels
                guiLabelsY[i]->setTransform(flipLabel,false);
                guiLabelsY[i]->setRotation(currentFieldAngle);
                guiLabelsY[i]->setZValue(4);
                guiLabelsY[i]->setX(dash->objectPos->getBotCoordX(false,i));
                guiLabelsY[i]->setY(dash->objectPos->getBotCoordY(false,i));
                if (dash->ui->check_showIDs->isChecked()) {
                    guiLabelsY[i]->hidden = false;
                }else{
                    guiLabelsY[i]->hidden = true;
                }
            } else {
                guiTeamY[i]->hide();
                guiLabelsY[i]->hide();
            }
        }// yellow team for loop

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
    if (centeredBotID > -1) {
        dash->ui->gView_field->centerOn(guiTeam[centeredBotID]);
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
    simplePoints.push_front(p);
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
        for (unsigned int i=startIter; i<lineAPoints.size(); i++) {
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

    for (Point p : simplePoints)
        point_drawer->points.push_back(p);

    simplePoints.clear();

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
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
            if (dash->robotpanel->botIcons[i]->doubleClicked || guiTeam[i]->doubleClicked)  {
                dash->robotpanel->botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
                centeredBotID = i;
                centerViewOnBot();
                zoomField(20);
                dash->guiPrint("Focused on Robot " + std::to_string(centeredBotID));
                break;
            }
        }//nullcheck
    }//end for
}

void FieldPanel::cameraMoveScan() {
    // Scrolling the camera removes centeredOn but not selection
    if (justScrolled) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
                dash->robotpanel->botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
            }//nullcheck
        }
        centeredBotID = -1;
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
    if (field->highlighted || sidelines->highlighted) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
                guiTeam[i]->highlighted = false;
                guiTeam[i]->setSelected(false);
                dash->robotpanel->botIcons[i]->highlighted = false;
                dash->robotpanel->botIcons[i]->setSelected(false);
                dash->robotpanel->botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
            }//nullcheck
        }
        field->highlighted = false;
        sidelines->highlighted = false;
        selectedBot = -1;
        centeredBotID = -1;

//        newSelection = true;
        return true;
    }
     return true;
}

bool FieldPanel::panelBotClickScan() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
            // Bots on the panel clicked
            if (dash->robotpanel->botIcons[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<dash->teamSize_blue; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    dash->robotpanel->botIcons[j]->highlighted = false;
                    dash->robotpanel->botIcons[j]->setSelected(false);

                }
                dash->robotpanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
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

bool FieldPanel::fieldBotClickScan() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
            // Bots on the field clicked
            if (guiTeam[i]->isSelected()) {
                selectedBot = i;
                dash->robotpanel->scrollToSelBot(i);
                for (int j=0; j<dash->teamSize_blue; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    dash->robotpanel->botIcons[j]->highlighted = false;
                    dash->robotpanel->botIcons[j]->setSelected(false);
                }
                dash->robotpanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
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
    centeredBotID = -1;

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



