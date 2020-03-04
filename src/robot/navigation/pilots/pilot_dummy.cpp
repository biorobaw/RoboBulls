#include "pilot_dummy.h"
#include <QtGlobal>

PilotDummy::PilotDummy(Robot* r) :
    Pilot(r)
{

}

void PilotDummy::driveTo(Point targetPose, float targetAngle, Point nextPose){
    Q_UNUSED(targetPose);
    Q_UNUSED(targetAngle);
    Q_UNUSED(nextPose);
}
