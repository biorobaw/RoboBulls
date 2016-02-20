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

/* BEHIND_RAD_AVOID Defines the distance behind the ball that the robot will initially
 * move to to get in position for a kick. Must be at least ROBOT_SIZE
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
 * KICKLOCK_COUNT is the number of times isInKickLock must see the robot in "kick lock"
 * before it take action. Kicklock occurs when the robot is close to the ball but not facing it,
 * resulting in the robot continually in a lock pushing the ball in a line.
 */
#if SIMULATED
float BEHIND_RAD_AVOID = ROBOT_SIZE * 0.6;
float BEHIND_RAD = ROBOT_SIZE * 0.4;
float FORWARD_WAIT_COUNT = 15;
float STRICTEST_ANG_TOL = 10 * (M_PI/180);
float KICK_LOCK_ANGLE = 3 * (M_PI/180);
#else
float BEHIND_RAD_AVOID = ROBOT_SIZE * 0.6;
float BEHIND_RAD = ROBOT_SIZE * 0.3;
float FORWARD_WAIT_COUNT = 15;
float KICK_LOCK_ANGLE = 12 * (M_PI/180);
float STRICTEST_ANG_TOL = 40 * (M_PI/180);
#endif

float KICKLOCK_COUNT = 15;
float RECREATE_DIST_TOL = 25;

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
    , m_targetTolerance((targetTolerance == -1) ? STRICTEST_ANG_TOL : targetTolerance)
    , m_kickDistance(kickDistance)
    , m_kickLockCount(0)
    , m_hasRecoveredKickLock(true)
    , m_useFullPower(useFullPower)
    , m_lastBallAwayDist(0)
    , m_ballMovingAwayCount(0)
    , m_kickCommandCount(0)
    , m_hasKickedOnce(false)
    , state(MOVE_BEHIND)

{
    debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
}

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();

    // Angle between the ball and the kick target
    float ballTargetAng = Measurments::angleBetween(bp, *m_targetPointer);

    // If at any time we are in kick lock, stop and restart out progress (state-wise)
    if(isInKickLock(robot))
        state = MOVE_BEHIND;

    // If at any time we HAVE kicked the ball, and it is moving away, stop. We've finished.
    // This should eventually happen by going through the three states below.
    if(m_hasKickedOnce)
    {
        m_hasKickedOnce = false;
        return true;
    }

    //Calculate the point behind the ball to move
    //TODO: factor in ball prediction
    float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);

    switch(state)
    {
    case MOVE_BEHIND:
        {
//            std::cout << "KTPO STATE: MOVE BEHIND" << std::endl;
            behindBall = bp + Point(BEHIND_RAD_AVOID * cos(targetBallAng), BEHIND_RAD_AVOID * sin(targetBallAng));
            move_skill.setVelocityMultiplier(1);
            move_skill.recreate(behindBall, ballTargetAng, true, true);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_INTERMEDIATE;
                m_hasRecoveredKickLock = true;
                m_moveCompletionCount = 0;
            }
        }
        break;

    case MOVE_INTERMEDIATE:
        {
//            std::cout << "KTPO STATE: MOVE INTERMEDIATE" << std::endl;

            // Move towards the ball at the angle to target (straight)
            behindBall = bp + Point(BEHIND_RAD * cos(targetBallAng), BEHIND_RAD * sin(targetBallAng));
            move_skill.setMovementTolerances(20,3*M_PI/180);
            move_skill.setVelocityMultiplier(1);
            move_skill.recreate(behindBall, ballTargetAng, false, false);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_FORWARD;
                m_hasRecoveredKickLock = true;
                m_moveCompletionCount = 0;
            }
        }
        break;

    case MOVE_FORWARD:
        {
//            std::cout << "KTPO STATE: MOVE FORWARD" << std::endl;

            // Move towards the ball at the angle to target (straight)
            move_skill.setVelocityMultiplier(0.2);
            move_skill.recreate(bp - Point(BEHIND_RAD_AVOID * 10 * cos(targetBallAng), BEHIND_RAD_AVOID * 10 * sin(targetBallAng)), ballTargetAng, false, false);
            move_skill.perform(robot);

            /* Kick when in range, or go back to moving behind if it
             * moves too far or we are in kick lock */
            if(canKick(robot)) {
                state = KICK;
            } else if(!isFacingTarget(robot) || isVeryFarFromBall(robot)) {

                state = MOVE_BEHIND;
            }
        }
        break;
    case KICK:
        {
//            std::cout << "KTPO STATE: KICK" << std::endl;

            // Are we using full power? Otherwise, use distance-based power
            float powerDistance = Measurments::distance(robot, *m_targetPointer);
            if(m_useFullPower)
                powerDistance = Kick::defaultKickDistance;

            // Perform the kick with the power and record that we did
            Kick k(powerDistance);
            k.perform(robot);

            if(m_kickCommandCount < 1)
            {
                // We sent the command several times to ensure the robot receives it
                // The robot should actuate one kick every 6 seconds regardless of
                // how many kick commands it receives
                m_kickCommandCount++;
            }
            else
            {
                // We go back to move_behind.
                state = MOVE_BEHIND;
                m_hasKickedOnce = true;
                m_kickCommandCount = 0;
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
    bool closeToBall = isCloseToBall(robot);
    bool facing = isFacingBall(robot);
    bool withinKickDist =  isWithinKickDistance(robot);
    bool facingTarget = isFacingTarget(robot);
    return m_hasRecoveredKickLock &&
           closeToBall && facing &&
           withinKickDist &&
           facingTarget;
}

bool KickToPointOmni::isWithinKickDistance(Robot *robot) {
    return m_kickDistance == -1 || Measurments::distance(robot, *m_targetPointer) < m_kickDistance;
}

bool KickToPointOmni::isCloseToBall(Robot *robot) {
    // We cannot simply check the distance to the ball because it is unreliable
    // A misplacement of the markings on top of the robot can render the check tolerance
    // too much or too little. Instead we check how far the robot has travelled from
    // the behindball point.
    //std::cout << Measurments::distance(robot, behindBall) << std::endl;
    return Measurments::distance(robot, behindBall) >= 50;
}

bool KickToPointOmni::isVeryFarFromBall(Robot *robot) {
    return Measurments::distance(robot, gameModel->getBallPoint()) > ROBOT_RADIUS*6;
}

bool KickToPointOmni::isFacingBall(Robot* robot) {
    return Comparisons::isFacingPoint(robot, gameModel->getBallPoint(), m_targetTolerance);
}

bool KickToPointOmni::isFacingTarget(Robot* robot) {
    return Comparisons::isFacingPoint(robot, *m_targetPointer, m_targetTolerance);
}

bool KickToPointOmni::isInKickLock(Robot* robot)
{
    bool close = isCloseToBall(robot);
    bool facingBall = Comparisons::isFacingPoint(robot, gameModel->getBallPoint(), KICK_LOCK_ANGLE);
    if(close && !facingBall) {
        if(++m_kickLockCount > KICKLOCK_COUNT) {
            m_kickLockCount = 0;
            m_hasRecoveredKickLock = false;
            return true;
        }
    }
    return false;
}

/* Check to see if kicking is done or not.
 * It happens when we have observed the ball moving away, when:
 *  - The current distance of ball-to-robot is greater than the last
 *  - The change is significant
 *  - It happened far from the robot. So we don't record changes of the robot pushing the ball.
 * FIXME: This is duplicated from DefendBehavior
 * FIXME: This is not reliable. Does not return true most of the time when it should.
 *        For now, this is not used and the skill returns after the state-machine
 *        reaches kick.
 */
bool KickToPointOmni::ballIsMovingAway(Robot* robot)
{
    //Let's look at the distance from the robot to the ball. Okay:
    float thisDist = Measurments::distance(gameModel->getBallPoint(), robot);

    bool farther_than_last = thisDist > m_lastBallAwayDist;
    bool increase_is_significant = abs(thisDist - m_lastBallAwayDist) > 40;
    bool far_from_robot = thisDist > ROBOT_SIZE*2;

    std::cout << farther_than_last << increase_is_significant << far_from_robot << std::endl;

    if( farther_than_last && increase_is_significant && far_from_robot)
    {
        m_lastBallAwayDist = thisDist;
        ++m_ballMovingAwayCount;
    }

    //The above three conditions have held enough times.
    //We reset it go not save invalid state over kicking again, if it happens.
    if(m_ballMovingAwayCount > 2) {
        m_ballMovingAwayCount = 0;
        return true;
    }

    return false;
}


}
