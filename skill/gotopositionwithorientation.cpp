#include "gotopositionwithorientation.h"
#include "Utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/rotate.h"
#include "skill/closedloopcontrol.h"
#include "Model/gamemodel.h"

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

    //********************************************************************
    //Checking shamsi's code
    RobComm *nxtbee = RobComm::getRobComm();
    Point robotPosition = robot->getRobotPosition();
    float robot_x = robotPosition.x;
    float robot_y = robotPosition.y;
    float robot_orientation = robot->getOrientation();
    float finalOrientation = Measurments::slop(robotPosition, targetPosition);
    wheelvelocities wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation, targetPosition.x, targetPosition.y, finalOrientation);

    float left_wheel_velocity = wheelvelocity.left;
    float right_wheel_velocity = wheelvelocity.right;

    nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());
    //********************************************************************
}
