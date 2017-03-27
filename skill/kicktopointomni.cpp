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
 * move to in order to get in position for a kick. Must be at least 60
 * for obstacle avoidance not to interfere.
 *
 * BEHIND_RAD Defines the distance behind the ball that the robot will move to
 * without considering the ball as an obstacle.
 *
 * FORWARD_WAIT_COUNT Defines the number of times the movement skill must finish
 * (return true) until the robot starts to move forward. On the field, this ensures
 * the robot is actually facing the ball somewhat.
 *
 * KICKLOCK_COUNT is the number of times isInKickLock must see the robot in "kick lock"
 * before it takes action. Kicklock occurs when the robot is close to the ball but not facing it,
 * resulting in the robot continually in a lock pushing the ball in a line.
 *
 * RECREATE_DIST_TOL Defines the distance tolerance that the ball must move to be
 * recreated in the MOVE_BEHIND state. Helps make on-field motion less jittery.
 *
 * STRICTEST_ANGLE_TOL Defines the tightest angle tolerance in degrees that
 * the robot must be facing the kick target with to be able to kick. Results in the most accurate kick.
 * Can be overridden by constructor argument to be less strict when needed.
 *
 */
#if SIMULATED
float BEHIND_RAD_AVOID = ROBOT_RADIUS + BALL_RADIUS+30;
float BEHIND_RAD = ROBOT_RADIUS + BALL_RADIUS;
float FORWARD_WAIT_COUNT = 15;
float RECREATE_DIST_TOL = 25;
float STRICTEST_ANG_TOL = 10 * (M_PI/180);
float KICK_LOCK_ANGLE = 3 * (M_PI/180);
float KICKLOCK_COUNT = 15;
#else
float BEHIND_RAD_AVOID = ROBOT_RADIUS+BALL_RADIUS + 50;
float BEHIND_RAD = ROBOT_RADIUS+BALL_RADIUS;
float FORWARD_WAIT_COUNT = 60;
float RECREATE_DIST_TOL = 25;
float STRICTEST_ANG_TOL = 40 * (M_PI/180);
float KICK_LOCK_ANGLE = 12 * (M_PI/180);
float KICKLOCK_COUNT = 15;
#endif


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
    , m_targetTolerance((targetTolerance < 0) ? STRICTEST_ANG_TOL : targetTolerance)
    , m_kickDistance(kickDistance)
    , m_kickLockCount(0)
    , m_hasRecoveredKickLock(true)
    , m_useFullPower(useFullPower)
    , m_lastBallAwayDist(0)
    , m_ballMovingAwayCount(0)
    , m_kickCommandCount(0)
    , m_hasKicked(false)
    , state(MOVE_BEHIND)

{
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
}

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    GuiInterface::getGuiInterface()->drawLine(bp, *m_targetPointer);

    // Angle between the ball and the kick target
    float ballTargetAng = Measurements::angleBetween(bp, *m_targetPointer);

    // If at any time we are in kick lock, stop and restart out progress (state-wise)
    if(isInKickLock(robot))
        state = MOVE_BEHIND;

    // If at any time we HAVE kicked the ball, and it is moving away, stop. We've finished.
    // This should eventually happen by going through the states below.
    if(m_hasKicked)
    {
        m_hasKicked = false;
        return true;
    }

    //Calculate the point behind the ball to move
    //TODO: factor in ball prediction
    float targetBallAng = Measurements::angleBetween(*m_targetPointer, bp);

    switch(state)
    {
    case MOVE_BEHIND:
        {
    //      std::cout << "KTPO STATE: MOVE BEHIND" << std::endl;
            robot->setDribble(false);

            behindBall = bp + Point(BEHIND_RAD_AVOID * cos(targetBallAng), BEHIND_RAD_AVOID * sin(targetBallAng));
            move_skill.setVelocityMultiplier(1);
            move_skill.updateGoal(behindBall, ballTargetAng, true, true);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_INTERMEDIATE;
                m_hasRecoveredKickLock = true;
                m_moveCompletionCount = 0;
            }

            if(canKick(robot))
                state = MOVE_INTERMEDIATE;
        }
        break;

    case MOVE_INTERMEDIATE:
        {
//            std::cout << "KTPO STATE: MOVE INTERMEDIATE" << std::endl;
            robot->setDribble(false);
            // Move towards the ball at the angle to target
            // Motion will be straight ahead, given the completion of MOVE_BEHIND
            behindBall = bp + Point(BEHIND_RAD * cos(targetBallAng), BEHIND_RAD * sin(targetBallAng));
            move_skill.setMovementTolerances(20, 3*M_PI/180);
            move_skill.setVelocityMultiplier(1);
            move_skill.updateGoal(behindBall, ballTargetAng, false, false);

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

            robot->setDribble(true);
            // Move towards the ball at the angle to target (straight)
            move_skill.setVelocityMultiplier(0.2);
            move_skill.updateGoal(bp - Point(BEHIND_RAD * cos(targetBallAng), BEHIND_RAD * sin(targetBallAng)), ballTargetAng, false, false);
            move_skill.perform(robot);

            /* Kick when in range, or go back to moving behind if it
             * moves too far or we are in kick lock */
            if(canKick(robot))
                state = KICK;
            else if(!isFacingTarget(robot) || isVeryFarFromBall(robot))
                state = MOVE_BEHIND;
        }
        break;
    case KICK:
        {
//            std::cout << "KTPO STATE: KICK" << std::endl;

            // Are we using full power? Otherwise, use distance-based power
            float powerDistance = Measurements::distance(robot, *m_targetPointer);
            if(m_useFullPower)
                powerDistance = Kick::defaultKickDistance;

            // Perform the kick with the power and record that we did
            Kick k(powerDistance);
            k.perform(robot);

            if(m_kickCommandCount < 100)
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
                m_hasKicked = true;
                m_kickCommandCount = 0;
                robot->setDribble(false);
            }
        }
        break;
    }

    return false;
}

//The following are utility functions to help switch state.
//- canKick: All conditions okay to actuate the kicker
//- isWithinKickDistnace: "Within kick distance" means we are close to the target to kick to it
//- isCloseToBall: We are close to the ball if we are within kicking distance
//- isVeryFarFromBall: True if we are pretty far away from the ball
//- isFacingBall: True if facing the ball. The tolerance angle is the user-given m_targetTolerance
/*- isInKickLock: "Kick-lock" is a side-effect of robot physical geometry.
    * Sometimes the robot gets too close to the ball while not facing it, and keeps
    * pushing the ball along in which it cannot get behind it. This helps to detect that */

bool KickToPointOmni::canKick(Robot* robot) {
    bool closeToBall = isCloseToBall(robot);
    bool facingBall = isFacingBall(robot);
    bool facingTarget = isFacingTarget(robot);
    bool withinKickDist =  isWithinKickDistance(robot);
    return m_hasRecoveredKickLock &&
           closeToBall && facingBall && facingTarget
           && withinKickDist;
}

bool KickToPointOmni::isWithinKickDistance(Robot *robot) {
    return m_kickDistance == -1 || Measurements::distance(robot, *m_targetPointer) < m_kickDistance;
}

bool KickToPointOmni::isCloseToBall(Robot *robot) {
    // We cannot simply check the distance to the ball because it is unreliable
    // A misplacement of the markings on top of the robot can render the check tolerance
    // too much or too little. Instead we check how far the robot has travelled from
    // the behindball point.
    //std::cout << measurements::distance(robot, behindBall) << std::endl;
    return Measurements::distance(robot, behindBall) >= 30;
}

bool KickToPointOmni::isVeryFarFromBall(Robot *robot) {
    return Measurements::distance(robot, gameModel->getBallPoint()) > ROBOT_RADIUS*6;
}

bool KickToPointOmni::isFacingBall(Robot* robot) {
    return Comparisons::isFacingPoint(robot, gameModel->getBallPoint(), M_PI/3.0);
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

}
