#include "robotpanel.h"
#include "guirobot.h"
#include "mainwindow.h"
//#include <QTextCharFormat>
//#include <QTextCursor>

// Global static pointer used to ensure a single instance of the class.
RobotPanel* RobotPanel::robotpanel = NULL;

RobotPanel::RobotPanel() {}

RobotPanel *RobotPanel::getRobotPanel() {
    if (robotpanel == NULL) {
        robotpanel = new RobotPanel();
    }
    return robotpanel;
}

void RobotPanel::setupBotPanel(MainWindow * win) {

    // Bot Frames
    botFrames.push_back(win->ui->frame_robot_0);
    botFrames.push_back(win->ui->frame_robot_1);
    botFrames.push_back(win->ui->frame_robot_2);
    botFrames.push_back(win->ui->frame_robot_3);
    botFrames.push_back(win->ui->frame_robot_4);
    botFrames.push_back(win->ui->frame_robot_5);
    // Title label vector
    botTitle.push_back(win->ui->title_robPanel_0);
    botTitle.push_back(win->ui->title_robPanel_1);
    botTitle.push_back(win->ui->title_robPanel_2);
    botTitle.push_back(win->ui->title_robPanel_3);
    botTitle.push_back(win->ui->title_robPanel_4);
    botTitle.push_back(win->ui->title_robPanel_5);
    // Velocity dials
    velocityDials.push_back(win->ui->dial_botSpeed_0);
    velocityDials.push_back(win->ui->dial_botSpeed_1);
    velocityDials.push_back(win->ui->dial_botSpeed_2);
    velocityDials.push_back(win->ui->dial_botSpeed_3);
    velocityDials.push_back(win->ui->dial_botSpeed_4);
    velocityDials.push_back(win->ui->dial_botSpeed_5);
    // X Coordinate vector
    botXcoords.push_back(win->ui->lcd_coordX_0);
    botXcoords.push_back(win->ui->lcd_coordX_1);
    botXcoords.push_back(win->ui->lcd_coordX_2);
    botXcoords.push_back(win->ui->lcd_coordX_3);
    botXcoords.push_back(win->ui->lcd_coordX_4);
    botXcoords.push_back(win->ui->lcd_coordX_5);
    // Y Coordinate vector
    botYcoords.push_back(win->ui->lcd_coordY_0);
    botYcoords.push_back(win->ui->lcd_coordY_1);
    botYcoords.push_back(win->ui->lcd_coordY_2);
    botYcoords.push_back(win->ui->lcd_coordY_3);
    botYcoords.push_back(win->ui->lcd_coordY_4);
    botYcoords.push_back(win->ui->lcd_coordY_5);
    // Orientation vector
    botOrients.push_back(win->ui->dial_botOrient_0);
    botOrients.push_back(win->ui->dial_botOrient_1);
    botOrients.push_back(win->ui->dial_botOrient_2);
    botOrients.push_back(win->ui->dial_botOrient_3);
    botOrients.push_back(win->ui->dial_botOrient_4);
    botOrients.push_back(win->ui->dial_botOrient_5);
    // Icons vector
    robotIcon0 = new GuiRobot;
    robotIcon1 = new GuiRobot;
    robotIcon2 = new GuiRobot;
    robotIcon3 = new GuiRobot;
    robotIcon4 = new GuiRobot;
    robotIcon5 = new GuiRobot;
    botIcons.push_back(robotIcon0);
    botIcons.push_back(robotIcon1);
    botIcons.push_back(robotIcon2);
    botIcons.push_back(robotIcon3);
    botIcons.push_back(robotIcon4);
    botIcons.push_back(robotIcon5);
    // Icon scenes vector
    scene_botIcon_0 = new QGraphicsScene;
    scene_botIcon_1 = new QGraphicsScene;
    scene_botIcon_2 = new QGraphicsScene;
    scene_botIcon_3 = new QGraphicsScene;
    scene_botIcon_4 = new QGraphicsScene;
    scene_botIcon_5 = new QGraphicsScene;
    botIconScenes.push_back(scene_botIcon_0);
    botIconScenes.push_back(scene_botIcon_1);
    botIconScenes.push_back(scene_botIcon_2);
    botIconScenes.push_back(scene_botIcon_3);
    botIconScenes.push_back(scene_botIcon_4);
    botIconScenes.push_back(scene_botIcon_5);
    // Icon frames vector
    botIconFrames.push_back(win->ui->gView_robot_0);
    botIconFrames.push_back(win->ui->gView_robot_1);
    botIconFrames.push_back(win->ui->gView_robot_2);
    botIconFrames.push_back(win->ui->gView_robot_3);
    botIconFrames.push_back(win->ui->gView_robot_4);
    botIconFrames.push_back(win->ui->gView_robot_5);
    // Selected bot icons
    robotIcon0Sel = new GuiRobot;
    robotIcon1Sel = new GuiRobot;
    robotIcon2Sel = new GuiRobot;
    robotIcon3Sel = new GuiRobot;
    robotIcon4Sel = new GuiRobot;
    robotIcon5Sel = new GuiRobot;
    botIconsSelected.push_back(robotIcon0Sel);
    botIconsSelected.push_back(robotIcon1Sel);
    botIconsSelected.push_back(robotIcon2Sel);
    botIconsSelected.push_back(robotIcon3Sel);
    botIconsSelected.push_back(robotIcon4Sel);
    botIconsSelected.push_back(robotIcon5Sel);
    scene_botIconSel_0 = new QGraphicsScene;
    scene_botIconSel_1 = new QGraphicsScene;
    scene_botIconSel_2 = new QGraphicsScene;
    scene_botIconSel_3 = new QGraphicsScene;
    scene_botIconSel_4 = new QGraphicsScene;
    scene_botIconSel_5 = new QGraphicsScene;
    botIconSelScenes.push_back(scene_botIconSel_0);
    botIconSelScenes.push_back(scene_botIconSel_1);
    botIconSelScenes.push_back(scene_botIconSel_2);
    botIconSelScenes.push_back(scene_botIconSel_3);
    botIconSelScenes.push_back(scene_botIconSel_4);
    botIconSelScenes.push_back(scene_botIconSel_5);
    // Ball GUI icon
    ballIcon = new GuiBall();
    scene_ballIcon = new QGraphicsScene;

    // relevant vectors filled & connected; initial details added to GUI
    for (int i=0; i<win->teamSize; i++) {
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
        velocityDials[i]->setValue(0);
        win->overriddenBots.push_back(false);    // creating each element, and setting to false
    }
    // putting ball icon into GUI
    scene_ballIcon->addItem(ballIcon);
    win->ui->gView_ball->setScene(scene_ballIcon);
    win->ui->gView_ball->scale(.2, .2);
    win->ui->gView_ball->scale(1, -1);
    win->ui->gView_ball->hide();

    // Formatting selected bot panel
    win->ui->gView_robot_prime->scale(.4, .4);
    win->ui->gView_robot_prime->scale(1,-1);
    win->ui->gView_robot_prime->rotate(90);

}//setupBotPanel

void RobotPanel::updateBotPanel(MainWindow * win) {
    // Printing current bot info to Robot Panels
    for (int i=0; i<win->teamSize; i++) {
        botTitle[i]->setText("Robot " + QString::number(i));
        // Nullcheck
        if (win->gamemodel->find(i,win->gamemodel->getMyTeam()) != NULL) {
            botXcoords[i]->display(win->getBotCoordX(true, i));
            botYcoords[i]->display(win->getBotCoordY(true, i));
            botOrients[i]->setValue(win->getBotOrientDouble(true, i));
            botIcons[i]->setX(0);
            botIcons[i]->setY(0);
            botIcons[i]->setZValue(2);
            if (botIconFrames[i]->isVisible() == false) {
                botIconFrames[i]->show();
            }
            velocityDials[i]->setValue(win->getVelocity(i));
//            printBehavior(i, to_string(getVelocity(i)), true);
//            cout << to_string(getVelocity(0)) << "\n";
            // dynamic velocity dial colors
            if (velocityDials[i]->value() > 0) {            // forward
                velocityDials[i]->setStyleSheet("background-color: rgb(0, 200, 0);");

            } else if (velocityDials[i]->value() < 0) {     // reverse
                velocityDials[i]->setStyleSheet("background-color: rgb(200, 0, 0);");

            } else {                                        // motionless
                velocityDials[i]->setStyleSheet("background-color: rgb(150, 150, 150);");
            }

            botIconFrames[i]->update();
        }//nullcheck
    }
    // Selected Bot Panel velocity dial
    if (win->ui->dial_botSpeed->value() > 0) {
        win->ui->dial_botSpeed->setStyleSheet("background-color: rgb(0, 200, 0);");
        win->ui->lcd_botSpeed->setStyleSheet("background-color: rgb(0, 100, 0);");
    } else if (win->ui->dial_botSpeed->value() < 0 ) {
        win->ui->dial_botSpeed->setStyleSheet("background-color: rgb(200, 0, 0);");
        win->ui->lcd_botSpeed->setStyleSheet("background-color: rgb(100, 0, 0);");
    } else {
        win->ui->dial_botSpeed->setStyleSheet("background-color: rgb(150, 150, 150);");
        win->ui->lcd_botSpeed->setStyleSheet("background-color: rgb(100, 100, 100);");
    }
    // Robot override checkbox
    if (win->selectedBot > -1) {
        win->ui->check_botOverride->setEnabled(true);
        win->ui->check_botOverride->show();
        if (win->overriddenBots[win->selectedBot] == true) {
            win->ui->check_botOverride->setChecked(true);
        } else {
            win->ui->check_botOverride->setChecked(false);
        }
    } else {
        win->ui->check_botOverride->setEnabled(false);
        win->ui->check_botOverride->hide();
    }


    // Mouse point
    win->ui->lcd_coordX_cursor->display(win->getMouseCoordX());
    win->ui->lcd_coordY_cursor->display(win->getMouseCoordY());

}

