#include "panel_selected_robot.h"
#include "gui/data/gui_robot.h"
#include <QScrollBar>
#include <iostream>
#include "gui/graphics/graphics_robot.h"
#include "gui/style_sheets/color_palettes.h"
using std::endl,std::cout;

// SPEED SHOULD BE MEASURED IN mm/s (millimeter per second)
#define SPEED_MODIFIER 1  // multiplied with units/cycle to get units/second
#define MOVEMENT_MIN   20 // minimum (unmodified) speed detected that we can consider actual movement

PanelSelectedRobot::PanelSelectedRobot(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    hide();
    text_robot_output->setTextColor(Qt::white);

    // Formatting selected bot panel
    gView_robot->setScene(&icon_robot);
    gView_robot->scale(.4, .4);
    gView_robot->scale(1,-1);
    gView_robot->rotate(90);

    robot_drawer = new GraphicsRobot(this,0,0, true);
    icon_robot.addItem(robot_drawer);

    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            connect(GuiRobot::get(i,j),SIGNAL(selectedChanged(GuiRobot*)),
                    this,SLOT(update_selected_robot()));

}

PanelSelectedRobot::~PanelSelectedRobot()
{
    if(robot_drawer!=nullptr) delete robot_drawer;
    robot_drawer = nullptr;
}

void PanelSelectedRobot::update_panel(){
    // check if theres a robot selected:
    if(!isVisible()) return;
    auto robot = GuiRobot::get_selected_robot();

    // print behavior and scroll text
    text_robot_output->setText("Behavior Keywords:\n" +
                         robot->getBehaviorName() +"\n");
    QScrollBar *sb = text_robot_output->verticalScrollBar();
    text_robot_output->verticalScrollBar()->setValue(sb->maximum());

    // display position and orientation
    lcd_pos_x->display((int)robot->getCurrentPosition().x);
    lcd_pos_y->display((int)robot->getCurrentPosition().y);
    lcd_orientation->display(robot->getOrientationAsString() + 90);
    dial_orientation->setValue(robot->getOrientation() + 90);

    // display velocity, dial color changes according to sign
    int v = robot->getSpeedCommand();
    dial_velocity->setValue(v);
    lcd_velocity->display(v);
    auto dial_colors = colors_dial_sign[SIGN(v)];
    dial_velocity->setStyleSheet(dial_colors.dial_background);
    lcd_velocity->setStyleSheet (dial_colors.lcd_background);


    // display speed, dial color changes according to speed value
    int s = robot->getCurrentSpeed() * SPEED_MODIFIER;
    dial_sepeed->setValue(s);
    lcd_speed->display(s);

    int index = PERCENTAGE_INDEX(dial_sepeed->value(), MOVEMENT_MIN, dial_sepeed->maximum());
    dial_colors = colors_dial_value[index];
    dial_sepeed->setStyleSheet(dial_colors.dial_background);
    lcd_speed->setStyleSheet  (dial_colors.lcd_background);

}

void PanelSelectedRobot::update_selected_robot(){
    GuiRobot* last_selected = nullptr;
    auto robot = GuiRobot::get_selected_robot();

    // if a robot was last selected disconnect the value of the checkbox
    if(last_selected){
        disconnect(last_selected,SIGNAL(overridenChanged(bool)),check_override,SLOT(setChecked(bool)));
    }

    // check if there's a robot selected:
    if(robot == nullptr) {
        hide();
    } else {

        box_selected->setTitle("Robot " + QString::number(robot->id) + " selected");
        check_override->setChecked(robot->isOverriden());
        update_colors(robot->team);
        robot_drawer->setRobot(robot);
        show();

        repaint();
        gView_robot->repaint();
        connect(robot,SIGNAL(overridenChanged(bool)),check_override,SLOT(setChecked(bool)));
    }

    last_selected = robot;

}



//QStringList PanelSelectedRobot::getKeyWords(std::string behavior){
//    /** Takes a behavior title, parses it for keywords, and returns a list of them
//     * */

//    QStringList keywords;
//    QString title = QString::fromStdString(behavior);

//    if (title.contains("no behavior", Qt::CaseInsensitive)) {
//        keywords.push_back("no behavior");
//    }
//    if (title.contains("ball", Qt::CaseInsensitive)) {
//        keywords.push_back("ball");
//    }
//    if (title.contains("mov", Qt::CaseInsensitive)) {
//        keywords.push_back("moving");
//    }
//    if (title.contains("pass", Qt::CaseInsensitive)) {
//        keywords.push_back("passing");
//    }
//    if (title.contains("send", Qt::CaseInsensitive)) {
//        keywords.push_back("sending");
//    }
//    if (title.contains("receiv", Qt::CaseInsensitive)) {
//        keywords.push_back("receiving");
//    }
//    if (title.contains("defend", Qt::CaseInsensitive)) {
//        keywords.push_back("defending");
//    }
//    if (title.contains("attack", Qt::CaseInsensitive)) {
//        keywords.push_back("attacking");
//    }
//    if (title.contains("farfrom", Qt::CaseInsensitive)) {
//        keywords.push_back("far from");
//    }
//    if (title.contains("nearto", Qt::CaseInsensitive)) {
//        keywords.push_back("near to");
//    }
//    if (title.contains("penalty", Qt::CaseInsensitive)) {
//        keywords.push_back("penalty");
//    }
//    if (title.contains("simple", Qt::CaseInsensitive)) {
//        keywords.push_back("simple");
//    }



//    return keywords;
//}



void PanelSelectedRobot::update_colors(int team){
    auto& colors = team_colors[team];
    setStyleSheet(colors.frame_background);
    dial_orientation->setStyleSheet(colors.dial_orientation_background);
    lcd_orientation->setStyleSheet(colors.text_background);
    text_robot_output->setStyleSheet(colors.text_background);
    lcd_pos_x->setStyleSheet (colors.text_background);
    lcd_pos_y->setStyleSheet (colors.text_background);

}


void PanelSelectedRobot::on_check_override_stateChanged(int arg1)
{
    auto robot = GuiRobot::get_selected_robot();
    robot->setOverriden(arg1);
    gView_robot->repaint();
}
