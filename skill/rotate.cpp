#include "skill/rotate.h"
#include "Utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include <stdio.h>

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
    RobComm *nxt = RobComm::getRobComm();
    float    currentAngle = robot->getOrientation();
    float    angleDiff    = angle_mod(targetAngle - currentAngle);

//    Point rpos = robot->getRobotPosition();
//    printf("Current Angle %f; target %f; tol %f\n", currentAngle, targetAngle, ROT_TOLERANCE);
//    printf("x: %f; y: %f;", rpos.x, rpos.y);
//    printf("AngleDiff: %f; id: %d\n", angleDiff, robot->getID());

    if(fabs(angleDiff) > ROT_TOLERANCE)
    {
        direction = (angle_pos(angleDiff) > M_PI) ? CCLOCKWISE : CLOCKWISE;
        nxt->sendVels(-direction * ROT_VELOCITY, direction * ROT_VELOCITY, robot->getID());

    }
    else
    {
        if(stopWhenDone)
            nxt->sendVels(0, 0, robot->getID());
	}
}
