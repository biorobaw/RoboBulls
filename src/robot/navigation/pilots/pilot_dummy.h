#ifndef PILOT_DUMMY_H
#define PILOT_DUMMY_H

#include "../robot_pilot.h"

class PilotDummy : public Pilot
{
public:
    PilotDummy(Robot * r);
protected:
    void driveTo(Point targetPose, float targetAngle, Point nextPose) override;
};

#endif // PILOT_DUMMY_H
