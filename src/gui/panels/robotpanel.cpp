#include <QGraphicsView>
#include <QLCDNumber>
#include <QDial>
#include <QLabel>
#include <QCursor>

#include "robotpanel.h"
#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "model/game_state.h"
#include "robot/robot.h"
#include "utilities/measurements.h"
//Helper classes
#include "gui/utils/gui_robot_drawer.h"
#include "gui/data/guiball.h"
#include "model/ball.h"
#include "panel_teams/frame_robot.h"


RobotPanel::RobotPanel(MainWindow *mw) {
    dash = mw;
}

void RobotPanel::setupBotPanel() {

//    for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++)
        dash->layout_robots->insertWidget(0,new FrameRobot(dash));

    // Bot Frames
    botFrames.push_back(dash->frame_robot_0);
    botFrames.push_back(dash->frame_robot_1);
    botFrames.push_back(dash->frame_robot_2);
    botFrames.push_back(dash->frame_robot_3);
    botFrames.push_back(dash->frame_robot_4);
    botFrames.push_back(dash->frame_robot_5);
    botFrames.push_back(dash->frame_robot_6);
    botFrames.push_back(dash->frame_robot_7);
    botFrames.push_back(dash->frame_robot_8);

    // Title label vector
    botTitle.push_back(dash->title_robPanel_0);
    botTitle.push_back(dash->title_robPanel_1);
    botTitle.push_back(dash->title_robPanel_2);
    botTitle.push_back(dash->title_robPanel_3);
    botTitle.push_back(dash->title_robPanel_4);
    botTitle.push_back(dash->title_robPanel_5);
    botTitle.push_back(dash->title_robPanel_6);
    botTitle.push_back(dash->title_robPanel_7);
    botTitle.push_back(dash->title_robPanel_8);

    // Velocity dials
    velocityDials.push_back(dash->dial_botVel_0);
    velocityDials.push_back(dash->dial_botVel_1);
    velocityDials.push_back(dash->dial_botVel_2);
    velocityDials.push_back(dash->dial_botVel_3);
    velocityDials.push_back(dash->dial_botVel_4);
    velocityDials.push_back(dash->dial_botVel_5);
//    velocityDials.push_back(dash->dial_botVel_6);
//    velocityDials.push_back(dash->dial_botVel_7);
//    velocityDials.push_back(dash->dial_botVel_8);
//    velocityDials.push_back(dash->dial_botVel_9);

    //Current Behavior Labels
    currBehavior.push_back(dash->currBehavior_0);
    currBehavior.push_back(dash->currBehavior_1);
    currBehavior.push_back(dash->currBehavior_2);
    currBehavior.push_back(dash->currBehavior_3);
    currBehavior.push_back(dash->currBehavior_4);
    currBehavior.push_back(dash->currBehavior_5);

 /*   // Speed dials disabled so that I can see GUI changes
    speedDials.push_back(dash->dial_botSpeed_0);
    speedDials.push_back(dash->dial_botSpeed_1);
    speedDials.push_back(dash->dial_botSpeed_2);
    speedDials.push_back(dash->dial_botSpeed_3);
    speedDials.push_back(dash->dial_botSpeed_4);
    speedDials.push_back(dash->dial_botSpeed_5);
    speedDials.push_back(dash->dial_botSpeed_6);
    speedDials.push_back(dash->dial_botSpeed_7);
    speedDials.push_back(dash->dial_botSpeed_8);
    speedDials.push_back(dash->dial_botSpeed_9);
*/
    // X Coordinate vector
    botXcoords.push_back(dash->lcd_coordX_0);
    botXcoords.push_back(dash->lcd_coordX_1);
    botXcoords.push_back(dash->lcd_coordX_2);
    botXcoords.push_back(dash->lcd_coordX_3);
    botXcoords.push_back(dash->lcd_coordX_4);
    botXcoords.push_back(dash->lcd_coordX_5);
//    botXcoords.push_back(dash->lcd_coordX_6);
//    botXcoords.push_back(dash->lcd_coordX_7);
//    botXcoords.push_back(dash->lcd_coordX_8);
//    botXcoords.push_back(dash->lcd_coordX_9);

    // Y Coordinate vector
    botYcoords.push_back(dash->lcd_coordY_0);
    botYcoords.push_back(dash->lcd_coordY_1);
    botYcoords.push_back(dash->lcd_coordY_2);
    botYcoords.push_back(dash->lcd_coordY_3);
    botYcoords.push_back(dash->lcd_coordY_4);
    botYcoords.push_back(dash->lcd_coordY_5);
//    botYcoords.push_back(dash->lcd_coordY_6);
//    botYcoords.push_back(dash->lcd_coordY_7);
//    botYcoords.push_back(dash->lcd_coordY_8);
//    botYcoords.push_back(dash->lcd_coordY_9);

    // Orientation vector
    botOrients.push_back(dash->dial_botOrient_0);
    botOrients.push_back(dash->dial_botOrient_1);
    botOrients.push_back(dash->dial_botOrient_2);
    botOrients.push_back(dash->dial_botOrient_3);
    botOrients.push_back(dash->dial_botOrient_4);
    botOrients.push_back(dash->dial_botOrient_5);
//    botOrients.push_back(dash->dial_botOrient_6);
//    botOrients.push_back(dash->dial_botOrient_7);
//    botOrients.push_back(dash->dial_botOrient_8);
//    botOrients.push_back(dash->dial_botOrient_9);

    // Icon frames vector
    botIconFrames.push_back(dash->gView_robot_0);
    botIconFrames.push_back(dash->gView_robot_1);
    botIconFrames.push_back(dash->gView_robot_2);
    botIconFrames.push_back(dash->gView_robot_3);
    botIconFrames.push_back(dash->gView_robot_4);
    botIconFrames.push_back(dash->gView_robot_5);
//    botIconFrames.push_back(dash->gView_robot_6);
//    botIconFrames.push_back(dash->gView_robot_7);
//    botIconFrames.push_back(dash->gView_robot_8);
//    botIconFrames.push_back(dash->gView_robot_9);


    // Icons vector
    for(int team=0; team<2; team++)
        for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++){
            robotIcon[i] = new GuiRobotDrawer(0,i);
            robotIcon[i]->icon = true;

            robotIconSel[i] = new GuiRobotDrawer(0,0);
            robotIconSel[i]->icon = true;

            scene_botIcon[i] = new QGraphicsScene;
            scene_botIcon[i]->addItem(robotIcon[i]);


            scene_botIconSel[i] = new QGraphicsScene;
            scene_botIconSel[i]->addItem(robotIconSel[i]);
        }


    // relevant vectors filled & connected; initial details added to GUI
    for (int i=0; i<6; i++) {

        botIconFrames[i]->scale(.2, .2);
        botIconFrames[i]->scale(1,-1);
        botIconFrames[i]->rotate(90);

        botIconFrames[i]->setScene(scene_botIcon[i]);
        botIconFrames[i]->setToolTip("Robot " + QString::number(i));
        botIconFrames[i]->hide();

        velocityDials[i]->setValue(0);
    }




    // Formatting robots panel
//    dash->layout_robots->setContentsMargins(QMargins(0,0,0,0));

}//setupBotPanel


void RobotPanel::updateBotPanel() {

    // Printing current bot info to Robot Panels
    int slected_team_id = dash->getSelectedTeamId();
    for (int i=0; i<6; i++) {


        botTitle[i]->setText("Robot " + QString::number(i));
        // Nullcheck
        auto& roboti = GuiRobot::proxies[slected_team_id][i]; //dash->getSelectedTeam()->getRobot(i);
        if ( roboti.hasProxy() ) {
            robotIcon[i]->robot = robotIconSel[i]->robot = &roboti;

//            botIconFrames[i]->setScene(scene_botIcon[slected_team_id][i]);


            botFrames[i]->setEnabled(true);
            botXcoords[i]->display(roboti.getCurrentPosition().x);
            botYcoords[i]->display(roboti.getCurrentPosition().y);
            botOrients[i]->setValue(roboti.getOrientation() +90 );
            robotIcon[i]->setX(0);
            robotIcon[i]->setY(0);
            robotIcon[i]->setZValue(2);

            // if frame hiding icon is not visible, then show icon
            if (!botIconFrames[i]->isVisible())  botIconFrames[i]->show();

            // Velocity dials
            velocityDials[i]->setValue(roboti.getSpeedCommand());
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
//            std::cout << "beh " << slected_team_id << " " << i << " " << roboti.getBehaviorName() << std::endl;
            currBehavior[i]->setText(QString::fromStdString(roboti.getBehaviorName()));


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
    dash->gState->setText(QString::fromStdString(getGameState()));

    //Populating the ballVel label with current ball velocity
    //Fix so that velocity reading are rounded to 2nd or 3rd decimal
    double b_vel = Ball::getSpeed();
    if(b_vel == 0)
        dash->ballVel->setText("Vel: 0 (Stopped)");
    else
    {
        dash->ballVel->setText("Vel: " +
            QString::fromStdString(std::to_string(b_vel)) );
    }

    //Populating the ballAccel label with curent ball acceleration
    dash->ballAccel->setText("Accel: Constant");

    //Populating the currStrategy label with the current strategy
    dash->currStrategy->setText(QString::fromStdString(getCurrStrategy()));




}

void RobotPanel::scrollToSelBot(int id) {
    dash->scroll_robots->ensureVisible(0,91*id,50,50);
}

void RobotPanel::updateTeamColors() {
    if  (dash->getSelectedTeamId() == ROBOT_TEAM_YELLOW) {
        // button color
        dash->btn_toggleTeamColor->setStyleSheet("background-color: yellow;" "color: black");
//        for (int i=0; i<dash->teamSize_blue; i++) {
//            botOrients[i]->setStyleSheet("background-color: rgb(0, 0, 150);");
//            botXcoords[i]->setStyleSheet("background-color: rgb(100, 100, 0);");
//            botYcoords[i]->setStyleSheet("background-color: rgb(100, 100, 0);");
//        }
    } else if (dash->getSelectedTeamId() == ROBOT_TEAM_BLUE){
        // button color
        dash->btn_toggleTeamColor->setStyleSheet("background-color: blue;" "color: white");
        // robot panel colors

        for (int i=0; i<6; i++) {
            botOrients[i]->setStyleSheet("background-color: rgb(255, 255, 0);");
            botXcoords[i]->setStyleSheet("background-color: rgb(0, 0, 150);");
            botYcoords[i]->setStyleSheet("background-color: rgb(0, 0, 150);");

        }

    }

    // rerendering affected objects that aren't regularly updated
    dash->panel_field->gView_field->scene()->update();
    if (dash->panel_field->selectedBot > -1) {
        // dash->panel_selected_robot->gView_robot->scene()->update();
    }
}

void RobotPanel::toggleIconVisible() {
    int id = dash->panel_field->selectedBot;
    int team_id = dash->getSelectedTeamId();
    auto& thisBot = GuiRobot::proxies[team_id][id];
    GuiRobotDrawer *thisIcon = robotIcon[id];

    if (dash->panel_field->selectedBot > -1) {
        if (thisBot.visible) {
            thisBot.visible = false;
//            thisIcon->enabled = false;
            thisIcon->setOpacity(.3);
//            thisBot.setOpacity(.3);
        } else {
            thisBot.visible = true;
//            thisIcon->enabled = true;
            thisIcon->setOpacity(1);
//            thisBot.setOpacity(1);
        }
    }
}

const std::string RobotPanel::getCurrStrategy()
{

    switch(GameState::getRefereeCommand())
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

    switch(GameState::getRefereeCommand())
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

