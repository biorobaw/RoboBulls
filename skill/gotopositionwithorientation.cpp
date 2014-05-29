#include "gotopositionwithorientation.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"
#include "skill/closedloopcontrol.h"
#include "model/gamemodel.h"
#include <math.h>

namespace Skill {

GoToPositionWithOrientation::GoToPositionWithOrientation(Point target, double goalOrientation)
{
    targetPosition = target;
    this->goalOrientation = goalOrientation;
}

GoToPositionWithOrientation::GoToPositionWithOrientation(float tx, float ty, double goalOrientation)
{
    targetPosition = Point(tx, ty);
    this->goalOrientation = goalOrientation;
}

void GoToPositionWithOrientation::perform(Robot * robot)
{
    RobComm *nxtbee = RobComm::getRobComm();

    Point robotPosition = robot->getRobotPosition();
    double robot_x = robotPosition.x;
    double robot_y = robotPosition.y;
    double robot_orientation = robot->getOrientation();

    wheelvelocities wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation, targetPosition.x, targetPosition.y, goalOrientation);

    float left_wheel_velocity = wheelvelocity.left;
    float right_wheel_velocity = wheelvelocity.right;

    nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());
}

}
