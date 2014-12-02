#include <math.h>
#include "defendclosetoball.h"
#include "model/gamemodel.h"
#include "utilities/measurments.h"


DefendCloseToBall::DefendCloseToBall()
    : GenericMovementBehavior()
{
    setVelocityMultiplier(0.75);
}


void DefendCloseToBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point ballPoint  = gm->getBallPoint();
    Point myGoal     = gm->getMyGoal();
    double angle     = Measurments::angleBetween(ballPoint, myGoal);
    double direction = Measurments::angleBetween(myGoal, ballPoint);

    Point frontOfBall(200*cos(angle) + ballPoint.x, 200*sin(angle) + ballPoint.y);
    
    setMovementTargets(frontOfBall, direction);
    GenericMovementBehavior::perform(robot);
}
