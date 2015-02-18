#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "skill/kick.h"
#include "skill/kicktopointomni.h"

namespace Skill
{

/************************************************************************/
/* USER CONFIGURATION */

/* Defines the distance behind the ball that the robot will initially move
 * to to get in position for a kick. Must be at least ROBOT_SIZE
 */
#define BEHIND_RADIUS  (ROBOT_SIZE * 2)
/* Defines the minimum distance the robot must be near to the ball to kick it.
 * Could need a SIMULATED / non-SIMULATED version
 */
#define KICK_DISTANCE  120
/* Defines the number of times the skill has the robot send a "Kick" skill.
 * This gives us greater insurance that the robot's kick will not miss the ball,
 * and is to prevent the robot from instantly going back to the initial state
 * when the kick was ineffective.
 */
#define KICK_COUNT_MAX 2

/************************************************************************/

KickToPointOmni::KickToPointOmni(const Point& target)
    : m_targetPoint(target)
    , m_targetPointer(&m_targetPoint)
    { }

KickToPointOmni::KickToPointOmni(Point* targetPtr)
    : m_targetPointer(targetPtr)
    { }

bool KickToPointOmni::perform(Robot* robot)
{
    Point bp = gameModel->getBallPoint();

    switch(state)
    {
    case MOVE_BEHIND:
        {
            float targetBallAng = Measurments::angleBetween(*m_targetPointer, bp);
            float dx = BEHIND_RADIUS * cos(targetBallAng);
            float dy = BEHIND_RADIUS * sin(targetBallAng);
            Point moveTarget = bp + Point(dx, dy);
            move_skill.recreate(moveTarget, UNUSED_ANGLE_VALUE, false, false);
            move_skill.setMovementTolerances(120, M_PI);
            
            if(move_skill.perform(robot)) {
                state = ADJUST_ANGLE;
            }
        }
        break;
    case ADJUST_ANGLE:
        {
            float ballTargAng = Measurments::angleBetween(bp, *m_targetPointer);
            move_skill.recreate(bp, ballTargAng, false, false);
            move_skill.perform(robot, Movement::Type::facePoint);
            
            if(Comparisons::isFacingPoint(robot, *m_targetPointer, M_PI_4)) {
                state = MOVE_FORWARD;
            }
        }
        break;
    case MOVE_FORWARD:
        {
            float robBallAng = Measurments::angleBetween(robot, bp);

            if(Measurments::distance(robot, bp) > KICK_DISTANCE) {
                move_skill.recreate(bp, robBallAng, false, false);
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
