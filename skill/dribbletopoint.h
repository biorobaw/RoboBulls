#ifndef DRIBBLETOPOINT_H
#define DRIBBLETOPOINT_H

#include "skill.h"
#include "utilities/point.h"
#include "movement/gotoposition.h"
#include "model/gamemodel.h"

namespace Skill
{

/*! @brief Uses the dribbler to move the ball to a given point
 * @author Muhaimen Shamsi
 * @details Attempts to push/pull the ball to the point targetPoint
 * with the dribbler active.
 */

class DribbleToPoint: public Skill
{
public:
    DribbleToPoint(Point& target);
    DribbleToPoint(Point* target);
    bool perform(Robot*) override;

private:
    Point* target;
    Point grasp_point;
    Movement::GoToPosition move_skill;
    enum {move_to_ball, grasp, move_to_target, adjust1, adjust2} state;

    bool targetIsAhead(const float& ang_to_ball, const Point& rp);
    bool safeToAdjust(const Point& bp, const int rob_id);
};

}

#endif // DRIBBLETOPOINT_H
