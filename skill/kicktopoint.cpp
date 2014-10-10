#include "kicktopoint.h"
#include "kick.h"
#include "include/config/simulated.h"
#include "movement/gotopositionwithorientation.h"

#if SIMULATED
 #define KDIST_TOLERANCE    110
 #define CENTER_TOLERANCE   0.40
 #define POSITION_ANGLE_TOL ROT_TOLERANCE
#else
 #define KDIST_TOLERANCE    DIST_TOLERANCE*2
 #define CENTER_TOLERANCE   0.70
 #define POSITION_ANGLE_TOL ROT_TOLERANCE
#endif

namespace Skill
{

KickToPoint::KickToPoint(Point target, float targetTolerance, float kickDistance)
   : state(Positioning)
   , m_targetPoint(target)
   , m_angleTolerance(targetTolerance)
   , m_kickDistance(kickDistance)
{
#if KICK_TO_POINT_DEBUG
   std::cout << "~KickToPoint~" << std::endl;
#endif
   move_skill.setVelocityMultiplier(1.0);
}


void KickToPoint::doPositioningState(Robot *robot)
{
    move_skill.setMovementTolerances(ROBOT_RADIUS*2, POSITION_ANGLE_TOL);
    move_skill.recreate(behindBall, ballTargetAngle, false);
    move_skill.setVelocityMultiplier(1.0);

    if(move_skill.perform(robot))
        state = Moving;
}


void KickToPoint::doMovingState(Robot *robot)
{
    float robTargetAngle = Measurments::angleBetween(robPoint, m_targetPoint);
    float robBallAngle = Measurments::angleBetween(robPoint, ballPoint);
    robot->setDrible(true);

    move_skill.setRecreateTolerances(80, ROT_TOLERANCE);
    move_skill.setMovementTolerances(DIST_TOLERANCE, ROT_TOLERANCE);
    move_skill.recreate(m_targetPoint, UNUSED_ANGLE_VALUE, false);
    move_skill.setVelocityMultiplier(0.75);

    bool ballCloseToCenter
        = Measurments::lineDistance(ballPoint, robPoint, m_targetPoint)
            < ROBOT_RADIUS * CENTER_TOLERANCE;

    bool robotFacingTarget
        = Measurments::isClose(robAngle, robTargetAngle, m_angleTolerance);

    bool robotCloseToBall
        = Measurments::isClose(robPoint, ballPoint, KDIST_TOLERANCE);

    bool robFarFromBall
        = !Measurments::isClose(robPoint, ballPoint, KDIST_TOLERANCE*5);

    bool robotNotFacingBall
        = abs(Measurments::angleDiff(robAngle, robBallAngle))
            > ROT_TOLERANCE * 6;

    bool robotCanKick
        = m_kickDistance == NO_KICK_DIST ||
          Measurments::isClose(robPoint, m_targetPoint, m_kickDistance);

#if KICK_TO_POINT_DEBUG
    static int count = 0;
    if(++count > 15) {
        count = 0;
        std::cout << ballCloseToCenter << robotFacingTarget
                  << robotCloseToBall  << robotCanKick
                  << std::endl;
    }
#endif

    if(ballCloseToCenter && robotFacingTarget && robotCloseToBall && robotCanKick)
        state = Kicking;
    else if(robotNotFacingBall || robFarFromBall)
        state = Positioning;
    else
        move_skill.perform(robot);
}


void KickToPoint::doKickingState(Robot *robot)
{
#if KICK_TO_POINT_DEBUG
    std::cout << "KTP KICK" << std::endl;
#endif
    if(!hasKicked) {
        ::Skill::Kick k;
        k.perform(robot);
        hasKicked = true;
    }
}


bool KickToPoint::perform(Robot * robot)
{
    GameModel* gm = GameModel::getModel();
    ballPoint = gm->getBallPoint();
    robPoint  = robot->getRobotPosition();
    robAngle  = robot->getOrientation();
    targetBallAngle
        = Measurments::angleBetween(m_targetPoint, ballPoint);
    ballTargetAngle
        = Measurments::angleBetween(ballPoint, m_targetPoint);
    behindBall
        = Point(ROBOT_SIZE*1.2*cos(targetBallAngle), ROBOT_SIZE*1.2*sin(targetBallAngle))
          + ballPoint;

    switch(this->state)
    {
    case Positioning:
        doPositioningState(robot);
        break;
    case Moving:
        doMovingState(robot);
        break;
    case Kicking:
        doKickingState(robot);
        break;
    }

    return (state == Kicking && hasKicked);
}


}
