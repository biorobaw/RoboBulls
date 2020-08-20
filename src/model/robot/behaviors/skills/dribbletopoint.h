#ifndef DRIBBLETOPOINT_H
#define DRIBBLETOPOINT_H

#include "model/robot/behavior.h"
#include "utilities/point.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"



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

class DribbleToPoint: public Behavior
{
public:
    DribbleToPoint(Robot* robot, Point& target, bool avoid_obstacles = true, bool prefer_forward_motion = true);
    DribbleToPoint(Robot* robot, Point* target, bool avoid_obstacles = true, bool prefer_forward_motion = true);
    bool perform() override;
    bool isFinished() override;
    string getName() override;

private:

    Point* target;
    bool prefer_forward_motion = true;
    Point grasp_point;
    CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
    enum {move_to_ball, grasp, move_to_target, adjust1, adjust2} state;

    bool targetIsAhead(const float& ang_to_ball, const Point& rp);
    bool safeToAdjust(const Point& bp, Robot* robot);
    bool adjustingIsFaster(Robot*);
};



#endif // DRIBBLETOPOINT_H
