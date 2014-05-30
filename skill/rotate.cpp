#include <stdio.h>
#include "skill/rotate.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "include/globals.h"

#define CLOCKWISE   1
#define CCLOCKWISE -1

namespace Skill
{

Rotate::Rotate()
{
    //Rotate(0.0f);
    targetAngle = 0;
}

Rotate::Rotate(float tAngle, float* lmvp, float* rmvp)
    : lmv_ptr(lmvp)
    , rmv_ptr(rmvp)
{
    targetAngle  = tAngle;
    stopWhenDone = false;
}

void Rotate::perform(Robot* robot)
{
    RobComm *nxt = RobComm::getRobComm();
    float    currentAngle = robot->getOrientation();
    float    angleDiff    = angle_mod(targetAngle - currentAngle);

    if(fabs(angleDiff) > ROT_TOLERANCE) {
        direction = (angle_pos(angleDiff) > M_PI) ? CCLOCKWISE : CLOCKWISE;

        *lmv_ptr = -direction * ROT_VELOCITY;
        *rmv_ptr = direction * ROT_VELOCITY;

        //nxt->sendVels(-direction * ROT_VELOCITY, direction * ROT_VELOCITY, robot->getID());
    } else {
       // if(stopWhenDone)
           // nxt->sendVels(0, 0, robot->getID());
    }
}

}
