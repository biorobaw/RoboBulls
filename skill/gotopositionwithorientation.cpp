#include "gotopositionwithorientation.h"
#include "Utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"
#include "skill/closedloopcontrol.h"
#include "Model/gamemodel.h"
#include <math.h>

GoToPositionWithOrientation::GoToPositionWithOrientation(Point target)
{
    targetPosition = target;
}

GoToPositionWithOrientation::GoToPositionWithOrientation(float tx, float ty)
{
    targetPosition = Point(tx, ty);
}

void GoToPositionWithOrientation::perform(Robot * robot)
{
    robComm* nxtbee= robComm::getnxtbee();
    Point robotPosition = robot->getRobotPosition();
    double robot_x = robotPosition.x;
    double robot_y = robotPosition.y;
    double robot_orientation = robot->getOrientation();
    double finalOrientation = atan2(targetPosition.y-robot_y, targetPosition.x-robot_x);
    wheelvelocities wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation, targetPosition.x, targetPosition.y, finalOrientation);

    float left_wheel_velocity = wheelvelocity.left;
    float right_wheel_velocity = wheelvelocity.right;

    nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());
}
