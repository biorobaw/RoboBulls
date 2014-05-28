#include "gotoposition.h"
#include "Utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"
#include "skill/closedloopcontrol.h"
#include "Model/gamemodel.h"

//GoToPosition::GoToPosition()
//{
//    GoToPosition(Point(0, 0)); //Arbitrary. Needs to be fixed
//}

GoToPosition::GoToPosition(Point target)
{

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

    cout<<"Target Position" << targetPosition.x << ", " << targetPosition.y<<endl;
//    exit(2);

    Rotate rot(targetAng, false);
    rot.perform(robot);



    if(Measurments::isClose(rPos, targetPosition, DIST_TOLERANCE) == false) {
        nxt->sendVels(DIST_VELOCITY, DIST_VELOCITY, robot->getID());
    } else {
        nxt->sendVels(0, 0, robot->getID());
    }

}
