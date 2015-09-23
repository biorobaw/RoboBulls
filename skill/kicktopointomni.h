#ifndef KTP_OMNI_H
#define KTP_OMNI_H
#include "model/robot.h"
#include "utilities/point.h"
#include "skill/skill.h"
#include "movement/gotoposition.h"

namespace Skill
{

/*! @brief General-purpose  skill to kick the ball to a point
 *  @author JamesW
 * @details A simplified, optimized version of KTP for Omni robots.
 * The ability the track a moving target is preserved with the
 * pointer constructor. The robot is asked to fist move to behind the ball
 * at any angle while rotating to face the ball. Then the Robot moves
 * forwards, and finally kicks the ball when close enough.
 * KTPO supports both a <b>static point</b> and <b>variable point</b> constructors.
 *
 * <b>Example Everyday Usage</b>
 * @include example_kicktopointomni.cpp
 */
class KickToPointOmni : public Skill
{
public:
    /*! @brief <b>Static-point</b> constructor, for kicking to a static point
    * @param target Target point to kick toarwards
    * @param targetTolerance: Tolerance for the robot angle facing the target point before
    * a kick is made, for accuracy or non-accuracy reasons.
    * Leave blank to use default
    * @param kickDistance: How close should the robot be to the target before kicking.
    * leave blank to kick as soon as possible */
    KickToPointOmni(const Point& target,
                    float targetTolerance = -1,
                    float kickDistance = -1);

    /*! @brief <b>variable-point</b> constructor, for kicking to a changing point
     * @param targetPtr Pointer to a point to kick to. Can change while skill is created */
    KickToPointOmni(Point* targetPtr,
                    float  targetTolerance = -1,
                    float  kickDistance = -1);

    bool perform(Robot* robot) override;

private:
    Movement::GoToPosition move_skill;
    Point  m_targetPoint;         //Local (static-point) target stored only by first ctor
    Point* m_targetPointer;       //Pointer to point we are kicking to (m_targetPoint for static)
      int  m_moveCompletionCount; //Number of times move_skill says we are behind ball
    float  m_targetTolerance;     //Mininum angle threshold we must be facing the target to kick
      int  m_kickDistance;        //Mininim distance we must be to *m_targetPointer to kick (or -1)
      int  m_kickLockCount;       //Count of times we are seen in "kick lock"

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
