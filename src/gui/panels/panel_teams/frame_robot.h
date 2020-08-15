#ifndef FRAME_ROBOT_H
#define FRAME_ROBOT_H

#include <QFrame>
#include "ui_frame_robot.h"

// Frame used to display robot information by TabTeam.
// Extends the form frame_robot.ui

class FrameRobot : public QFrame , public Ui::FrameRobot
{
    Q_OBJECT

public:
    explicit FrameRobot(QWidget *parent = nullptr);
    ~FrameRobot();

    void update_frame(); // updates the frames information
    void set_robot(int team_id, int robot_id); // sets the robot of the frame


private slots:
    void on_check_robot_stateChanged(int arg1); // processes check box events that control whether to override the robot

private:
    int robot_id=0; // id of the robot
    int team_id=0;  // id of the team the robot belongs to
    QGraphicsScene icon_robot; // graphics item to draw the robot

\

};

#endif // FRAME_ROBOT_H
