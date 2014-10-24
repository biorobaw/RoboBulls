#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"

//The distance from the goal where the defend robot stays
#define DISTANCE 500

DefendFarFromBall::DefendFarFromBall(const ParameterList& list)
    : GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
//    setVelocityMultiplier(0.75);
}

void DefendFarFromBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();
    double direction = Measurments::angleBetween(myGoal, ballPoint);

    Point defensiveWall(cos(direction)*DISTANCE + myGoal.x, sin(direction)*DISTANCE + myGoal.y);

    setMovementTargets(defensiveWall, direction, false);
    GenericMovementBehavior::perform(robot);
}
