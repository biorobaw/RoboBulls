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
float BEHIND_RADIUS  = ROBOT_SIZE*1.1;
float KICK_DISTANCE  = 220;
float FACING_ANGLE_TOL  = 30;
float FORWARD_WAIT_COUNT = 1;
float MOVE_TOLERANCE = DIST_TOLERANCE/2;
float KICK_LOCK_ANGLE = 3 * (M_PI/180);
#else
float BEHIND_RADIUS  = ROBOT_SIZE;
float KICK_DISTANCE  = 140;
float FACING_ANGLE_TOL  = 20;
float FORWARD_WAIT_COUNT = 15;
float MOVE_TOLERANCE = DIST_TOLERANCE;
float KICK_LOCK_ANGLE = 12 * (M_PI/180);
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
    , m_targetTolerance((targetTolerance == -1) ? FACING_ANGLE_TOL*(M_PI/180) : targetTolerance)
    , m_kickDistance(kickDistance)
    , m_kickLockCount(0)
    , m_hasRecoveredKickLock(true)
    , m_useFullPower(useFullPower)
    , m_lastBallAwayDist(0)
    , m_ballMovingAwayCount(0)
    , m_hasKickedOnce(false)
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

    //If at any time we are in kick lock, stop and restart out progress (state-wise)
    if(isInKickLock(robot))
        state = MOVE_BEHIND;

    //If at any time we HAVE kicked the ball, and it is moving away, stop. We've finished.
    // This should eventually happen by going through the three states below.
    if(m_hasKickedOnce && ballIsMovingAway(robot))
        return true;

    switch(state)
    {
    case MOVE_BEHIND:
        {
            // Skip to Kick if feasable
            if(canKick(robot)) {
                state = KICK;
                break;
            }
            // TODO: If ready to move forward

            robot->setDrible(0); //To stop from MOVE_FORWARD

            // Calculate the point behind the ball to move
            // TODO: factor in ball prediction
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point behindBall = bp + Point(dx, dy);
            move_skill.setMovementTolerances(MOVE_TOLERANCE*0.4, m_targetTolerance);
            move_skill.recreate(behindBall, ballTargetAng, true, true);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_FORWARD1;
                m_hasRecoveredKickLock = true;
                m_moveCompletionCount = 0;
            }
        }
        break;
    case MOVE_FORWARD1:
        {
            //Move towards the ball at the angle to target (straight)
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = KICK_DISTANCE * 0.8 * cos(targetBallAng);
            float dy = KICK_DISTANCE * 0.8 * sin(targetBallAng);
            move_skill.recreate(bp+Point(dx,dy), ballTargetAng, false, false);

            if(move_skill.perform(robot))
                ++m_moveCompletionCount;
            if(m_moveCompletionCount > FORWARD_WAIT_COUNT) {
                state = MOVE_FORWARD2;
                m_hasRecoveredKickLock = true;
                m_moveCompletionCount = 0;
            }

            robot->setDrible(1);
        }
        break;
    case MOVE_FORWARD2:
        {
            //Move towards the ball at the angle to target (straight)
            move_skill.recreate(bp, ballTargetAng, false, false);
            move_skill.perform(robot);
            robot->setDrible(1);

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
            //Stop the dribble game.
            robot->setDrible(0);

            //Are we using full power? Otherwise, use distance-based power
            float powerDistance = Measurments::distance(robot, *m_targetPointer);
            if(m_useFullPower)
                powerDistance = Kick::defaultKickDistance;

            //Perform the kick with the power and record that we did
            Kick k(powerDistance);
            k.perform(robot);
            m_hasKickedOnce = true;

            // Now: We go back to move behind. The skill does not return true until
            // we have seen the ball moving away. That happens above.
            state = MOVE_FORWARD1;
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
    double distance = Measurments::distance(robot, gameModel->getBallPoint());
//    double radial_err = 500 * std::abs(Measurments::angleDiff(robot->getOrientation(),Measurments::angleBetween(robot,gameModel->getBallPoint())));
//    double tangential_distance = distance - radial_err;
//    std::cout << distance << "\t" << radial_err << "\t" << tangential_distance << std::endl;
    return distance <= KICK_DISTANCE;
}

bool KickToPointOmni::isVeryFarFromBall(Robot *robot) {
    return Measurments::distance(robot, gameModel->getBallPoint()) > KICK_DISTANCE*6;
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
 */
bool KickToPointOmni::ballIsMovingAway(Robot* robot)
{
    //Let's look at the distance from the robot to the ball. Okay:
    float thisDist = Measurments::distance(gameModel->getBallPoint(), robot);

    if( thisDist > m_lastBallAwayDist               //The distance is farther away than last
        && abs(thisDist - m_lastBallAwayDist) > 40  //The increase is significant
        && thisDist > ROBOT_SIZE*2)                 //The change is far from robot
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
