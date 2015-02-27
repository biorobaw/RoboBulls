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
 * KICK_COUNT_MAX Defines the number of times the skill has the robot send a
 * "Kick" skill. This gives us greater insurance that the robot's kick will
 * not miss the ball, and is to prevent the robot from instantly going back to
 * the initial state when the kick was ineffective.
 *
 * RECREATE_DIST_TOL Defines the distance tolerance that the ball must move to be
 * recreated in the MOVE_BEHIND state. Helps make on-field motion less jittery.
 *
 * FACING_ANGLE_TOL Defines the angle tolerance that the robot must be facing the kick
 * target to be able to kick.
 */
#if SIMULATED
int BEHIND_RADIUS  = 600;
int KICK_DISTANCE  = 110;
int KICK_COUNT_MAX = 1;
float FACING_ANGLE_TOL  = 60 * (M_PI / 180);
#else
int BEHIND_RADIUS  = 300;
int KICK_DISTANCE  = 135;
int KICK_COUNT_MAX = 1;
float FACING_ANGLE_TOL  = 20 * (M_PI / 180);
#endif

int   RECREATE_DIST_TOL = 25;

/************************************************************************/

KickToPointOmni::KickToPointOmni(const Point& target)
    : KickToPointOmni(&m_targetPoint)
{
    m_targetPoint = target;
}

KickToPointOmni::KickToPointOmni(Point* targetPtr)
    : m_targetPointer(targetPtr)
{
    debug::registerVariable("ktpo_kd", &KICK_DISTANCE);
    debug::registerVariable("ktpo_br", &BEHIND_RADIUS);
    debug::registerVariable("ktpo_kc", &KICK_COUNT_MAX);
    debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
}

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();

    //Check used to skip to final state
    bool canKick = Comparisons::isFacingPoint(robot, *m_targetPointer, FACING_ANGLE_TOL)
               and Measurments::distance(robot, bp) < KICK_DISTANCE;

    //Angle between the ball and the kick target
    float ballTargetAng = Measurments::angleBetween(bp, *m_targetPointer);


    switch(state)
    {
    case MOVE_BEHIND:
        {
            //Calculate and set move targets to the BehindTheBall point
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point behindBall = bp + Point(dx, dy);

            move_skill.recreate(behindBall, ballTargetAng, false, false);
            move_skill.setRecreateTolerances(RECREATE_DIST_TOL, M_PI);
            move_skill.setVelocityMultiplier(1.0);
            
            //Check to skip to the last state if we manage to be able to kick in this state
            if(canKick) {
                state = MOVE_FORWARD;
            }
            else if(move_skill.perform(robot)) {
                state = ADJUST_ANGLE;
            }
        }
        break;
    case ADJUST_ANGLE:
        {
            //We move the robot to itself with a different angle, getting rotation only
            move_skill.recreate(robot->getRobotPosition(), ballTargetAng, false, false);
        #if !SIMULATED
            move_skill.setVelocityMultiplier(0.6);
        #endif
            move_skill.perform(robot);
            
            //Advance when robot is facing the kick target
            if(Comparisons::isFacingPoint(robot, *m_targetPointer, FACING_ANGLE_TOL)) {
                state = MOVE_FORWARD;
            }
        }
        break;
    case MOVE_FORWARD:
        {
            //If we end up far away from that ball, set to go behind it again
            if(Measurments::distance(robot, bp) > BEHIND_RADIUS * 1.5) {
                state = MOVE_BEHIND;
            }
            //Otherwise if we are just not in kick range, move torwards it slowly
            else if(Measurments::distance(robot, bp) > KICK_DISTANCE) {
                move_skill.recreate(bp, ballTargetAng, false, false);
                move_skill.setVelocityMultiplier(0.8);
                move_skill.perform(robot);
            } else {
                /* Otherwise if we are in kick range, kick the ball
                 *There's another check of "canKick" here for ensuring vality of
                 * facing angle
                 */
                if(canKick) {
                    ::Skill::Kick k;
                    k.perform(robot);
                    state = MOVE_BEHIND;
                    return true;
                }
            }
        }
        break;
    }
    
    return false;
}

}
