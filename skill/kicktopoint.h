#ifndef KICKTOPOINT_H
#define KICKTOPOINT_H

#include "skill/skill.h"
#include "model/gamemodel.h"
#include "movement/gotoposition.h"

namespace Skill
{

/* KickToPoint: Positions the robot behind the ball, and kicks
 * the ball to (torwards) a specified point.
 * Constructor Params:
*target
*  | Target point to kick toarwards
*targetTolerance:
*  | Tolerance for the robot angle facing the target point before
*  | a kick is made, for accuracy reasons
* kickDistance:
*  | How close should the robot be to the target before kicking.
*  | use NO_KICK_DIST no kick as soon as possible.
 */

/* Constant that indicates the robot should kick as soon as possible;
 * instead of only driving the ball until a certain distance away
 */
#define NO_KICK_DIST -1

#define KICK_TO_POINT_DEBUG 0

class KickToPoint : public Skill
{
public:
    KickToPoint(Point target
              , float targetTolerance = 10*M_PI/180
              , float kickDistance = NO_KICK_DIST);

    bool perform(Robot *) override;

private:
    enum {Positioning, Moving, Kicking} state;

    void doPositioningState(Robot* robot);
    void doMovingState(Robot* robot);
    void doKickingState(Robot* robot);

    Movement::GoToPosition move_skill;
    Point m_kickTarget;
    float m_kickAngleTol;
    float m_kickDistance;
    Point robPoint;
    Point ballPoint;
    Point behindBall;
    float ballTargetAngle;
    float targetBallAngle;
    float robAngle;
    bool  hasKicked = false;
};

} //namespace Skill

#endif // KICKTOPOINT_H
