#include "fieldpanel.h"
#include "objectposition.h"
#include "robotpanel.h"
#include "selrobotpanel.h"

FieldPanel::FieldPanel(MainWindow * mw) {
    dash = mw;
}

void FieldPanel::setUpScene() {

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
    botLabel0 = new GuiBotLabel;
    botLabel1 = new GuiBotLabel;
    botLabel2 = new GuiBotLabel;
    botLabel3 = new GuiBotLabel;
    botLabel4 = new GuiBotLabel;
    botLabel5 = new GuiBotLabel;
    guiLabels.push_back(botLabel0);
    guiLabels.push_back(botLabel1);
    guiLabels.push_back(botLabel2);
    guiLabels.push_back(botLabel3);
    guiLabels.push_back(botLabel4);
    guiLabels.push_back(botLabel5);

    // Bot Labels - yellow team
    botLabel0Y = new GuiBotLabel;
    botLabel1Y = new GuiBotLabel;
    botLabel2Y = new GuiBotLabel;
    botLabel3Y = new GuiBotLabel;
    botLabel4Y = new GuiBotLabel;
    botLabel5Y = new GuiBotLabel;
    guiLabelsY.push_back(botLabel0Y);
    guiLabelsY.push_back(botLabel1Y);
    guiLabelsY.push_back(botLabel2Y);
    guiLabelsY.push_back(botLabel3Y);
    guiLabelsY.push_back(botLabel4Y);
    guiLabelsY.push_back(botLabel5Y);

    // Blue Team Robots
    robot0 = new GuiRobot();
    robot1 = new GuiRobot();
    robot2 = new GuiRobot();
    robot3 = new GuiRobot();
    robot4 = new GuiRobot();
    robot5 = new GuiRobot();
    guiTeam.push_back(robot0);
    guiTeam.push_back(robot1);
    guiTeam.push_back(robot2);
    guiTeam.push_back(robot3);
    guiTeam.push_back(robot4);
    guiTeam.push_back(robot5);

    // Yellow Team
    robot0Y = new GuiRobot();
    robot1Y = new GuiRobot();
    robot2Y = new GuiRobot();
    robot3Y = new GuiRobot();
    robot4Y = new GuiRobot();
    robot5Y = new GuiRobot();
    guiTeamY.push_back(robot0Y);
    guiTeamY.push_back(robot1Y);
    guiTeamY.push_back(robot2Y);
    guiTeamY.push_back(robot3Y);
    guiTeamY.push_back(robot4Y);
    guiTeamY.push_back(robot5Y);

    // Adding the previous gui items to the scene
    scene->addItem(field);
    scene->addItem(botLabel0);
    scene->addItem(botLabel1);
    scene->addItem(botLabel2);
    scene->addItem(botLabel3);
    scene->addItem(botLabel4);
    scene->addItem(botLabel5);
    scene->addItem(botLabel0Y);
    scene->addItem(botLabel1Y);
    scene->addItem(botLabel2Y);
    scene->addItem(botLabel3Y);
    scene->addItem(botLabel4Y);
    scene->addItem(botLabel5Y);
    scene->addItem(robot0);
    scene->addItem(robot1);
    scene->addItem(robot2);
    scene->addItem(robot3);
    scene->addItem(robot4);
    scene->addItem(robot5);
    scene->addItem(robot0Y);
    scene->addItem(robot1Y);
    scene->addItem(robot2Y);
    scene->addItem(robot3Y);
    scene->addItem(robot4Y);
    scene->addItem(robot5Y);
    scene->addItem(sidelines);
    scene->addItem(ball);


    // Setting static details for robots
    for (int i=0; i<dash->teamSize; i++) {
        // Blue team
        guiTeam[i]->id = i;
        guiTeam[i]->setToolTip("Robot " + QString::number(i));
        guiTeam[i]->myTeam = true;
        guiLabels[i]->id = i;
        guiLabels[i]->myTeam = true;
        guiLabels[i]->setScale(2.5);

        // Yellow team
        guiTeamY[i]->id = i;
        guiTeamY[i]->setToolTip("Robot " + QString::number(i));
        guiTeamY[i]->myTeam = false;
        guiLabelsY[i]->id = i;
        guiLabelsY[i]->myTeam = false;
        guiLabelsY[i]->setScale(2.5);
    }//end for

    // Drawing debug line (optional)
//    guidrawline = new GuiDrawLine();
//    guidrawline->setZValue(4);
//    guidrawline->setX(100);
//    guidrawline->setY(100);
//    scene->addItem(guidrawline);

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
        for (int i=0; i<dash->teamSize; i++) {
            // Blue Team
            if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
                guiTeam[i]->setX(dash->objectPos->getBotCoordX(true, i));
                guiTeam[i]->setY(dash->objectPos->getBotCoordY(true, i));
                guiTeam[i]->setZValue(3);
                double angle = dash->objectPos->getBotOrientDouble(true, i) ;
                guiTeam[i]->setRotation(angle);
                // Action colors (may be better in the button slots)
                if (i != selectedBot) {
                    if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam())->getDrible() ) {
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
                // drawLine TEST
//                drawLine(dash->getBotCoordX(true, 0),dash->getBotCoordY(true, 0), Movement::Move.pathQueue[i].x, Movement::Move.pathQueue[i].y );
//                dash->ui->gView_field->update();

            }
            // Yellow Team
            if (dash->gamemodel->find(i, dash->gamemodel->getOponentTeam()) != NULL) {
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
            }
        }

    // Keeping camera centered
    dash->centerViewOnBot();
    dash->ui->gView_field->update();

}

void FieldPanel::scanForSelection() {
    bool newSelection = true;
    // Scanning for double-click selection
    for (int i=0; i<dash->teamSize; i++) {
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
            if (dash->robotpanel->botIcons[i]->doubleClicked || guiTeam[i]->doubleClicked) {
                dash->robotpanel->botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
                centeredBotID = i;
                dash->centerViewOnBot();
                zoomField(20);
                dash->guiPrint("Focused on Robot " + to_string(centeredBotID));
                break;
            }
        }//nullcheck
    }//end for

    // Scrolling the camera removes centeredOn but not selection
    if (justScrolled) {
        for (int i=0; i<dash->teamSize; i++) {
            if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
                dash->robotpanel->botIcons[i]->doubleClicked = false;
                guiTeam[i]->doubleClicked = false;
            }//nullcheck
        }
        centeredBotID = -1;
    }
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
        for (int i=0; i<dash->teamSize; i++) {
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
        newSelection = true;
    }

    for (int i=0; i<dash->teamSize; i++) {
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
//            // tab TEST
//            if (robotpanel->botFrames[i]->hasFocus()) {
//                robotpanel->botIcons[i]->setSelected(true);
//            }

        // Bots on the panel clicked
            if (dash->robotpanel->botIcons[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<dash->teamSize; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    dash->robotpanel->botIcons[j]->highlighted = false;
                    dash->robotpanel->botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;

                }
//                field->highlighted = false;
                dash->robotpanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<dash->teamSize; r++) {
                    dash->robotpanel->botIconFrames[i]->update();
                }
                dash->ui->gView_robot_prime->hide();
                dash->ui->gView_robot_prime->show();
                newSelection = true;
            }
            // Bots on the field clicked
            if (guiTeam[i]->isSelected()) {
                selectedBot = i;
                for (int j=0; j<dash->teamSize; j++) {
                    guiTeam[j]->highlighted = false;
                    guiTeam[j]->setSelected(false);
                    dash->robotpanel->botIcons[j]->highlighted = false;
                    dash->robotpanel->botIcons[j]->setSelected(false);
//                    botIcons[i]->doubleClicked = false;
//                    guiTeam[i]->doubleClicked = false;
                }
//                field->highlighted = false;
                dash->robotpanel->botIcons[i]->highlighted = true;
                guiTeam[i]->highlighted = true;
                refresh = true;
                // Refresh GUI
                for (int r=0; r<dash->teamSize; r++) {
                    dash->robotpanel->botIconFrames[i]->update();
                }
                dash->ui->gView_robot_prime->hide();
                dash->ui->gView_robot_prime->show();
                newSelection = true;
            }
        }//null check
    }//for loop
    if (newSelection) {
        dash->selrobotpanel->updateSelectedBotPanel(selectedBot);
    } else { return; }

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



