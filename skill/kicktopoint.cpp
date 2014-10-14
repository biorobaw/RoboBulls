#include "kicktopoint.h"
#include "kick.h"
#include "include/config/simulated.h"
#include "movement/gotopositionwithorientation.h"

#if SIMULATED
 #define KDIST_TOLERANCE    110
 #define CENTER_TOLERANCE   0.40
 #define POSITION_ANGLE_TOL ROT_TOLERANCE
#else
 #define KDIST_TOLERANCE    160
 #define CENTER_TOLERANCE   0.70
 #define POSITION_ANGLE_TOL ROT_TOLERANCE
#endif

namespace Skill
{

KickToPoint::KickToPoint(Point target, float targetTolerance, float kickDistance)
   : state(Positioning)
   , m_kickTarget(target)
   , m_kickAngleTol(targetTolerance)
   , m_kickDistance(kickDistance)
{
#if KICK_TO_POINT_DEBUG
   std::cout << "~KickToPoint~" << std::endl;
#endif
   move_skill.setVelocityMultiplier(1.0);
}


void KickToPoint::doPositioningState(Robot *robot)
{
    move_skill.recreate(behindBall, ballTargetAngle, false);

    if(move_skill.perform(robot))
    {
        state = Moving;
        cout << "KTP: MOVE RETURNS TRUE" << endl;
    }
}


void KickToPoint::doMovingState(Robot *robot)
{
    float robTargetAngle = Measurments::angleBetween(robPoint, m_kickTarget);
    float robBallAngle = Measurments::angleBetween(robPoint, ballPoint);
    robot->setDrible(true);

    bool ballCloseToCenter
        = Measurments::lineDistance(ballPoint, robPoint, m_kickTarget)
            < ROBOT_RADIUS * CENTER_TOLERANCE;

    bool robotFacingTarget
        = Measurments::isClose(robAngle, robTargetAngle, m_kickAngleTol);

    bool robotCloseToBall
        = Measurments::isClose(robPoint, ballPoint, KDIST_TOLERANCE);

    bool robFarFromBall
        = !Measurments::isClose(robPoint, ballPoint, 500);

    bool robotNotFacingBall
        = abs(Measurments::angleDiff(robAngle, robBallAngle))
            > 60*M_PI/180;

    bool robotCanKick
        = m_kickDistance == NO_KICK_DIST ||
          Measurments::isClose(robPoint, m_kickTarget, m_kickDistance);

#if KICK_TO_POINT_DEBUG
    static int count = 0;
    if(++count > 1) {
        count = 0;
        std::cout << ballCloseToCenter << robotFacingTarget
                  << robotCloseToBall  << robotCanKick
                  << std::endl;
    }
#endif

    if(ballCloseToCenter && robotFacingTarget && robotCloseToBall && robotCanKick)
    {
        state = Kicking;
        cout << "KTP: KICKING" << endl;
    }
    else if(robotNotFacingBall || robFarFromBall)
    {
        state = Positioning;
        cout << "KTP: POSITIONING" << endl;
    }
    else
    {
        cout << "KTP: MOVING" << endl;
        move_skill.recreate(m_kickTarget, UNUSED_ANGLE_VALUE, false);
        move_skill.perform(robot);
    }
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
        state = Positioning;
    }
}


bool KickToPoint::perform(Robot * robot)
{
    GameModel* gm = GameModel::getModel();
    ballPoint = gm->getBallPoint();
    robPoint  = robot->getRobotPosition();
    robAngle  = robot->getOrientation();
    targetBallAngle
        = Measurments::angleBetween(m_kickTarget, ballPoint);
    ballTargetAngle
        = Measurments::angleBetween(ballPoint, m_kickTarget);
    behindBall
        = Point(350*cos(targetBallAngle), 350*sin(targetBallAngle))
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

    if (hasKicked)
    {
        hasKicked = false;
        return true;
    }
    return false;
}

}
