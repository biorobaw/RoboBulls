#ifndef KTP_OMNI_H
#define KTP_OMNI_H

#include "model/robot.h"
#include "utilities/point.h"
#include "skill/skill.h"
#include "movement/gotoposition.h"

namespace Skill
{

/* KickToPointOmni:
 * A simplified, optimized version of KTP for Omni robots.
 * The ability the track a moving target is preserved with the
 * pointer constructor.
 *
 * KickToPointOmni makes some assumptions about movement of the robot
 * that apply only to omnis. The robot is asked to move to behind the
 * ball at any angle, then rotated to the ball. In the "moving to ball"
 * state (MOVE_FORWARD), no checks are made for "not facing the ball" because
 * the omni doesn't need to stop and rotate to adjust--only position is
 * checked against
 *
 * It takes the same arguments as the original KickToPoint.
 *target
 *  | Target point to kick toarwards
 *  | `Point* targetPtr` allows the skill to kick to a changing point.
 *  | Pass in a pointer and the skill will continue to allign.
 *targetTolerance:
 *  | Tolerance for the robot angle facing the target point before
 *  | a kick is made, for accuracy or non-accuracy reasons.
 *  | Leave blank to use default
 * kickDistance:
 *  | How close should the robot be to the target before kicking.
 *  | leave blank to kick as soon as possible
 */

class KickToPointOmni : public Skill
{
public:
    //Constant point constructor
    KickToPointOmni(const Point& target,
                    float targetTolerance = -1,
                    float kickDistance = -1);

    //Variable point constructor
    KickToPointOmni(Point* targetPtr,
                    float targetTolerance = -1,
                    float kickDistance = -1);

    bool perform(Robot* robot) override;

private:
    Movement::GoToPosition move_skill;
    Point  m_targetPoint    = Point(0, 0);
    Point* m_targetPointer  = nullptr;
    float  m_hasKickedCount = 0;
    int move_comp_counter = 0;
    enum { MOVE_BEHIND, MOVE_FORWARD, KICK } state = MOVE_BEHIND;

private:
    bool  canKick(Robot *robot);
    float m_targetTolerance;
      int m_kickDistance;
};

}

#endif
