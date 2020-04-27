#include "frame_robot.h"
#include "ui_frame_robot.h"

FrameRobot::FrameRobot(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRobot)
{
    ui->setupUi(this);
}

FrameRobot::~FrameRobot()
{
    delete ui;
}
