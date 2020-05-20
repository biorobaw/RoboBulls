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

    void update_frame();
    void set_robot(int robot_id, int team_id);


private:
    int robot_id=0;
    int team_id=0;
    QGraphicsScene icon_robot;

//public slots:
//    void test();

};

#endif // FRAME_ROBOT_H
