#include "defendclosetoball.h"
#include "model/gamemodel.h"
#include "utilities/point.h"
#include "math.h"
#include "utilities/measurments.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "communication/robcomm.h"

DefendCloseToBall::DefendCloseToBall(const ParameterList& list)
{
    UNUSED_PARAM(list);
}

void DefendCloseToBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();

    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();

    double angle = Measurments::angleBetween(ballPoint, myGoal);

    Point frontOfBall(200*cos(angle) + ballPoint.x, 200*sin(angle) + ballPoint.y);
//    ClosedLoopSharpTurns clc;
    ClosedLoopControl clc;
    clc.setVelMultiplier(10);

    double direction = Measurments::angleBetween(myGoal, ballPoint);

    wheelvelocities wheelVel = clc.closed_loop_control(robot, frontOfBall.x, frontOfBall.y, direction);

    robot->setL(wheelVel.left);
    robot->setR(wheelVel.right);
}
