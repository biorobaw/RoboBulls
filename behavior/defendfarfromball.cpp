#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"

//The distance from the goal where the defend robot stays
#define DISTANCE 500

DefendFarFromBall::DefendFarFromBall(const ParameterList& list)
    : GenericMovementBehavior(list)
    , KTPSkill(nullptr)
    , wasNotPreviousScoreHazard(true)
    , isKickingAwayBall(false)
    , lastKickCounter(0)
{
    UNUSED_PARAM(list);
//    setVelocityMultiplier(0.75);
}

void DefendFarFromBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point robPoint = robot->getRobotPosition();
    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();
    double direction = Measurments::angleBetween(myGoal, ballPoint);

    bool isScoreHazard =
            Measurments::distance(myGoal, ballPoint) < 1200
            and not(Measurments::isClose(robPoint, ballPoint, 200))
            and lastKickCounter <= 0;
//    setMovementTargets(defensiveWall, direction, false);
//    GenericMovementBehavior::perform(robot);

    if(isScoreHazard or isKickingAwayBall) {
        if(wasNotPreviousScoreHazard) {
            KTPSkill = new Skill::KickToPoint(Point(0,0), 50*M_PI/180);
            isKickingAwayBall = true;
            wasNotPreviousScoreHazard = false;
        }
        if(KTPSkill->perform(robot)) {
            lastKickCounter = 100;
            wasNotPreviousScoreHazard = true;
            isKickingAwayBall = false;
            delete KTPSkill;
            KTPSkill = nullptr;
        }
    } else {
        if(lastKickCounter > 0)
            --lastKickCounter;
        Point defensiveWall(cos(direction)*DISTANCE + myGoal.x,
                            sin(direction)*DISTANCE + myGoal.y);
        setMovementTargets(defensiveWall, direction);
        GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
    }
}
