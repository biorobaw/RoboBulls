#include "frame_robot.h"
#include "ui_frame_robot.h"
#include "gui/data/gui_robot.h"
#include "gui/graphics/graphics_robot.h"
#include "gui/style_sheets/color_palettes.h"
#include <iostream>
using std::cout, std::endl;

FrameRobot::FrameRobot(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    //hide();
}

FrameRobot::~FrameRobot()
{

}



void FrameRobot::set_robot(int robot_id, int team_id){

    this->robot_id = robot_id;
    this->team_id = team_id;

    check_robot->setText("Robot " + QString::number(robot_id).rightJustified(2,' '));

    gView_icon->setToolTip("Robot " + QString::number(robot_id));
    gView_icon->setScene(&icon_robot);
    gView_icon->scale(.2, .2);
    gView_icon->scale(1,-1);
    gView_icon->rotate(90);
    auto robot_drawer = new GraphicsRobot(this,team_id,robot_id, true);
    robot_drawer->setX(0);
    robot_drawer->setY(0);
    robot_drawer->setZValue(2);
    icon_robot.addItem(robot_drawer);

    // set colors widget colors based on team
    auto& t_colors = team_colors[team_id];
    dial_orientation->setStyleSheet(t_colors.dial_orientation_background);
    lcd_x->setStyleSheet           (t_colors.text_background);
    lcd_y->setStyleSheet           (t_colors.text_background);


    check_robot->setCheckState(Qt::CheckState::Checked);

    connect(GuiRobot::get(team_id,robot_id),SIGNAL(overridenChanged(bool)), check_robot,SLOT(setChecked(bool)));
    connect(GuiRobot::get(team_id,robot_id),SIGNAL(selectedChanged(GuiRobot*)), gView_icon,SLOT(repaint()));
//    connect(this,SIGNAL(CLI))

//    setEnabled(true);

//    auto scene = new QGraphicsScene;
//    scene->addItem(robot_drawer);

//    auto robot_icon_sel = new GuiRobotDrawer(0,0);
//    robot_icon_sel->icon = true;

//    auto scene_selec = new QGraphicsScene;


}


void FrameRobot::update_frame(){
    auto robot = GuiRobot::get(team_id,robot_id); //dash->getSelectedTeam()->getRobot(i);
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
