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
    hide();
    text_robot->setTextColor(Qt::white);

    // Formatting selected bot panel
    gView_robot->scale(.4, .4);
    gView_robot->scale(1,-1);
    gView_robot->rotate(90);


}

PanelSelectedRobot::~PanelSelectedRobot()
{
}

void PanelSelectedRobot::update_panel(){
    // check if theres a robot selected:
    if(selected_robot == -1) return;
    auto& robot = GuiRobot::proxies[selected_team][selected_robot];

    // set override checkbox
    bool overriden = robot.overridden;
    check_override->setChecked(overriden);

    // print behavior
    QString behavoir = "Behavior Keywords:\n";
    foreach (QString word, getKeyWords(robot.getBehaviorName()))
        behavoir += word +"\n";
    text_robot->setText(behavoir);
    QScrollBar *sb = text_robot->verticalScrollBar();
    text_robot->verticalScrollBar()->setValue(sb->maximum());

    // display position and orientation
    lcd_pos_x->display(robot.getCurrentPosition().x);
    lcd_pos_y->display(robot.getCurrentPosition().y);
    lcd_orientation->display(robot.getOrientationAsString() + 90);
    dial_orientation->setValue(robot.getOrientation() + 90);

    // display velocity, dial color changes according to sign
    int v = robot.getSpeedCommand();
    dial_velocity->setValue(v);
    lcd_velocity->display(v);
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

    // display speed, dial color changes according to speed value
    int s = robot.getCurrentSpeed() * SPEED_MODIFIER;
    dial_sepeed->setValue(s);
    lcd_speed->display(s);

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




}

void PanelSelectedRobot::update_selected_robot(int team_id, int robot_id){
    selected_robot = robot_id;
    selected_team = team_id;

    // check if there's a robot selected:
    if(selected_robot == -1 ||
       !GuiRobot::proxies[selected_team][selected_robot].hasProxy()) {
        hide();
        check_override->setChecked(false);
    } else {

        box_selected->setTitle("Robot " + QString::number(selected_robot) + " selected");
        update_team_color();

        // TODO: fix robot icon issue
        // gView_robot->setScene(dash->robotpanel->scene_botIconSel[id]);

        show();

    }

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
