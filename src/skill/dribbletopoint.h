#ifndef DRIBBLETOPOINT_H
#define DRIBBLETOPOINT_H

#include "skill.h"
#include "utilities/point.h"
#include "movement/go_to_pose.h"
#include "model/game_state.h"

namespace Skill
{

/*! @file
 * Uses the dribbler to move the ball to a given point
 * Attempts to push/pull the ball to the target with the dribbler active.
 *
 * The following motions are performed:
 * - Get near the ball.
 * - Move slowly towards it with the dribbler on to grasp it.
 * - Move to the target.
 *
 * If the flag to prevent backward motion is set, the robot will re-position
 * itself around the ball so that minimal backward motion is required to reach
 * the target.
 */

class DribbleToPoint: public Skill
{
public:
    DribbleToPoint(Point& target, bool avoid_obstacles = true, bool prefer_forward_motion = true);
    DribbleToPoint(Point* target, bool avoid_obstacles = true, bool prefer_forward_motion = true);
    bool perform(Robot*) override;

private:
    Point* target;
    bool avoid_obstacles = true;
    bool prefer_forward_motion = true;
    Point grasp_point;
    Move::GoToPose move_skill;
    enum {move_to_ball, grasp, move_to_target, adjust1, adjust2} state;

    bool targetIsAhead(const float& ang_to_ball, const Point& rp);
    bool safeToAdjust(const Point& bp, Robot* robot);
    bool adjustingIsFaster(Robot*);
};

}

#endif // DRIBBLETOPOINT_H
