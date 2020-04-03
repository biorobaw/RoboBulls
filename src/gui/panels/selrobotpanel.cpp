#include <iostream>
#include <QtWidgets/QScrollBar>
#include "selrobotpanel.h"
#include "gui/mainwindow.h"
#include "robotpanel.h"
#include "gamepanel.h"
#include "fieldpanel.h"
#include "gui/guiinterface.h"
#include "ui_mainwindow.h"
#include "gui/data/gui_robot.h"
//#include "model/game_state.h"
#include <typeinfo>
#include <typeindex>
#ifdef __GNUC__
 #include <cxxabi.h>
 #include <unordered_map>
#endif

#define SPEED_MODIFIER 100  // multiplied with units/cycle to get units/second
#define MOVEMENT_MIN 2      // minimum (unmodified) speed detected that we can consider actual movement


SelRobotPanel::SelRobotPanel(MainWindow * mw) {
    dash = mw;
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
}

void SelRobotPanel::printBehavior(int id) {
    int team = dash->getSelectedTeamId();
    auto& bot = GuiRobot::proxies[team][id];
    if (bot.hasProxy()) {
       std::string s = bot.getBehaviorName();
        guiPrintRobot(id, s);
        std::cout << s << "\n";
    }
}

void SelRobotPanel::setupSelRobotPanel() {
    hide();
}

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
        int team = dash->getSelectedTeamId();
        auto& robot = GuiRobot::proxies[team][id];
        if(!robot.hasProxy()) {
            hide();
            return;
        }

        // showing widgets
        dash->ui->frame_primeBot->show();

        v = robot.getSpeedCommand();
        s = robot.getCurrentSpeed() * SPEED_MODIFIER;
//        cout << "Robot " << id << " speed: " << s << "\n";
        dash->ui->gView_robot_prime->setScene(dash->robotpanel->scene_botIconSel[id]);
        dash->ui->gView_robot_prime->show();
        // display velocity
        dash->ui->dial_botVel_->setValue(v);
        dash->ui->lcd_botVel_->display(v);
        // display
        dash->ui->box_primeBot->setTitle("Robot " + QString::number(id) + " selected");
        dash->ui->dial_botSpeed_->setValue(s);
        dash->ui->lcd_botSpeed_->display(s);
        dash->ui->lcd_orient_prime->display(robot.getOrientationAsString() + 90);
        dash->ui->lcd_coordX_prime->display(robot.getCurrentPosition().x);
        dash->ui->lcd_coordY_prime->display(robot.getCurrentPosition().x);
        dash->ui->dial_botOrient_prime->setValue(robot.getOrientation() + 90);
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
        if (dash->ui->dial_botSpeed_->value() > MOVEMENT_MIN) {
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
            int team = dash->getSelectedTeamId();
            int r_id = dash->fieldpanel->selectedBot;
            bool overriden = GuiRobot::proxies[team][r_id].overridden;
            dash->ui->check_botOverride->setChecked(overriden);

        } else {
            dash->ui->check_botOverride->setEnabled(false);
            dash->ui->check_botOverride->hide();
        }

        dash->guiPrint(robot.getBehaviorName());
        guiPrintRobot(id, "Behavior Keywords:");
        QStringList keywords = getKeyWords(robot.getBehaviorName());
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



QStringList SelRobotPanel::getKeyWords(std::string behavior){
    /** Takes a behavior title, parses it for keywords, and returns a list of them
     * */

    QStringList keywords;
    QString title = QString::fromStdString(behavior);

    if (title.contains("no behavior", Qt::CaseInsensitive)) {
        keywords.push_back("no behavior");
    }
    if (title.contains("ball", Qt::CaseInsensitive)) {
        keywords.push_back("ball");
    }
    if (title.contains("mov", Qt::CaseInsensitive)) {
        keywords.push_back("moving");
    }
    if (title.contains("pass", Qt::CaseInsensitive)) {
        keywords.push_back("passing");
    }
    if (title.contains("send", Qt::CaseInsensitive)) {
        keywords.push_back("sending");
    }
    if (title.contains("receiv", Qt::CaseInsensitive)) {
        keywords.push_back("receiving");
    }
    if (title.contains("defend", Qt::CaseInsensitive)) {
        keywords.push_back("defending");
    }
    if (title.contains("attack", Qt::CaseInsensitive)) {
        keywords.push_back("attacking");
    }
    if (title.contains("farfrom", Qt::CaseInsensitive)) {
        keywords.push_back("far from");
    }
    if (title.contains("nearto", Qt::CaseInsensitive)) {
        keywords.push_back("near to");
    }
    if (title.contains("penalty", Qt::CaseInsensitive)) {
        keywords.push_back("penalty");
    }
    if (title.contains("simple", Qt::CaseInsensitive)) {
        keywords.push_back("simple");
    }



    return keywords;
}

