#ifndef MYGAMEPAD_H
#define MYGAMEPAD_H

#include <QObject>
#include <QtGamepad/QGamepad>
#include <QDebug>
#include "utilities/point.h"

#include "model/team/team.h"


class Robot;

class MyGamepad : public QGamepad
{
    Q_OBJECT
public:
    MyGamepad(int pad_id, QObject* parent);

    void connectToRobot(Robot* robot, bool use_overriden_controller, RobotTeam *team);

private:
    Robot* robot;
    RobotTeam *team;
    bool use_overriden_controller;
private slots:
    void processMotionCommand();
    void processKickCommand();
    void switchRobot();


signals:
    void setVelocity(Point velocity, float angular);
    void setKickSpeed(int speed=5000);


    friend QDebug operator<<(QDebug debug,  MyGamepad* pad);

};

#endif // MYGAMEPAD_H
