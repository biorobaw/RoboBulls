#include <iostream>
#include <QtWidgets/QScrollBar>
#include "selrobotpanel.h"
#include "mainwindow.h"
#include "robotpanel.h"
#include "objectposition.h"
#include "gamepanel.h"
#include "fieldpanel.h"
#include "guiinterface.h"
#include "ui_mainwindow.h"
#include "getbehavior.h"
#include "guirobot.h"
#include "model/gamemodel.h"

SelRobotPanel::SelRobotPanel(MainWindow * mw) {
    dash = mw;
}


void SelRobotPanel::setGuiOverride() {
    // Required for Override to work with Vision
}

void SelRobotPanel::guiPrintRobot(int robotID, string output) {
    /* Converts given string into a QString, ensures it hasn't already been
     * printed during this cycle, and prints it on the Selected Robot Panel.
     *
     * robotID: ID # of the robot to whom this comment applies
     * output:  comment string to be printed
     */

    // ensuring we haven't already printed this string during this cycle...
    if (botBehavior[robotID].indexOf(QString::fromStdString(output)) == -1) {
        // ensuring we don't lead with a carriage return if the field is currently blank...
        if (botBehavior[robotID] == "") {
            botBehavior[robotID] += (QString::fromStdString(output));
        } else {
            botBehavior[robotID] += ("\n" + QString::fromStdString(output));
        }
    }
    // test
    //    botBehavior[robotID] = ("\n" + QString::fromStdString(output));
}

void SelRobotPanel::printBehavior(int id) {
    if (dash->gamemodel->find(id,dash->gamemodel->getMyTeam()) != NULL) {
       Robot * bot = dash->gamemodel->find(id, dash->gamemodel->getMyTeam());
       std::string s = getbehavior->getBehaviorName(bot);
        guiPrintRobot(id, s);
//        dash->guiPrint(getbehavior->getBehaviorName(bot));
        std::cout << s << "\n";
    }
}

void SelRobotPanel::setupSelRobotPanel() {
    hide();

    return;
}//end guiPrintRobot()

void SelRobotPanel::updateSelectedBotPanel(int id) {
    int v = 0;
    int s = 0;
    if (id == -1) {
        dash->ui->gView_robot_prime->hide();
        dash->ui->dial_botVel_->setValue(0);
        dash->ui->lcd_botVel_->display(0);
        dash->ui->lcd_orient_prime->display("0");
        dash->ui->lcd_coordX_prime->display("0");
        dash->ui->lcd_coordY_prime->display("0");
        dash->ui->dial_botOrient_prime->setValue(0);
        dash->ui->box_primeBot->setTitle(" ");
        dash->ui->text_primeBot->setText(" ");
        // hiding widgets
        hide();
    } else {
        //Check for removal when selected; we hide the panel and do nothing
        if(!dash->gamemodel->findMyTeam(id)) {
            hide();
            return;
        }

        // showing widgets
        dash->ui->frame_primeBot->show();

        v = dash->objectPos->getVelocity(id);
        s = dash->objectPos->botSpeeds[id] * dash->objectPos->speedModifier;
//        cout << "Robot " << id << " speed: " << s << "\n";
        dash->ui->gView_robot_prime->setScene(dash->robotpanel->botIconSelScenes[id]);
        dash->ui->gView_robot_prime->show();
        // display velocity
        dash->ui->dial_botVel_->setValue(v);
        dash->ui->lcd_botVel_->display(v);
        // display
        dash->ui->box_primeBot->setTitle("Robot " + QString::number(id) + " selected");
        dash->ui->dial_botSpeed_->setValue(s);
        dash->ui->lcd_botSpeed_->display(s);
        dash->ui->lcd_orient_prime->display(dash->objectPos->getBotOrientString(id));
        dash->ui->lcd_coordX_prime->display(dash->objectPos->getBotCoordX(true, id));
        dash->ui->lcd_coordY_prime->display(dash->objectPos->getBotCoordY(true,id));
        dash->ui->dial_botOrient_prime->setValue(dash->objectPos->getBotOrientDouble(true, id));
        // Selected Bot Panel velocity dial
        if (dash->ui->dial_botVel_->value() > 0) {
            dash->ui->dial_botVel_->setStyleSheet("background-color: rgb(0, 200, 0);");
            dash->ui->lcd_botVel_->setStyleSheet("background-color: rgb(0, 100, 0);");
        } else if (dash->ui->dial_botVel_->value() < 0 ) {
            dash->ui->dial_botVel_->setStyleSheet("background-color: rgb(200, 0, 0);");
            dash->ui->lcd_botVel_->setStyleSheet("background-color: rgb(100, 0, 0);");
        } else {
            dash->ui->dial_botVel_->setStyleSheet("background-color: rgb(150, 150, 150);");
            dash->ui->lcd_botVel_->setStyleSheet("background-color: rgb(100, 100, 100);");
        }

        // Selected Bot Panel speed dial
        int maxSpeed = dash->ui->dial_botSpeed_->maximum();
        if (dash->ui->dial_botSpeed_->value() > dash->objectPos->movementMin) {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(0, 191, 255);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(0, 140, 200);");
            if (dash->ui->dial_botSpeed_->value() > maxSpeed*.2) {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(0, 0, 255);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(0, 0, 200);");
            }
            if (dash->ui->dial_botSpeed_->value() > maxSpeed*.4) {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(160, 32, 240);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(100, 0, 190);");
            }
            if (dash->ui->dial_botSpeed_->value() > maxSpeed*.6) {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(208, 32, 144);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(150, 0, 90);");
            }
            if (dash->ui->dial_botSpeed_->value() > maxSpeed*.8) {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(255, 20, 147);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(200, 0, 90);");
            }
        } else {
            dash->ui->dial_botSpeed_->setStyleSheet("background-color: rgb(150, 150, 150);");
            dash->ui->lcd_botSpeed_->setStyleSheet("background-color: rgb(100, 100, 100);");
        }

        // Override checkbox
        if (dash->fieldpanel->selectedBot > -1) {
            dash->ui->check_botOverride->setEnabled(true);
            dash->ui->check_botOverride->show();
            if (dash->overriddenBots[dash->fieldpanel->selectedBot] == true) {
                dash->ui->check_botOverride->setChecked(true);
            } else {
                dash->ui->check_botOverride->setChecked(false);
            }
        } else {
            dash->ui->check_botOverride->setEnabled(false);
            dash->ui->check_botOverride->hide();
        }

        dash->guiPrint(getbehavior->getBehaviorName(dash->gamemodel->find(id, dash->gamemodel->getMyTeam())));
        guiPrintRobot(id, "Behavior Keywords:");
        QStringList keywords = dash->objectPos->getKeyWords(getbehavior->getBehaviorName(dash->gamemodel->find(id, dash->gamemodel->getMyTeam())));
        foreach (QString word, keywords) {
            guiPrintRobot(id, word.toStdString());
        }

        // Text field
        dash->ui->text_primeBot->setTextColor(Qt::white);
        dash->ui->text_primeBot->setText(botBehavior[id]);
        QScrollBar *sb = dash->ui->text_primeBot->verticalScrollBar();
        sb->setValue(sb->maximum());



    }
}

void SelRobotPanel::hide()
{
    dash->ui->frame_primeBot->hide();
    dash->fieldpanel->selectedBot = -1;
    dash->ui->check_botOverride->setChecked(false);
}
