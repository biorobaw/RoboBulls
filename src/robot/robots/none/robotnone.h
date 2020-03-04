#ifndef ROBOTNONE_H
#define ROBOTNONE_H

#include "../../robot.h"
#include "../../navigation/pilots/pilot_dummy.h"

class RobotNone : public Robot
{
public:
    RobotNone(int _id, int _team, RobotRole role);
    bool hasKicker() override;
    Pilot* getPilot() override;
private:
    PilotDummy dummyPilot = PilotDummy(this);

};

#endif // ROBOTNONE_H
