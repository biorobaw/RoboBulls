#ifndef ROBOTYISIBOT_H
#define ROBOTYISIBOT_H
#include "../../robot.h"
#include "../../navigation/drives/omni_drive.h"
#include "../../navigation/pilots/pilot_omni.h"


class RobotYisibot : public Robot
{
public:
    RobotYisibot(int id, int team, RobotRole role);

    bool  hasKicker() override;
    Pilot* getPilot() override;
private:
    PilotOmni pilot;

};

#endif // ROBOTYISIBOT_H
