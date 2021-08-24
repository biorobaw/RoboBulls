#include "mygamepad.h"
#include <QDebug>

#include "model/robot/robot.h"

#define MAX_ANGUAR 3.14  // 2pi radians per second
#define MAX_LINEAR 500   // 500mm/s = 0.5m/s

MyGamepad::MyGamepad(int pad_id, QObject* parent) :
    QGamepad(pad_id,parent)
{

}

void MyGamepad::connectToRobot(Robot* robot, bool use_overriden_controller ,RobotTeam *team){
    this->team = team;
    this->robot = robot;
    this->use_overriden_controller = use_overriden_controller;
    auto controller = use_overriden_controller ? robot->getOverridenController() : robot;
    connect(this, &QGamepad::axisLeftXChanged , this, &MyGamepad::processMotionCommand);
    connect(this, &QGamepad::axisLeftYChanged , this, &MyGamepad::processMotionCommand);
    connect(this, &QGamepad::axisRightXChanged, this, &MyGamepad::processMotionCommand);
    connect(this, &MyGamepad::setVelocity, controller, &RobotLowLevelControls::setTargetVelocityLocal);

    connect(this, &QGamepad::buttonL1Changed, controller, &RobotLowLevelControls::setDribble);

    connect(this, &QGamepad::buttonR1Changed, this, &MyGamepad::processKickCommand);
    connect(this, &MyGamepad::setKickSpeed, controller, &RobotLowLevelControls::setKickSpeed);

    connect(this, &QGamepad::buttonL2Changed, this, &MyGamepad::switchRobot);


    //disconnect(this, nullptr, nullptr, nullptr);

//    connect(this, SIGNAL(buttonAChanged), controller, [](bool pressed){
//        qDebug() << "Button A" << pressed;
//    });
//    connect(this, &QGamepad::buttonR1Changed, controller, [](bool pressed){
//        qDebug() << "Button R1" << pressed;
//    });

//    connect(this, SIGNAL(axisRightYChanged), controller, [](double value){
//        qDebug() << "Right Y" << value;
//    });
//    connect(this, &QGamepad::buttonBChanged, controller, [](bool pressed){
//        qDebug() << "Button B" << pressed;
//    });
//    connect(this, &QGamepad::buttonXChanged, controller, [](bool pressed){
//        qDebug() << "Button X" << pressed;
//    });
//    connect(this, &QGamepad::buttonYChanged, controller, [](bool pressed){
//        qDebug() << "Button Y" << pressed;
//    });


//    connect(this, &QGamepad::buttonL2Changed, controller, [](double value){
//        qDebug() << "Button L2: " << value;
//    });
//    connect(this, &QGamepad::buttonR2Changed, controller, [](double value){
//        qDebug() << "Button R2: " << value;
//    });
//    connect(this, &QGamepad::buttonSelectChanged, controller, [](bool pressed){
//        qDebug() << "Button Select" << pressed;
//    });
//    connect(this, &QGamepad::buttonStartChanged, controller, [](bool pressed){
//        qDebug() << "Button Start" << pressed;
//    });
//    connect(this, &QGamepad::buttonGuideChanged, controller, [](bool pressed){
//        qDebug() << "Button Guide" << pressed;
//    });
}

namespace  {
    double removeDeadZone(double value){
        if(value < -0.1) value +=0.1;
        else if(value > 0.1) value -= 0.1;
        else value = 0;
        return value/0.9;
    }
}

void MyGamepad::processMotionCommand(){
    auto x   = -removeDeadZone(axisLeftY() );
    auto y   = -removeDeadZone(axisRightX() );
    auto vel = Point(x,y)*MAX_LINEAR;
    auto w   = -removeDeadZone(axisLeftX())*MAX_ANGUAR;
    emit this->setVelocity(vel,w);
}

void MyGamepad::processKickCommand(){
    if(buttonR1())
        emit this->setKickSpeed();
}

void MyGamepad::switchRobot(){
    if(!(buttonL2() == 1))
        return;

    disconnect(this, nullptr, nullptr, nullptr);

    do{
        this->robot = team->getRobot((robot->getId()+1) %MAX_ROBOTS_PER_TEAM);
    } while (!this->robot->isInField());

    qInfo() <<"Switched to robot:  " <<robot->getId();


    auto controller = this->use_overriden_controller ? robot->getOverridenController() : robot;
    connect(this, &QGamepad::axisLeftXChanged , this, &MyGamepad::processMotionCommand);
    connect(this, &QGamepad::axisLeftYChanged , this, &MyGamepad::processMotionCommand);
    connect(this, &QGamepad::axisRightXChanged, this, &MyGamepad::processMotionCommand);
    connect(this, &MyGamepad::setVelocity, controller, &RobotLowLevelControls::setTargetVelocityLocal);

    connect(this, &QGamepad::buttonL1Changed, controller, &RobotLowLevelControls::setDribble);

    connect(this, &QGamepad::buttonR1Changed, this, &MyGamepad::processKickCommand);
    connect(this, &MyGamepad::setKickSpeed, controller, &RobotLowLevelControls::setKickSpeed);

    connect(this, &QGamepad::buttonL2Changed, this, &MyGamepad::switchRobot);

    //auto controller = use_overriden_controller ? robot->getOverridenController() : robot;

            //teams[team_id]->getRobot(robot_id)
    //if(buttonR1())
    //    emit this->setKickSpeed();
}




QDebug operator<<(QDebug debug, MyGamepad* pad){
    QDebugStateSaver saver(debug);
    debug.nospace() << "Joy " << pad->deviceId() << " name " << pad->name()
                    << " contected to robot " << pad->robot->getId() << " team " << pad->robot->getTeamId();
    return debug;
}
