#ifndef ROBOTGRSIM_H
#define ROBOTGRSIM_H
#include "../../robot.h"
#include "../../navigation/drives/omni_drive.h"
#include "../../navigation/pilots/pilot_omni.h"

#define LEFT_FRONT  0
#define LEFT_BACK   1
#define RIGHT_BACK  2
#define RIGHT_FRONT 3

class RobotGrsim : public Robot
{
public:
    RobotGrsim(int id, int team, RobotRole role);

    bool  hasKicker() override;
    Pilot* getPilot() override;

    void getWheelSpeeds(double wheelSpeeds[4]);

private:
    OmniDrive drive;
    PilotOmni pilot;

};

#endif // ROBOTGRSIM_H
