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
    Point defensiveWall(cos(direction)*DISTANCE + myGoal.x,
                        sin(direction)*DISTANCE + myGoal.y);

    // Calculate the distances to the nearest teammate
    // and the nearest opponent
    double nearest_teammate=9001;
    double nearest_opponent=9000;
    double distance;
    for(Robot* myRob:gm->getMyTeam())
    {
        distance = Measurments::distance(myRob->getRobotPosition(),ballPoint);
        if (distance < nearest_teammate && myRob->getID() != 5)
            nearest_teammate = distance;
    }
    for(Robot* opRob:gm->getOponentTeam())
    {
        distance = Measurments::distance(opRob->getRobotPosition(),ballPoint);
        if (distance < nearest_opponent && opRob->getID() != 5)
            nearest_opponent = distance;
    }

    bool isScoreHazard =
            Measurments::distance(myGoal, ballPoint) < 1200
            and not(Measurments::isClose(robPoint, ballPoint, 200))
            and lastKickCounter <= 0
            and nearest_teammate < nearest_opponent;

    if(isScoreHazard or isKickingAwayBall) {
        if(wasNotPreviousScoreHazard) {
            KTPSkill = new Skill::KickToPoint(Point(0,0), 50*M_PI/180);
            isKickingAwayBall = true;
            wasNotPreviousScoreHazard = false;
        }
        if(KTPSkill->perform(robot) or
                Measurments::distance(ballPoint, myGoal) > 1200){
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
        setMovementTargets(defensiveWall, direction, true);
        GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
    }
}

