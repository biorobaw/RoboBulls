#ifndef DRIBBLEBACK_H
#define DRIBBLEBACK_H

#include "skill.h"
#include "utilities/point.h"
#include "movement/go_to_pose.h"
#include "model/gamemodel.h"

namespace Skill
{

/*! @file
 * Uses the dribbler to move the ball to a given point while driving
 * backwards. Does not perform obstacle avoidance.
 *
 * This skill will position the robot in the line between the ball
 * and the target, turn on the dribbler, make the robot approach
 * the ball, and move straight backwards once the ball is grasped by
 * the dribbler. It applies an acceleration while moving back.
 *
 * Useful when the DribbleToPoint skill's path-planning
 * is not desired or when a guarantee is needed that the motion will
 * be backwards. As such, this should be deprecated when DribbleToPoint
 * is able to achive the same functionality through input parameters.
 */

class DribbleBack : public Skill
{
public:
    DribbleBack(Point& target);
    DribbleBack(Point* target);
    bool perform(Robot*) override;
private:
    Point* target;
    Point grasp_point;
    Move::GoToPose move_skill;
    enum {move_to_ball, grasp, move_back} state = move_to_ball;
    float prev_vel = 0;
};

}


#endif // DRIBBLEBACK_H
