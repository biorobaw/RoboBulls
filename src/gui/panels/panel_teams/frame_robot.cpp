#include "frame_robot.h"
#include "gui/data/gui_robot.h"
#include "gui/graphics/graphics_robot.h"
#include "gui/style_sheets/color_palettes.h"
#include <iostream>
using std::cout, std::endl;

FrameRobot::FrameRobot(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    hide();
}

FrameRobot::~FrameRobot()
{

}



void FrameRobot::set_robot(int robot_id, int team_id){

    this->robot_id = robot_id;
    this->team_id = team_id;

    // sets the check box text that identifies the robot
    check_robot->setText("Robot " + QString::number(robot_id).rightJustified(2,' '));

    // set the robot icon graphics view
    gView_icon->setToolTip("Robot " + QString::number(robot_id));
    gView_icon->setScene(&icon_robot);
    gView_icon->scale(.2, .2);
    gView_icon->scale(1,-1);
    gView_icon->rotate(90);
    icon_robot.addItem(new GraphicsRobot(this,team_id,robot_id, true));

    // set colors widget colors based on team
    auto& t_colors = team_colors[team_id];
    dial_orientation->setStyleSheet(t_colors.dial_orientation_background);
    lcd_x->setStyleSheet           (t_colors.text_background);
    lcd_y->setStyleSheet           (t_colors.text_background);
    setStyleSheet(t_colors.frame_background);


    // robots iniialized overriden
    check_robot->setCheckState(Qt::CheckState::Checked);

    // connect the overriden signal to the robot overriden checkbox
    // connect the signal that indicates a new robot has been selected with the reapint signal of the icon
    connect(GuiRobot::get(team_id,robot_id),SIGNAL(overridenChanged(bool)), check_robot,SLOT(setChecked(bool)));
    connect(GuiRobot::get(team_id,robot_id),SIGNAL(selectedChanged(GuiRobot*)), gView_icon,SLOT(repaint()));



}


void FrameRobot::update_frame(){
    auto robot = GuiRobot::get(team_id,robot_id);
    if ( robot->isInField() ) {

        // set behavior
        label_behavior->setText(robot->getBehaviorName());

        // set x, y coords and orientation:
        auto pos = robot->getCurrentPosition();
        lcd_x->display((int)pos.x);
        lcd_y->display((int)pos.y);
        dial_orientation->setValue(robot->getOrientation() + 90);

        // set dial speed and choose its color according to sign
        float s_cmd = robot->getSpeedCommand();
        dial_velocity->setValue(s_cmd);
        auto& color = colors_dial_sign[SIGN(s_cmd)].dial_background;
        dial_velocity->setStyleSheet(color);

        show();

    } else hide();

}

void FrameRobot::on_check_robot_stateChanged(int arg1)
{
    GuiRobot::get(team_id,robot_id)->setOverriden(arg1);
    repaint();
}
