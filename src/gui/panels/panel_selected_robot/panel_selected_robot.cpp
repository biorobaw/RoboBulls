#include "panel_selected_robot.h"
#include "gui/data/gui_robot.h"
#include <QScrollBar>
#include <iostream>

#define SPEED_MODIFIER 100  // multiplied with units/cycle to get units/second
#define MOVEMENT_MIN 2      // minimum (unmodified) speed detected that we can consider actual movement

PanelSelectedRobot::PanelSelectedRobot(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    hidePanel();
    text_robot->setTextColor(Qt::white);

    // Formatting selected bot panel
    gView_robot->scale(.4, .4);
    gView_robot->scale(1,-1);
    gView_robot->rotate(90);


}

PanelSelectedRobot::~PanelSelectedRobot()
{
}

void PanelSelectedRobot::update_selected_robot(int team_id, int robot_id){
    selected_robot = robot_id;
    selected_team = team_id;
    if(robot_id == -1){
        hidePanel();

    } else {
        //Check for removal when selected; we hide the panel and do nothing
        auto& robot = GuiRobot::proxies[team_id][robot_id];
        if(!robot.hasProxy()) {
            hidePanel();
            return;
        }

        // showing widgets
        update_team_color();
        show();
        box_selected->setTitle("Robot " + QString::number(robot_id) + " selected");

//        cout << "Robot " << id << " speed: " << s << "\n";
        //TODO: fix robot icon issue
//        gView_robot->setScene(dash->robotpanel->scene_botIconSel[id]);
//-        gView_robot_prime->show(); // probably not needed

        // display velocity
        int v = robot.getSpeedCommand();
        dial_velocity->setValue(v);
        lcd_velocity->display(v);

        // change background color of velocity dial and lcd according to sign
        if (dial_velocity->value() > 0) {
            dial_velocity->setStyleSheet("background-color: rgb(0, 200, 0);");
            lcd_velocity->setStyleSheet("background-color: rgb(0, 100, 0);");
        } else if (dial_velocity->value() < 0 ) {
            dial_velocity->setStyleSheet("background-color: rgb(200, 0, 0);");
            lcd_velocity->setStyleSheet("background-color: rgb(100, 0, 0);");
        } else {
            dial_velocity->setStyleSheet("background-color: rgb(150, 150, 150);");
            lcd_velocity->setStyleSheet("background-color: rgb(100, 100, 100);");
        }

        // display speed
        int s = robot.getCurrentSpeed() * SPEED_MODIFIER;
        dial_sepeed->setValue(s);
        lcd_speed->display(s);

        // display position and orientation
        lcd_pos_x->display(robot.getCurrentPosition().x);
        lcd_pos_y->display(robot.getCurrentPosition().y);
        lcd_orientation->display(robot.getOrientationAsString() + 90);
        dial_orientation->setValue(robot.getOrientation() + 90);



        // change background color of speed dial and lcd according to speed level
        int maxSpeed = dial_sepeed->maximum();
        int dial_value = dial_sepeed->value() / maxSpeed;
        if (dial_value <= MOVEMENT_MIN / maxSpeed) {
            dial_sepeed->setStyleSheet("background-color: rgb(150, 150, 150);");
            lcd_speed->setStyleSheet("background-color: rgb(100, 100, 100);");
        } else if( dial_value < 0.2){
            dial_sepeed->setStyleSheet("background-color: rgb(0, 191, 255);");
            lcd_speed->setStyleSheet("background-color: rgb(0, 140, 200);");
        } else if (dial_value < .4) {
            dial_sepeed->setStyleSheet("background-color: rgb(0, 0, 255);");
            lcd_speed->setStyleSheet("background-color: rgb(0, 0, 200);");
        } else if (dial_value < .6) {
            dial_sepeed->setStyleSheet("background-color: rgb(160, 32, 240);");
            lcd_speed->setStyleSheet("background-color: rgb(100, 0, 190);");
        } else if (dial_value < .8) {
            dial_sepeed->setStyleSheet("background-color: rgb(208, 32, 144);");
            lcd_speed->setStyleSheet("background-color: rgb(150, 0, 90);");
        } else {
            dial_sepeed->setStyleSheet("background-color: rgb(255, 20, 147);");
            lcd_speed->setStyleSheet("background-color: rgb(200, 0, 90);");
        }


        // set override checkbox
        bool overriden = GuiRobot::proxies[team_id][robot_id].overridden;
        check_override->setChecked(overriden);


        //TODO:
        // I commented out the following line, they need to be reimplemented
//         dash->guiPrint(robot.getBehaviorName());
         guiPrintRobot(robot_id, "Behavior Keywords:");
         QStringList keywords = getKeyWords(robot.getBehaviorName());
         foreach (QString word, keywords) {
            guiPrintRobot(robot_id, word.toStdString());
         }

        // Set behavior text field
        // TODO: selected robot should not remember behaviors of mulitple robots
        text_robot->setText(botBehavior[robot_id]);
        QScrollBar *sb = text_robot->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

}

void PanelSelectedRobot::hidePanel()
{
    QFrame::hide();
    selected_robot = -1;
    check_override->setChecked(false);
}



QStringList PanelSelectedRobot::getKeyWords(std::string behavior){
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


void PanelSelectedRobot::printBehavior(int team_id, int robot_id) {
    auto& bot = GuiRobot::proxies[team_id][robot_id];
    if (bot.hasProxy()) {
       std::string s = bot.getBehaviorName();
        guiPrintRobot(robot_id, s);
        std::cout << s << "\n";
    }
}



void PanelSelectedRobot::guiPrintRobot(int robotID, string output) {
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


void PanelSelectedRobot::update_team_color(){
    if(selected_team == ROBOT_TEAM_YELLOW){
        setStyleSheet("background-color: rgb(250, 250, 220);");
        text_robot->setStyleSheet("background-color: rgb(100, 100, 0);");
        dial_orientation->setStyleSheet("background-color: rgb(255, 255, 0);");
        lcd_orientation->setStyleSheet("background-color: rgb(0, 0, 150);");
        lcd_pos_x->setStyleSheet("background-color: rgb(100, 100, 0);");
        lcd_pos_y->setStyleSheet("background-color: rgb(100, 100, 0);");
    } else {
        setStyleSheet("background-color: rgb(225, 225, 255);");
        text_robot->setStyleSheet("background-color: rgb(0, 0, 100);");
        dial_orientation->setStyleSheet("background-color: rgb(255, 255, 0);");
        lcd_orientation->setStyleSheet("background-color: rgb(100, 100, 0);");
        lcd_pos_x->setStyleSheet("background-color: rgb(0, 0, 100);");
        lcd_pos_y->setStyleSheet("background-color: rgb(0, 0, 100);");
    }
}
