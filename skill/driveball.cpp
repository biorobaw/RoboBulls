#include "model/gamemodel.h"
#include "movement/gotopositionwithorientation.h"
#include "skill/skill.h"
#include "math.h"
#include "driveball.h"

#if SIMULATED
    #define CLOSE_ENOUGH 110
    #define ANGLE 13*M_PI/180
    #define DIST 333
    #define VEL_MULT 0.6
    #define TURN_ANG ANGLE*4
#else
    #define CLOSE_ENOUGH 280
    #define ANGLE 5*M_PI/180
    #define DIST 350
    #define VEL_MULT 0.4
    #define TURN_ANG ANGLE*15
#endif

namespace Skill
{

DriveBall::DriveBall(Point targetPoint, double finalDirection)
    : targetPosition(targetPoint)
    , direction(finalDirection)
    , state(moveBehindBall)
{
    move_skill.setMovementTolerances(CLOSE_ENOUGH, ROT_TOLERANCE);
}

bool DriveBall::perform(Robot* robot)
{
    GameModel* gm   = GameModel::getModel();
    Point ballPoint = gm->getBallPoint();
    Point robPoint  = robot->getRobotPosition();
    float targetBallAngle = Measurments::angleBetween(targetPosition, ballPoint);
    float ballTargetAngle =  Measurments::angleBetween(ballPoint, targetPosition);
    Point behindBall      = ballPoint + Point(DIST*cos(targetBallAngle),
                                              DIST*sin(targetBallAngle));
    switch (state)
    {
    case moveBehindBall:
        cout << "moving behind ball" << endl;
        move_skill.setVelocityMultiplier(1.0);
        move_skill.recreate(behindBall, ballTargetAngle, false);
        if(move_skill.perform(robot)) {
            state = driveBall;
        }
        break;

    case driveBall:
        cout << "drive ball" << endl;
        bool farFromBall   = !Measurments::isClose(robPoint, ballPoint, DIST*1.25);
        bool notFacingBall =  Measurments::lineDistance(ballPoint, robPoint, targetPosition)
                > ROBOT_RADIUS*1.75;
        if(farFromBall || notFacingBall) {
            state = moveBehindBall;
        }
        robot->setDrible(true);
        move_skill.setVelocityMultiplier(VEL_MULT);
        move_skill.recreate(this->targetPosition, this->direction, false);
        move_skill.perform(robot, Movement::Type::Default);
        break;
    }

    return (state==driveBall && Measurments::isClose(robPoint, targetPosition, CLOSE_ENOUGH));
}

}
