#include "gotoposition.h"
#include "Measure/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "Functionality/rotate.h"

GoToPosition::GoToPosition()
{
    GoToPosition(Point(0, 0)); //Arbitrary. Needs to be fixed
}

GoToPosition::GoToPosition(Point target)
{
    targetPosition = target;
    rotationDone = false;
}

GoToPosition::GoToPosition(float tx, float ty)
{
    GoToPosition(Point(tx, ty));
}

void GoToPosition::perform(Robot * robot)
{
    robComm* nxt       = robComm::getnxtbee();
    Point    rPos      = robot->getRobotPosition();
    float    targetAng = Measurments::angleBetween(rPos, targetPosition);

    Rotate rot(targetAng, false);
    rot.perform(robot);

    if(Measurments::isClose(rPos, targetPosition, DIST_TOLERANCE) == false) {
        nxt->sendVels(DIST_VELOCITY, DIST_VELOCITY, robot->getID());
    } else {
        nxt->sendVels(0, 0, robot->getID());
    }
}
