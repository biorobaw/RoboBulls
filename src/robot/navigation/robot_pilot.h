#ifndef DRIVER_H
#define DRIVER_H

#include "utilities/point.h"


class Robot;
class CmdGoToPose;

class Pilot
{
public:
    Pilot(Robot* robot);
    virtual ~Pilot();

    void setManualVelocity(Point _vel,float _angular);
    void goToPose(CmdGoToPose newCommand);
    bool finisedLastCommand();

    bool executeCommands();

    Point getVel();
    float getAngular();

protected:
    Robot* robot;
    Point vel = Point(0,0);
    float angular = 0;

    CmdGoToPose* cmdGoToPose = nullptr;

    bool executeCmdGoToPose(CmdGoToPose* cmd);
    virtual void driveTo(Point targetPose, float targetAngle, Point nextPose) = 0;

};

#endif // DRIVER_H
