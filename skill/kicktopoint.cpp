#include "kicktopoint.h"
#include "kick.h"
#include "movement/gotopositionwithorientation.h"

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
    move_skill.setRecreateTolerances(40, ROT_TOLERANCE);
    move_skill.setMovementTolerances(ROBOT_RADIUS*1.5, ROT_TOLERANCE*4);
    move_skill.recreate(behindBall, ballTargetAngle, false);

    //std::cout << "KTP POS " << robPoint.toString() << behindBall.toString() << std::endl;

    if(move_skill.perform(robot, Movement::Type::SharpTurns))
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

    bool ballCloseToCenter
        = Measurments::lineDistance(ballPoint, robPoint, m_targetPoint)
            < ROBOT_RADIUS * 0.45;

    bool robotFacingTarget
        = Measurments::isClose(robAngle, robTargetAngle, m_angleTolerance);

    bool robotCloseToBall
        = Measurments::isClose(robPoint, ballPoint, 120);

    bool robotNotFacingBall
        = abs(Measurments::angleDiff(robAngle, robBallAngle))
            > ROT_TOLERANCE *6;

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
    else if(robotNotFacingBall)
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
