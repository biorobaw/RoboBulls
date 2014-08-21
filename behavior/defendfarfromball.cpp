#include "defendfarfromball.h"
#include "model/gamemodel.h"
#include "utilities/point.h"
#include "math.h"
#include "utilities/measurments.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "communication/robcomm.h"

//The distance from the goal where the defend robot stays
#define DISTANCE 500

DefendFarFromBall::DefendFarFromBall(const ParameterList& list)
{
}

void DefendFarFromBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();

    ClosedLoopControl clc;
    clc.setVelMultiplier(10);

    double direction = Measurments::angleBetween(myGoal, ballPoint);

    Point defensiveWall(cos(direction)*DISTANCE + myGoal.x, sin(direction)*DISTANCE + myGoal.y);

    wheelvelocities wheelVel = clc.closed_loop_control(robot, defensiveWall.x, defensiveWall.y, direction);

    robot->setL(wheelVel.left);
    robot->setR(wheelVel.right);
}
