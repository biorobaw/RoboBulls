#include <QGraphicsView>
#include <QLCDNumber>
#include <QDial>
#include <QLabel>

#include "robotpanel.h"
#include "mainwindow.h"
#include "objectposition.h"
#include "gamepanel.h"
#include "fieldpanel.h"
#include "selrobotpanel.h"
#include "ui_mainwindow.h"
#include "model/gamemodel.h"
#include "utilities/getclassname.h"
#include "model/robot.h"

//Helper classes
#include "guirobot.h"
#include "guiball.h"


RobotPanel::RobotPanel(MainWindow *mw) {
    dash = mw;
}

void RobotPanel::setupBotPanel() {

    // Bot Frames
    botFrames.push_back(dash->ui->frame_robot_0);
    botFrames.push_back(dash->ui->frame_robot_1);
    botFrames.push_back(dash->ui->frame_robot_2);
    botFrames.push_back(dash->ui->frame_robot_3);
    botFrames.push_back(dash->ui->frame_robot_4);
    botFrames.push_back(dash->ui->frame_robot_5);
    botFrames.push_back(dash->ui->frame_robot_6);
    botFrames.push_back(dash->ui->frame_robot_7);
    botFrames.push_back(dash->ui->frame_robot_8);
    botFrames.push_back(dash->ui->frame_robot_9);

    // Title label vector
    botTitle.push_back(dash->ui->title_robPanel_0);
    botTitle.push_back(dash->ui->title_robPanel_1);
    botTitle.push_back(dash->ui->title_robPanel_2);
    botTitle.push_back(dash->ui->title_robPanel_3);
    botTitle.push_back(dash->ui->title_robPanel_4);
    botTitle.push_back(dash->ui->title_robPanel_5);
    botTitle.push_back(dash->ui->title_robPanel_6);
    botTitle.push_back(dash->ui->title_robPanel_7);
    botTitle.push_back(dash->ui->title_robPanel_8);
    botTitle.push_back(dash->ui->title_robPanel_9);

    // Velocity dials
    velocityDials.push_back(dash->ui->dial_botVel_0);
    velocityDials.push_back(dash->ui->dial_botVel_1);
    velocityDials.push_back(dash->ui->dial_botVel_2);
    velocityDials.push_back(dash->ui->dial_botVel_3);
    velocityDials.push_back(dash->ui->dial_botVel_4);
    velocityDials.push_back(dash->ui->dial_botVel_5);
//    velocityDials.push_back(dash->ui->dial_botVel_6);
//    velocityDials.push_back(dash->ui->dial_botVel_7);
//    velocityDials.push_back(dash->ui->dial_botVel_8);
//    velocityDials.push_back(dash->ui->dial_botVel_9);

    //Current Behavior Labels
    currBehavior.push_back(dash->ui->currBehavior_0);
    currBehavior.push_back(dash->ui->currBehavior_1);
    currBehavior.push_back(dash->ui->currBehavior_2);
    currBehavior.push_back(dash->ui->currBehavior_3);
    currBehavior.push_back(dash->ui->currBehavior_4);
    currBehavior.push_back(dash->ui->currBehavior_5);



 /*   // Speed dials disabled so that I can see GUI changes
    speedDials.push_back(dash->ui->dial_botSpeed_0);
    speedDials.push_back(dash->ui->dial_botSpeed_1);
    speedDials.push_back(dash->ui->dial_botSpeed_2);
    speedDials.push_back(dash->ui->dial_botSpeed_3);
    speedDials.push_back(dash->ui->dial_botSpeed_4);
    speedDials.push_back(dash->ui->dial_botSpeed_5);
    speedDials.push_back(dash->ui->dial_botSpeed_6);
    speedDials.push_back(dash->ui->dial_botSpeed_7);
    speedDials.push_back(dash->ui->dial_botSpeed_8);
    speedDials.push_back(dash->ui->dial_botSpeed_9);
*/
    // X Coordinate vector
    botXcoords.push_back(dash->ui->lcd_coordX_0);
    botXcoords.push_back(dash->ui->lcd_coordX_1);
    botXcoords.push_back(dash->ui->lcd_coordX_2);
    botXcoords.push_back(dash->ui->lcd_coordX_3);
    botXcoords.push_back(dash->ui->lcd_coordX_4);
    botXcoords.push_back(dash->ui->lcd_coordX_5);
//    botXcoords.push_back(dash->ui->lcd_coordX_6);
//    botXcoords.push_back(dash->ui->lcd_coordX_7);
//    botXcoords.push_back(dash->ui->lcd_coordX_8);
//    botXcoords.push_back(dash->ui->lcd_coordX_9);

    // Y Coordinate vector
    botYcoords.push_back(dash->ui->lcd_coordY_0);
    botYcoords.push_back(dash->ui->lcd_coordY_1);
    botYcoords.push_back(dash->ui->lcd_coordY_2);
    botYcoords.push_back(dash->ui->lcd_coordY_3);
    botYcoords.push_back(dash->ui->lcd_coordY_4);
    botYcoords.push_back(dash->ui->lcd_coordY_5);
//    botYcoords.push_back(dash->ui->lcd_coordY_6);
//    botYcoords.push_back(dash->ui->lcd_coordY_7);
//    botYcoords.push_back(dash->ui->lcd_coordY_8);
//    botYcoords.push_back(dash->ui->lcd_coordY_9);

    // Orientation vector
    botOrients.push_back(dash->ui->dial_botOrient_0);
    botOrients.push_back(dash->ui->dial_botOrient_1);
    botOrients.push_back(dash->ui->dial_botOrient_2);
    botOrients.push_back(dash->ui->dial_botOrient_3);
    botOrients.push_back(dash->ui->dial_botOrient_4);
    botOrients.push_back(dash->ui->dial_botOrient_5);
//    botOrients.push_back(dash->ui->dial_botOrient_6);
//    botOrients.push_back(dash->ui->dial_botOrient_7);
//    botOrients.push_back(dash->ui->dial_botOrient_8);
//    botOrients.push_back(dash->ui->dial_botOrient_9);

    // Icons vector
    robotIcon0 = new GuiRobot;
    robotIcon1 = new GuiRobot;
    robotIcon2 = new GuiRobot;
    robotIcon3 = new GuiRobot;
    robotIcon4 = new GuiRobot;
    robotIcon5 = new GuiRobot;
//    robotIcon6 = new GuiRobot;
//    robotIcon7 = new GuiRobot;
//    robotIcon8 = new GuiRobot;
//    robotIcon9 = new GuiRobot;
    botIcons.push_back(robotIcon0);
    botIcons.push_back(robotIcon1);
    botIcons.push_back(robotIcon2);
    botIcons.push_back(robotIcon3);
    botIcons.push_back(robotIcon4);
    botIcons.push_back(robotIcon5);
//    botIcons.push_back(robotIcon6);
//    botIcons.push_back(robotIcon7);
//    botIcons.push_back(robotIcon8);
//    botIcons.push_back(robotIcon9);
    // Icon scenes vector
    scene_botIcon_0 = new QGraphicsScene;
    scene_botIcon_1 = new QGraphicsScene;
    scene_botIcon_2 = new QGraphicsScene;
    scene_botIcon_3 = new QGraphicsScene;
    scene_botIcon_4 = new QGraphicsScene;
    scene_botIcon_5 = new QGraphicsScene;
//    scene_botIcon_6 = new QGraphicsScene;
//    scene_botIcon_7 = new QGraphicsScene;
//    scene_botIcon_8 = new QGraphicsScene;
//    scene_botIcon_9 = new QGraphicsScene;
    botIconScenes.push_back(scene_botIcon_0);
    botIconScenes.push_back(scene_botIcon_1);
    botIconScenes.push_back(scene_botIcon_2);
    botIconScenes.push_back(scene_botIcon_3);
    botIconScenes.push_back(scene_botIcon_4);
    botIconScenes.push_back(scene_botIcon_5);
//    botIconScenes.push_back(scene_botIcon_6);
//    botIconScenes.push_back(scene_botIcon_7);
//    botIconScenes.push_back(scene_botIcon_8);
//    botIconScenes.push_back(scene_botIcon_9);

    // Icon frames vector
    botIconFrames.push_back(dash->ui->gView_robot_0);
    botIconFrames.push_back(dash->ui->gView_robot_1);
    botIconFrames.push_back(dash->ui->gView_robot_2);
    botIconFrames.push_back(dash->ui->gView_robot_3);
    botIconFrames.push_back(dash->ui->gView_robot_4);
    botIconFrames.push_back(dash->ui->gView_robot_5);
//    botIconFrames.push_back(dash->ui->gView_robot_6);
//    botIconFrames.push_back(dash->ui->gView_robot_7);
//    botIconFrames.push_back(dash->ui->gView_robot_8);
//    botIconFrames.push_back(dash->ui->gView_robot_9);

    // Selected bot icons
    robotIcon0Sel = new GuiRobot;
    robotIcon1Sel = new GuiRobot;
    robotIcon2Sel = new GuiRobot;
    robotIcon3Sel = new GuiRobot;
    robotIcon4Sel = new GuiRobot;
    robotIcon5Sel = new GuiRobot;
//    robotIcon6Sel = new GuiRobot;
//    robotIcon7Sel = new GuiRobot;
//    robotIcon8Sel = new GuiRobot;
//    robotIcon9Sel = new GuiRobot;

    botIconsSelected.push_back(robotIcon0Sel);
    botIconsSelected.push_back(robotIcon1Sel);
    botIconsSelected.push_back(robotIcon2Sel);
    botIconsSelected.push_back(robotIcon3Sel);
    botIconsSelected.push_back(robotIcon4Sel);
    botIconsSelected.push_back(robotIcon5Sel);
//    botIconsSelected.push_back(robotIcon6Sel);
//    botIconsSelected.push_back(robotIcon7Sel);
//    botIconsSelected.push_back(robotIcon8Sel);
//    botIconsSelected.push_back(robotIcon9Sel);

    scene_botIconSel_0 = new QGraphicsScene;
    scene_botIconSel_1 = new QGraphicsScene;
    scene_botIconSel_2 = new QGraphicsScene;
    scene_botIconSel_3 = new QGraphicsScene;
    scene_botIconSel_4 = new QGraphicsScene;
    scene_botIconSel_5 = new QGraphicsScene;
//    scene_botIconSel_6 = new QGraphicsScene;
//    scene_botIconSel_7 = new QGraphicsScene;
//    scene_botIconSel_8 = new QGraphicsScene;
//    scene_botIconSel_9 = new QGraphicsScene;

    botIconSelScenes.push_back(scene_botIconSel_0);
    botIconSelScenes.push_back(scene_botIconSel_1);
    botIconSelScenes.push_back(scene_botIconSel_2);
    botIconSelScenes.push_back(scene_botIconSel_3);
    botIconSelScenes.push_back(scene_botIconSel_4);
    botIconSelScenes.push_back(scene_botIconSel_5);
//    botIconSelScenes.push_back(scene_botIconSel_6);
//    botIconSelScenes.push_back(scene_botIconSel_7);
//    botIconSelScenes.push_back(scene_botIconSel_8);
//    botIconSelScenes.push_back(scene_botIconSel_9);

    // Ball GUI icon
    ballIcon = new GuiBall();
    scene_ballIcon = new QGraphicsScene;

    // relevant vectors filled & connected; initial details added to GUI
    for (int i=0; i<dash->teamSize_blue; i++) {
        botIcons[i]->id = i;
        botIcons[i]->icon = true;
        botIconsSelected[i]->id = i;
        botIconsSelected[i]->icon = true;
        botIconSelScenes[i]->addItem(botIconsSelected[i]);
        botIconFrames[i]->scale(.2, .2);
        botIconFrames[i]->scale(1,-1);
        botIconFrames[i]->rotate(90);
        botIconScenes[i]->addItem(botIcons[i]);
        botIconFrames[i]->setScene(botIconScenes[i]);
        botIconFrames[i]->hide();
        botIconFrames[i]->setToolTip("Robot " + QString::number(i));
        velocityDials[i]->setValue(0);
        dash->overriddenBots.push_back(false);    // creating each element, and setting to false
    }
    // putting ball icon into GUI
    scene_ballIcon->addItem(ballIcon);
    dash->ui->gView_ball->setScene(scene_ballIcon);
    dash->ui->gView_ball->scale(.2, .2);
    dash->ui->gView_ball->scale(1, -1);
    dash->ui->gView_ball->hide();

    // Formatting selected bot panel
    dash->ui->gView_robot_prime->scale(.4, .4);
    dash->ui->gView_robot_prime->scale(1,-1);
    dash->ui->gView_robot_prime->rotate(90);

    // Formatting robots panel
//    dash->ui->layout_robots->setContentsMargins(QMargins(0,0,0,0));

}//setupBotPanel



void RobotPanel::updateBotPanel() {

    // Printing current bot info to Robot Panels
    for (int i=0; i<dash->teamSize_blue; i++) {
        botTitle[i]->setText("Robot " + QString::number(i));
        // Nullcheck
        if (dash->gamemodel->find(i,dash->gamemodel->getMyTeam()) != NULL) {
            botFrames[i]->setEnabled(true);
            botXcoords[i]->display(dash->objectPos->getBotCoordX(true, i));
            botYcoords[i]->display(dash->objectPos->getBotCoordY(true, i));
            botOrients[i]->setValue(dash->objectPos->getBotOrientDouble(true, i) +90 );
            botIcons[i]->setX(0);
            botIcons[i]->setY(0);
            botIcons[i]->setZValue(2);
            if (botIconFrames[i]->isVisible() == false) {
                botIconFrames[i]->show();
            }
            // Velocity dials
            velocityDials[i]->setValue(dash->objectPos->getVelocity(i));
            // dynamic velocity dial colors
            if (velocityDials[i]->value() > 0) {            // forward
                velocityDials[i]->setStyleSheet("background-color: rgb(0, 200, 0);");

            } else if (velocityDials[i]->value() < 0) {     // reverse
                velocityDials[i]->setStyleSheet("background-color: rgb(200, 0, 0);");

            } else {                                        // motionless
                velocityDials[i]->setStyleSheet("background-color: rgb(150, 150, 150);");
            }

            //Setting the Current behavior labels for the robots
            //Find a way to fix the template error!!*********
            Robot* robot = gameModel->findMyTeam(i);
            currBehavior[i]->setText(QString::fromStdString(SelRobotPanel::getBehaviorName(robot)));


            // Speed dials disabled so that I can see how GUI is updated
/*          int maxSpeed = speedDials[i]->maximum();
            speedDials[i]->setValue(dash->objectPos->botSpeeds[i] * dash->objectPos->speedModifier);
            if (speedDials[i]->value() > dash->objectPos->movementMin) {
                speedDials[i]->setStyleSheet("background-color: rgb(0, 191, 255);");
                if (speedDials[i]->value() > maxSpeed*.2) {
                speedDials[i]->setStyleSheet("background-color: rgb(0, 0, 255);");
                }
                if (speedDials[i]->value() > maxSpeed*.4) {
                speedDials[i]->setStyleSheet("background-color: rgb(160, 32, 240);");
                }
                if (speedDials[i]->value() > maxSpeed*.6) {
                speedDials[i]->setStyleSheet("background-color: rgb(208, 32, 144);");
                }
                if (speedDials[i]->value() > maxSpeed*.8) {
                speedDials[i]->setStyleSheet("background-color: rgb(255, 20, 147);");
                }
            } else {
                speedDials[i]->setStyleSheet("background-color: rgb(150, 150, 150);");
            }
*/
            botIconFrames[i]->update();

        } else {
            botFrames[i]->setEnabled(false);
            botIconFrames[i]->setVisible(false);
        }
    }

    //Populating the gameState label with the current refbox command
    dash->ui->gState->setText(QString::fromStdString(getGameState()));

    //Populating the ballVel label with current ball velocity
    //Fix so that velocity reading are rounded to 2nd or 3rd decimal
    dash->ui->ballVel->setText("Vell: " + QString::fromStdString(gameModel->getBallVelocity().toString()) );

    //Populating the ballAccel label with curent ball acceleration
    dash->ui->ballAccel->setText("Accel: " + QString::fromStdString(gameModel->getBallAcceleration().toString()));

    //Populating the ballMove label with the current movement status of the ball
    //Fix this inquire about why getBallIsStopped is always returning false
    if(gamemodel->getBallIsStopped())
        dash->ui->ballMove->setText("Ball is not moving");
    else
        dash->ui->ballMove->setText("Ball is moving");

    //Populating the currStrategy label with the current strategy
    dash->ui->currStrategy->setText(QString::fromStdString(getCurrStrategy()));

    // Mouse point
    dash->ui->lcd_coordX_cursor->display(dash->objectPos->getMouseCoordX());
    dash->ui->lcd_coordY_cursor->display(dash->objectPos->getMouseCoordY());


}

void RobotPanel::scrollToSelBot(int id) {
    dash->ui->scroll_robots->ensureVisible(0,91*id,50,50);
}

void RobotPanel::updateTeamColors() {
    if  (dash->myTeam == "Yellow") {
        // button color
        dash->ui->btn_toggleTeamColor->setStyleSheet("background-color: yellow;" "color: black");
        // robot panel colors
        dash->ui->frame_robotsPanel->setStyleSheet("background-color: rgb(250, 250, 220);");
        dash->ui->text_primeBot->setStyleSheet("background-color: rgb(100, 100, 0);");
        dash->ui->dial_botOrient_prime->setStyleSheet("background-color: rgb(255, 255, 0);");
        dash->ui->lcd_orient_prime->setStyleSheet("background-color: rgb(0, 0, 150);");
        dash->ui->lcd_coordX_prime->setStyleSheet("background-color: rgb(100, 100, 0);");
        dash->ui->lcd_coordY_prime->setStyleSheet("background-color: rgb(100, 100, 0);");
        for (int i=0; i<dash->teamSize_blue; i++) {
//            botOrients[i]->setStyleSheet("background-color: rgb(0, 0, 150);");
//            botXcoords[i]->setStyleSheet("background-color: rgb(100, 100, 0);");
//            botYcoords[i]->setStyleSheet("background-color: rgb(100, 100, 0);");
        }
    } else if (dash->myTeam == "Blue"){
        // button color
        dash->ui->btn_toggleTeamColor->setStyleSheet("background-color: blue;" "color: white");
        // robot panel colors
        dash->ui->frame_robotsPanel->setStyleSheet("background-color: rgb(225, 225, 255);");
        dash->ui->text_primeBot->setStyleSheet("background-color: rgb(0, 0, 100);");
        dash->ui->dial_botOrient_prime->setStyleSheet("background-color: rgb(255, 255, 0);");
        dash->ui->lcd_orient_prime->setStyleSheet("background-color: rgb(100, 100, 0);");
        dash->ui->lcd_coordX_prime->setStyleSheet("background-color: rgb(0, 0, 100);");
        dash->ui->lcd_coordY_prime->setStyleSheet("background-color: rgb(0, 0, 100);");
        for (int i=0; i<dash->teamSize_blue; i++) {
            botOrients[i]->setStyleSheet("background-color: rgb(255, 255, 0);");
            botXcoords[i]->setStyleSheet("background-color: rgb(0, 0, 150);");
            botYcoords[i]->setStyleSheet("background-color: rgb(0, 0, 150);");

        }

    }
    // goal colors
//    fieldpanel->field->myTeam = myTeam;

    // bot icon colors
    if  (dash->myTeam == "Yellow") {

    }
    for (unsigned int i=0; i<dash->fieldpanel->guiTeam.size(); i++) {
//        dash->fieldpanel->guiTeam[i]->myTeam = dash->myTeam;
//        dash->fieldpanel->guiLabels[i]->myTeam = dash->myTeam;
//        dash->fieldpanel->guiTeamY[i]->myTeam = dash->myTeam;
//        dash->fieldpanel->guiLabelsY[i]->myTeam = dash->myTeam;
        botIcons[i]->myTeam = dash->myTeam;
        botIconsSelected[i]->myTeam = dash->myTeam;
    }
    // rerendering affected objects that aren't regularly updated
    dash->ui->gView_field->scene()->update();
    if (dash->fieldpanel->selectedBot > -1) {
        dash->ui->gView_robot_prime->scene()->update();
    }
}

void RobotPanel::toggleIconVisible() {
    int id = dash->fieldpanel->selectedBot;
    GuiRobot *thisBot = dash->fieldpanel->guiTeam[id];
    GuiRobot *thisIcon = botIcons[id];

    if (dash->fieldpanel->selectedBot > -1) {
        if (thisBot->enabled) {
            thisBot->enabled = false;
            thisIcon->enabled = false;
            thisIcon->setOpacity(.3);
            thisBot->setOpacity(.3);
        } else {
            thisBot->enabled = true;
            thisIcon->enabled = true;
            thisIcon->setOpacity(1);
            thisBot->setOpacity(1);
        }
    }
}

const std::string RobotPanel::getCurrStrategy()
{
    char gameState = gameModel->getGameState();

    switch(gameState)
    {
    case 'S':    //stop game
    case 'G':    //Blue Goal
    case 'g':    //Yellow Goal
        return "Stop Strategy";
        break;
    case 'p':   //Yellow Penalty Kick
    case 'P':   //Blue Penalty Kick
        return "Penalty Strategy";
        break;
    case 'k':   //Yellow Kickoff
    case 'K':   //Blue Kickoff
        return "Kick Off Strategy";
        break;
    case 'f':   //Yellow Free Kick
    case 'F':   //Blue Free Kick
        return "Free Kick Strategy";
        break;
    case 'i':   //Yellow Indirect Kick
    case 'I':   //Blue Indirect kick
        return "Indirect Kick Strategy";
        break;
    case 'T':
    case 't':
    case 'H':    //Halt
        return "Halt Strategy";
        break;
    case ' ':    //Normal game play
        return "Normal Game Strategy";
        break;
    case 's':    //Force Start
        return "Normal Game Strategy";
        break;
    default:    //Anything Else
        return "GS Not Implemented";
        break;
    };



}

const std::string RobotPanel::getGameState()
{
    char gameState = gameModel->getGameState();

    switch(gameState)
    {
    case 'S':    //stop game
        return "Stop Game";
        break;
    case 'G':    //Blue Goal
        return "Blue Goal";
        break;
    case 'g':    //Yellow Goal
        return "Yellow Goal";
        break;
    case 'p':   //Yellow Penalty Kick
        return "Yellow Penalty Kick";
        break;
    case 'P':   //Blue Penalty Kick
        return "Blue Penalty Kick";
        break;
    case 'k':   //Yellow Kickoff
        return "Yellow Kickoff";
        break;
    case 'K':   //Blue Kickoff
        return "Blue Kickoff";
        break;
    case 'f':   //Yellow Free Kick
        return "Yellow Free Kick";
        break;
    case 'F':   //Blue Free Kick
        return "Blue Free Kick";
        break;
    case 'i':   //Yellow Indirect Kick
        return "Yellow Indirect Kick";
        break;
    case 'I':   //Blue Indirect kick
        return "Blue Indirect Kick";
        break;
    case 'T':
    case 't':
    case 'H':    //Halt
        return "Halt";
        break;
    case ' ':    //Normal game play
        return "Normal game play";
        break;
    case 's':    //Force Start
        return "Force Start";
        break;
    default:    //Anything Else
        return "Defaul GameState";
        break;
    };
}

