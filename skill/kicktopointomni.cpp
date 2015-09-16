#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "utilities/debug.h"
#include "skill/kick.h"
#include "skill/kicktopointomni.h"
#include "include/config/simulated.h"

namespace Skill
{

/************************************************************************/
/* USER CONFIGURATION */

/* BEHIND_RADIUS Defines the distance behind the ball that the robot will initially
 * move to to get in position for a kick. Must be at least ROBOT_SIZE
 *
 * KICK_DISTANCE Defines the minimum distance the robot must be near to the ball to
 * kick it.
 *
 * RECREATE_DIST_TOL Defines the distance tolerance that the ball must move to be
 * recreated in the MOVE_BEHIND state. Helps make on-field motion less jittery.
 *
 * FACING_ANGLE_TOL Defines the angle tolerance in degrees (an integer) that
 * the robot must be facing the kick target with to be able to kick.
 *
 * FORWARD_WAIT_COUNT Defines the number of times the movement skill must finish
 * (return true) until the robot starts to move forward. On the field, this ensures
 * the robot is actually facing the ball somewhat.
 *
 * MOVE_TOLERANCE is the tolerance to setMovementTolerances in the first state
 *
 * KICKLOCK_COUNT is the number of times isInKickLock must see the robot in "kick lock"
 * before it take action. Kicklock occurs when the robot is close to the ball but not facing it,
 * resulting in the robot continually in a lock pushing the ball in a line.
 */
#if SIMULATED
int BEHIND_RADIUS  = 200;
int KICK_DISTANCE  = 110;
int FACING_ANGLE_TOL  = 30;
int FORWARD_WAIT_COUNT = 1;
int MOVE_TOLERANCE = DIST_TOLERANCE/2;
#else
int BEHIND_RADIUS  = ROBOT_SIZE;
int KICK_DISTANCE  = 200;
int FACING_ANGLE_TOL  = 20;
int FORWARD_WAIT_COUNT = 15;
int MOVE_TOLERANCE = DIST_TOLERANCE*1.2;
#endif

int KICKLOCK_COUNT = 80;
int RECREATE_DIST_TOL = 25;

/************************************************************************/

KickToPointOmni::KickToPointOmni(const Point& target,
                                 float targetTolerance, float kickDistance, bool useFullPower)
    : KickToPointOmni(&m_targetPoint, targetTolerance, kickDistance, useFullPower)
{
    m_targetPoint = target;
}

KickToPointOmni::KickToPointOmni(Point* targetPtr,
                                 float targetTolerance, float kickDistance, bool useFullPower)
    : m_targetPointer(targetPtr)
    , m_moveCompletionCount(0)
    , m_targetTolerance((targetTolerance == -1) ? FACING_ANGLE_TOL*(M_PI/180) : targetTolerance)
    , m_kickDistance(kickDistance)
    , m_kickLockCount(0)
    , m_useFullPower(useFullPower)
    , state(MOVE_BEHIND)
{
    debug::registerVariable("ktpo_kd", &KICK_DISTANCE);
    debug::registerVariable("ktpo_br", &BEHIND_RADIUS);
    debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    debug::registerVariable("ktpo_fa", &FACING_ANGLE_TOL);
}

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();

    // Angle between the ball and the kick target
    float ballTargetAng = Measurments::angleBetween(bp, *m_targetPointer);

    switch(state)
    {
    case MOVE_BEHIND:
        {
            // Skip to Kick if feasable
            if(canKick(robot)) {
                state = KICK;
                break;
            }

            //Calculate the point behind the ball to move
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point behindBall = bp + Point(dx, dy);
            move_skill.setVelocityMultiplier(1);
            move_skill.setMovementTolerances(MOVE_TOLERANCE, m_targetTolerance);
            move_skill.recreate(behindBall, ballTargetAng, true, true);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot, Movement::Type::facePoint))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_FORWARD;
                m_moveCompletionCount = 0;
            }
        }
        break;
    case MOVE_FORWARD:
        {
            //Move towards the ball at the angle to target (straight)
            move_skill.recreate(bp, ballTargetAng, false, false);
            move_skill.perform(robot);

            /* Kick when in range, or go back to moving behind if it
             * moves too far or we are in kick lock */
            if(canKick(robot)) {
                state = KICK;
            } else if(isVeryFarFromBall(robot) || isInKickLock(robot)) {
                state = MOVE_BEHIND;
            }
        }
        break;
    case KICK:
        {
            float powerDistance = Measurments::distance(robot, *m_targetPointer);
            if(m_useFullPower)
                powerDistance = Kick::defaultKickDistance;
            Kick k(powerDistance);
            k.perform(robot);
            if(ballIsMovingAway(robot) || isVeryFarFromBall(robot)) {
                state = MOVE_BEHIND;
                return true;
            } else {
                state = MOVE_FORWARD;
            }
        }
        break;
    }
    return false;
}



//The following are utility functions to help switch state.
//- canKick: All conditions okay to kick the ball
//- isWithinKickDistnace: "Within kick distance" means we are close to the target to kick to it
//- isCloseToBall: We are close to the ball if we are within kicking distance
//- isVeryFarFromBall: True if we are pretty far away from the ball
//- isFacingBall: True if facing the ball. The tolerance angle is the user-given m_targetTolerance
/*- isInKickLock: "Kick-lock" is a side-effect of robots.
    * Sometimes the robot gets too close to the ball while not facing it, and keeps
    * pushing the ball along in which it cannot get behind it. This helps to detect that */

bool KickToPointOmni::canKick(Robot* robot) {
    return isCloseToBall(robot) && isFacingBall(robot) && isWithinKickDistnace(robot);
}

bool KickToPointOmni::isWithinKickDistnace(Robot *robot) {
    return m_kickDistance == -1 || Measurments::distance(robot, *m_targetPointer) < m_kickDistance;
}

bool KickToPointOmni::isCloseToBall(Robot *robot) {
    return Measurments::distance(robot, gameModel->getBallPoint()) <= KICK_DISTANCE*1.10;
}

bool KickToPointOmni::isVeryFarFromBall(Robot *robot) {
    return Measurments::distance(robot, gameModel->getBallPoint()) > KICK_DISTANCE*3;
}

bool KickToPointOmni::isFacingBall(Robot* robot) {
    return Comparisons::isFacingPoint(robot, gameModel->getBallPoint(), m_targetTolerance);
}

bool KickToPointOmni::isInKickLock(Robot* robot)
{
    if(isCloseToBall(robot) && !isFacingBall(robot))
        ++m_kickLockCount;
    if(m_kickLockCount > KICKLOCK_COUNT)
        return (m_kickLockCount = 0, true);
    return false;
}

/* Check to see if kicking is done or not.
 * It happens when the ball has reasonable velocity that is not facing
 * the robot. That means we kicked
 * FIXME: This is duplicated from DefendBehavior
 */
bool KickToPointOmni::ballIsMovingAway(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    Point bv = gameModel->getBallVelocity();
    Point ba = gameModel->getBallAcceleration();
    //float bs = gameModel->getBallSpeed();
    float ba_mag = std::hypot(ba.x, ba.y);
    float va = atan2(bv.y, bv.x);
    float ballRobAng = Measurments::angleBetween(bp, robot);
    return (ba_mag > 0.001) && !(Measurments::isClose(va, ballRobAng, 90*(M_PI/180)));
}


}
