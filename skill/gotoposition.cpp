#include "gotoposition.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"

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
	robComm* nxt       = robComm::getnxtbee();
	Point    rPos      = robot->getRobotPosition();
	float    targetAng = Measurments::angleBetween(rPos, targetPosition);

	Rotate rot(targetAng, false);
	rot.perform(robot);

    if(Measurments::isClose(rPos, targetPosition, 1.05*DIST_TOLERANCE) == false) {
		nxt->sendVels(DIST_VELOCITY, DIST_VELOCITY, robot->getID());
	} else {
		nxt->sendVels(0, 0, robot->getID());
	}
}

}
