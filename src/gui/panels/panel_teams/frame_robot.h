#ifndef FRAME_ROBOT_H
#define FRAME_ROBOT_H

#include <QFrame>
#include "ui_frame_robot.h"

class FrameRobot : public QFrame , public Ui::FrameRobot
{
    Q_OBJECT

public:
    explicit FrameRobot(QWidget *parent = nullptr);
    ~FrameRobot();

};

#endif // FRAME_ROBOT_H
