#ifndef MYGAMEPAD_H
#define MYGAMEPAD_H

#include <QObject>
#include <QtGamepad/QGamepad>
#include <QDebug>
#include "utilities/point.h"

class Robot;

class MyGamepad : public QGamepad
{
    Q_OBJECT
public:
    MyGamepad(int pad_id, QObject* parent);

    void connectToRobot(Robot* robot, bool use_overriden_controller);

private:
    Robot* robot;

private slots:
    void processMotionCommand();
    void processKickCommand();


signals:
    void setVelocity(Point velocity, float angular);
    void setKickSpeed(int speed=5000);


    friend QDebug operator<<(QDebug debug,  MyGamepad* pad);

};

#endif // MYGAMEPAD_H
