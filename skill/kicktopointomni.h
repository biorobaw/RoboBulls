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
                    float  targetTolerance = -1,
                    float  kickDistance = -1);

    bool perform(Robot* robot) override;

private:
    Movement::GoToPosition move_skill;
    Point  m_targetPoint;           //Local (static) target stored only by first ctor
    Point* m_targetPointer;         //Pointer to point we are kicking to
      int  m_moveCompletionCount;   //Number of times move_skill says we are behind ball
    float  m_targetTolerance;       //Mininum angle threshold we must be facing the target to kick
      int  m_kickDistance;          //Mininim distance we must be to *m_targetPointer to kick (or -1)
      int  m_kickLockCount;         //Count of times we are seen in "kick lock"

    //Current skill state
    enum { MOVE_BEHIND,  //We are far from the ball and are moving behind it to face target
           MOVE_FORWARD, //We are behind the ball facing target, and are moving forward to kick
           KICK          //We are kicking the ball
         } state;

    //Querying information to help switch states
    bool isFacingBall(Robot* robot);
    bool isCloseToBall(Robot* robot);
    bool isVeryFarFromBall(Robot* robot);
    bool isWithinKickDistnace(Robot* robot);
    bool isInKickLock(Robot* robot);
    bool canKick(Robot *robot);
};

}

#endif
