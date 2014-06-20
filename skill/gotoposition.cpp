#include "gotoposition.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "model/gamemodel.h"
#include "include/globals.h"

namespace Skill
{

GoToPosition::GoToPosition(Point target)
{
   // std::cout << this << " GOTOP: " << target.toString() << std::endl;
	targetPosition = target;
	rotationDone = false;
}

GoToPosition::GoToPosition(float tx, float ty)
{
	targetPosition = Point(tx, ty);
	rotationDone = false;
}

void GoToPosition::perform(Robot * robot)
{
    RobComm* nxt       = RobComm::getRobComm();
	Point    rPos      = robot->getRobotPosition();
	float    targetAng = Measurments::angleBetween(rPos, targetPosition);

    float lmv=0, rmv=0;
    Rotate rot(targetAng, &lmv, &rmv);
	rot.perform(robot);

    if(Measurments::isClose(rPos, targetPosition, 1.05*DIST_TOLERANCE) == false) {
        nxt->sendVels(lmv + DIST_VELOCITY, rmv + DIST_VELOCITY, robot->getID());
	} else {
		nxt->sendVels(0, 0, robot->getID());
	}
}


}
