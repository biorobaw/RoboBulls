#ifndef ROBOTLOWLEVELCONTROLS_H
#define ROBOTLOWLEVELCONTROLS_H

#include <QObject>
#include "utilities/point.h"

class Robot;

class RobotLowLevelControls : public QObject
{
    Q_OBJECT
public:
    RobotLowLevelControls(QObject* parent, Robot* robot);

    float getTargetAngularSpeed();
    Point getTargetVelocity();
    float getTargetSpeed();
    int   getKickSpeed();
    bool  getDribble();
    bool  getChip();


public slots:
    /*! @brief Set speed ath which the ball should be kicked in mm/s
     * @details *Do not use;* use Skil::Kick instead
     */
    void  setTargetVelocityLocal(Point velocity, float angular_speed);
    void  setTargetVelocityGlobal(Point velocity, float angular_speed);
    void  setKickSpeed(int speed = 5000);
    void  setKickDistance(int distance = 6000); // sets kick speed based on desired distance
    void  setDribble(bool); // TODO: dribble speed can be set in the simulator
    void  setChip(bool);

protected:

    float target_angular_speed = 0;
    Point target_velocity = Point(0,0);
    int  kick_speed = 0;
    bool dribble = false;
    bool chip = false;

    Robot* robot;



};

#endif // ROBOTLOWLEVELCONTROLS_H
