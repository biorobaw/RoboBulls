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
 */
#if SIMULATED
int BEHIND_RADIUS  = 200;
int KICK_DISTANCE  = 110;
int FACING_ANGLE_TOL  = 30;
int FORWARD_WAIT_COUNT = 0;
int MOVE_TOLERANCE = DIST_TOLERANCE/2;
#else
int BEHIND_RADIUS  = ROBOT_SIZE;
int KICK_DISTANCE  = 200;
int FACING_ANGLE_TOL  = 20;
int FORWARD_WAIT_COUNT = 15;
int MOVE_TOLERANCE = DIST_TOLERANCE*1.2;
#endif

int RECREATE_DIST_TOL = 25;

/************************************************************************/

KickToPointOmni::KickToPointOmni(const Point& target, float targetTolerance, float kickDistance)
    : KickToPointOmni(&m_targetPoint, targetTolerance, kickDistance)
{
    m_targetPoint = target;
}

KickToPointOmni::KickToPointOmni(Point* targetPtr, float targetTolerance, float kickDistance)
    : m_targetPointer(targetPtr)
    , m_targetTolerance((targetTolerance == -1) ? FACING_ANGLE_TOL*(M_PI/180) : targetTolerance)
    , m_kickDistance(kickDistance)
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

    move_skill.setRecreateTolerances(RECREATE_DIST_TOL, ROT_TOLERANCE);

    switch(state)
    {
    case MOVE_BEHIND:
        {
            // Skip to Kick if feasable
            if(canKick(robot)) {
                state = KICK;
                break;
            }

            //Calculate and set move targets to the BehindTheBall point
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point behindBall = bp + Point(dx, dy);
            move_skill.setVelocityMultiplier(1);
            move_skill.setMovementTolerances(MOVE_TOLERANCE, m_targetTolerance);
            move_skill.recreate(behindBall, ballTargetAng, true, true);

            //Make sure move_skill keeps the robot at the correct pose
            //This is done by waiting for confirmation from the movement class
            if(move_skill.perform(robot))
                ++move_comp_counter;
            //else
                //move_comp_counter = 0;

            if(move_comp_counter > FORWARD_WAIT_COUNT) {
                state = MOVE_FORWARD;
                move_comp_counter = 0;
            }
        }
        break;

    case MOVE_FORWARD:
        {
            // Slowly move towards the ball
            move_skill.recreate(bp, ballTargetAng, false, false);
        #if SIMULATED == 0
            move_skill.setVelocityMultiplier(0.7);
        #endif
            move_skill.perform(robot);

            // Kick when in range
            if(canKick(robot)) {
                state = KICK;
            }
        }
        break;
    case KICK:
        {
            ::Skill::Kick k;
            k.perform(robot);
            state = MOVE_BEHIND;
            return true;
        }
        break;
    }
    return false;
}

//Returns true of the robot is okay to kick the ball
bool KickToPointOmni::canKick(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    bool kickDistOK = m_kickDistance == -1 || Measurments::distance(robot, *m_targetPointer) < m_kickDistance;
    return  Measurments::distance(robot, bp) < KICK_DISTANCE &&
            Comparisons::isFacingPoint(robot,bp,m_targetTolerance*(M_PI/180)) &&
            kickDistOK;
}

}
