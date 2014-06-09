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
        targetAngle = 0;
    }

    Rotate::Rotate(float angle, float* lmv_out, float* rmv_out)
        : lmv_ptr(lmv_out)
        , rmv_ptr(rmv_out)
    {
        targetAngle  = angle;
    }

    void Rotate::perform(Robot* robot)
    {
        RobComm *nxt = RobComm::getRobComm();

        float currentAngle = robot->getOrientation();
        float angleDiff = angle_mod(targetAngle - currentAngle);


        if(fabs(angleDiff) > ROT_TOLERANCE) {
            direction = (angle_pos(angleDiff) > M_PI) ? CCLOCKWISE : CLOCKWISE;
        } else {
            direction = 0; //No rotation needed
        }


        /* If the velocities are requested to be outputted,
         * output them and don't send the vels directly.
         */
        if((lmv_ptr != nullptr) && (rmv_ptr != nullptr)) {
            *lmv_ptr = -direction * ROT_VELOCITY;
            *rmv_ptr =  direction * ROT_VELOCITY;
        } else {
            nxt->sendVels(-direction * ROT_VELOCITY, direction * ROT_VELOCITY, robot->getID());
        }
    }

} //namespace Skill
