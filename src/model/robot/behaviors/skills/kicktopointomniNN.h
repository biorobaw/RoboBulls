#ifndef KTP_OMNINN_H
#define KTP_OMNINN_H
#include "utilities/point.h"
#include "model/robot/behavior.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"
#include "gui/interface/gui_interface.h"



/*! General-purpose  skill to kick the ball to a point
 * details A simplified, optimized version of KTP for Omni robots.
 * The ability the track a moving target is preserved with the
 * pointer constructor. The robot is asked to first move behind the ball
 * while rotating to face the ball. Then the Robot moves forwards, and finally
 * kicks the ball when close enough.
 *
 * KTPO supports both a <b>static point</b> and <b>variable point</b> constructors.
 *
 */
class KickToPointOmniNN : public Behavior
{
public:
    /*! @brief <b>Static-point</b> constructor, for kicking to a static point
    * @param target Target point to kick toarwards
    * @param targetTolerance: Tolerance for the robot angle facing the target point before
    * a kick is made, for accuracy or non-accuracy reasons. Leave blank to use default.
    * @param kickDistance: How close should the robot be to the target before kicking.
    * leave blank to kick as soon as possible
    * @param useFullPower: Use a full-power kick instead of a variable power one to the target */
    KickToPointOmniNN(Robot* robot, const Point& target,
                    float targetTolerance = -1,
                    float kickDistance = -1,
                    bool  useFullPower = false);

    /*! @brief <b>variable-point</b> constructor, for kicking to a changing point
     * @param targetPtr Pointer to a point to kick to. Can change while skill is created
     * @see KickToPointOmni */
    KickToPointOmniNN(Robot* robot, Point* targetPtr,
                    float  targetTolerance = -1,
                    float  kickDistance = -1,
                     bool  useFullPower = false);

    bool perform() override;
    bool isFinished() override;
    string getName() override;
    Point* getShotPoint();
    bool isKicking();


private:
    CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
    Point  m_targetPoint;         //Local (static-point) target stored only by first ctor
    Point* m_targetPointer;       //Pointer to point we are kicking to (m_targetPoint for static)
      int  m_moveCompletionCount; //Number of times move_skill says we are behind ball
    Point behindBall;             //Point behind the ball to navigate to for alighnment with target
    float  m_targetTolerance;     //Mininum angle threshold we must be facing the target to kick
      int  m_kickDistance;        //Mininim distance we must be to *m_targetPointer to kick (or -1)
      int  m_kickLockCount;       //Count of times we are seen in "kick lock"
     bool  m_hasRecoveredKickLock;//If was in KickLock, have recivered (moved behind fully?)
     bool  m_useFullPower;        //Do we perform the kick with full power?
    float  m_lastBallAwayDist;    //How far was the last ball observed from out position?
      int  m_ballMovingAwayCount; //How many times have we seen m_lastBallAwayDist increasing?
      int  m_kickCommandCount;
     bool  m_hasKicked;

     Point Original_bp;
    Point *shot_to;
    // Current skill state
    enum { MOVE_BEHIND,  //We are far from the ball and are moving behind it to face target
           MOVE_INTERMEDIATE,   // We are moving closer to the ball
           MOVE_FORWARD, //We are behind the ball facing target, and are moving forward to kick
           KICK,          //We are kicking the ball
           FACE_GOAL

         } state;

    // Querying information to help switch states
    bool isFacingBall(Robot* robot);
    bool isFacingTarget(Robot* robot);
    bool isCloseToBall(Robot* robot);
    bool isVeryFarFromBall(Robot* robot);
    bool isWithinKickDistance(Robot* robot);
    bool isInKickLock(Robot* robot);
    bool canKick(Robot *robot);
};



#endif
