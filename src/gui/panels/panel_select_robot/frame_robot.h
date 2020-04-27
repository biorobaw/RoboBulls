#ifndef FRAME_ROBOT_H
#define FRAME_ROBOT_H

#include <QFrame>

namespace Ui {
class FrameRobot;
}

class FrameRobot : public QFrame
{
    Q_OBJECT

public:
    explicit FrameRobot(QWidget *parent = nullptr);
    ~FrameRobot();

private:
    Ui::FrameRobot *ui;
};

#endif // FRAME_ROBOT_H
