#include "defendclosetoball.h"
#include "model/gamemodel.h"
#include "utilities/point.h"
#include "math.h"
#include "utilities/measurments.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "communication/robcomm.h"

DefendCloseToBall::DefendCloseToBall(const ParameterList& list)
{
}

void DefendCloseToBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();

    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();
//    float direction = Measurments::angleBetween();

    Point frontOfBall(200*cos(Measurments::angleBetween(ballPoint, myGoal)) + ballPoint.x, 200*sin(Measurments::angleBetween(ballPoint, myGoal)) + ballPoint.y);

    ClosedLoopControl clc;
    clc.setVelMultiplier(10);

    double direction = Measurments::angleBetween(myGoal, ballPoint);

    wheelvelocities wheelVel = clc.closed_loop_control(robot, frontOfBall.x, frontOfBall.y, direction);

    RobComm* rc = RobComm::getRobComm();

    rc->sendVels(wheelVel.left, wheelVel.right, robot->getID());

}
