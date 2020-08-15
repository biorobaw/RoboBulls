#ifndef ROBOTLOWLEVELCONTROLS_H
#define ROBOTLOWLEVELCONTROLS_H

#include <QObject>
#include "utilities/point.h"

class RobotLowLevelControls : public QObject
{
    Q_OBJECT
public:
    RobotLowLevelControls(QObject* parent);

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
    void  setTargetVelocity(Point velocity, float angular_speed);
    void  setKickSpeed(int speed = 5000);
    void  setDribble(bool); // TODO: dribble speed can be set in the simulator
    void  setChip(bool);

protected:

    float target_angular_speed = 0;
    Point target_velocity = Point(0,0);
    int  kick_speed = 0;
    bool dribble = false;
    bool chip = false;



};

#endif // ROBOTLOWLEVELCONTROLS_H
