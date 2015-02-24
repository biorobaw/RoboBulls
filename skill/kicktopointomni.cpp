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
 * kick it.Could need a SIMULATED / non-SIMULATED version
 *
 * KICK_COUNT_MAX Defines the number of times the skill has the robot send a
 * "Kick" skill. This gives us greater insurance that the robot's kick will
 * not miss the ball, and is to prevent the robot from instantly going back to
 * the initial state when the kick was ineffective.
 */
#if SIMULATED
int BEHIND_RADIUS  = 600;
int KICK_DISTANCE  = 110;
int KICK_COUNT_MAX = 1;
#else
int BEHIND_RADIUS  = 300;
int KICK_DISTANCE  = 135;
int KICK_COUNT_MAX = 2;
#endif

int RECV_DIST_TOL = 25;

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
    debug::registerVariable("ktpo_rc", &RECV_DIST_TOL);
}

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    float robBallAng = Measurments::angleBetween(robot, bp);

    switch(state)
    {
    case MOVE_BEHIND:
        {
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point moveTarget = bp + Point(dx, dy);

            move_skill.recreate(moveTarget, robBallAng, true, true);
            move_skill.setRecreateTolerances(RECV_DIST_TOL, M_PI);
            move_skill.setVelocityMultiplier(1.0);
            
            if(move_skill.perform(robot)) {
                state = ADJUST_ANGLE;
            }
        }
        break;
    case ADJUST_ANGLE:
        {
            float robBallAng = Measurments::angleBetween(robot, bp);
            float ballRobAng = Measurments::angleBetween(bp, robot);

            Point target = bp + Point(KICK_DISTANCE*1.3*cos(ballRobAng),
                                      KICK_DISTANCE*1.3*sin(ballRobAng));

            move_skill.recreate(robot->getRobotPosition(), robBallAng, false, false);
            move_skill.setVelocityMultiplier(0.6);
            move_skill.perform(robot);
            
            if(Comparisons::isFacingPoint(robot, target, 15*(M_PI/180))) {
                state = MOVE_FORWARD;
            }
        }
        break;
    case MOVE_FORWARD:
        {
            float robBallAng = Measurments::angleBetween(robot, bp);

            if(Measurments::distance(robot, bp) > KICK_DISTANCE) {
                move_skill.recreate(bp, robBallAng, false, false);
                move_skill.setVelocityMultiplier(0.8);
                move_skill.perform(robot);
            } else {
                ::Skill::Kick k;
                k.perform(robot);
                if(++m_hasKickedCount >= KICK_COUNT_MAX) {
                    m_hasKickedCount = 0;
                    state = MOVE_BEHIND;
                }
                return true;
            }
        }
        break;
    }
    
    return false;
}

}
