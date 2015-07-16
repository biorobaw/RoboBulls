#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"

//The distance from the goal where the defend robot stays
#define DISTANCE 300
#define VEL_CHANGE_COUNT 3500

int DefendFarFromBall::goalieDist = 900;

DefendFarFromBall::DefendFarFromBall()
    : GenericMovementBehavior()
    , KTPSkill(nullptr)
    , wasNotPreviousScoreHazard(true)
    , isKickingAwayBall(false)
    , lastKickCounter(0)
    , velChangeCounter(0)
    , isOnSlowVelMode(false)
{
    setVelocityMultiplier(0.65);
}

void DefendFarFromBall::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point robPoint = robot->getRobotPosition();
    Point ballPoint = gm->getBallPoint();
    Point myGoal = gm->getMyGoal();

    /* Check if there are any opp robots within 3000 distance of the ball
    *  This boolean is used to determine if the goalie should wait
    *  before kicking the ball to a teammate in case an opp robot intersepts
    *  the ball
    *  * TODO: Probably remove this
    */
    bool safeToKick = 1;
    /*
    for(Robot* opRob:gm->getOponentTeam())
    {
        if (Measurments::distance(opRob->getRobotPosition(),ballPoint) < 3000)
            safeToKick = 0;
    }
    */

    bool isScoreHazard =
            Measurments::distance(myGoal, ballPoint) < goalieDist
            and Measurments::distance(myGoal, ballPoint) > 300
            and not(Measurments::isClose(robPoint, ballPoint, 100))
            and lastKickCounter <= 0
            and safeToKick;

    if(isScoreHazard or isKickingAwayBall) {
        //Initially this goes here
        if(wasNotPreviousScoreHazard) {
            KTPSkill = new Skill::KickToPointOmni(Point(0,0));
            isKickingAwayBall = true;
            wasNotPreviousScoreHazard = false;
        }
        //Performing the kick and stopping if finished
        if(KTPSkill->perform(robot) or
                Measurments::distance(ballPoint, myGoal) > 1200){
            lastKickCounter = 100;
            wasNotPreviousScoreHazard = true;
            isKickingAwayBall = false;
            delete KTPSkill;
            KTPSkill = nullptr;
        }
    } else {
        //Logic to be able to kick again
        if(lastKickCounter > 0) {
            --lastKickCounter;
        }
        if(++velChangeCounter >= VEL_CHANGE_COUNT) {
            velChangeCounter = 0;
            isOnSlowVelMode = !isOnSlowVelMode;
        }
        if(isOnSlowVelMode) {
            setVelocityMultiplier(0.60);
        } else {
            setVelocityMultiplier(0.75);
        }

        //Logic to not be dumb following out-of-view balls
        Point  cmpPoint(myGoal.x + 500*GameModel::opSide, 0);
        double    cmpAng = Measurments::angleBetween(cmpPoint, ballPoint);
        double   realAng = Measurments::angleBetween(myGoal, ballPoint);
        double centerAng = Measurments::angleBetween(cmpPoint, Point(0,0));
        double direction = realAng;
        if(abs(cmpAng) > 100*(M_PI/180))
            direction = centerAng;

        //Just sitting and facing the ball
        Point defensiveWall(cos(direction)*DISTANCE + myGoal.x,
                            sin(direction)*DISTANCE + myGoal.y);
        setMovementTargets(defensiveWall, direction, false, false);
        GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
    }
}

