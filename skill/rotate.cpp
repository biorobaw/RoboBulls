#include <stdio.h>
#include "skill/rotate.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"

namespace Skill
{

Rotate::Rotate()
{
    Rotate(0.0f);
}

Rotate::Rotate(float tAngle, bool finishedStop)
{
    targetAngle  = tAngle;
    stopWhenDone = finishedStop;
}

void Rotate::perform(Robot* robot)
{
    robComm* nxt          = robComm::getnxtbee();
    float    currentAngle = robot->getOrientation();
    float    angleDiff    = angle_mod(targetAngle - currentAngle);

    if(fabs(angleDiff) > ROT_TOLERANCE) {
        direction = (angle_pos(angleDiff) > M_PI) ? CCLOCKWISE : CLOCKWISE;
        nxt->sendVels(-direction * ROT_VELOCITY, direction * ROT_VELOCITY, robot->getID());
    } else {
        if(stopWhenDone)
            nxt->sendVels(0, 0, robot->getID());
    }
}

}