#ifndef DRIVER_H
#define DRIVER_H

#include "utilities/point.h"


class Robot;
class RobotTeam;
class GameState;
class CmdGoToPose;

class RobotPilot
{
public:

    RobotPilot(Robot* robot);
    virtual ~RobotPilot();

    void setNewCommand(CmdGoToPose& newCommand);
    void cancelCommand();
    bool finishedCommand();
    bool executeCommands();



protected:
    Robot* robot;
    RobotTeam* team;
    GameState* game_state;

    CmdGoToPose* cmdGoToPose = nullptr;

    bool executeCmdGoToPose(CmdGoToPose* cmd);
    virtual void driveTo(Point targetPose, float targetAngle, Point nextPose) = 0; // function to drive from current position to target position
};

#endif // DRIVER_H
