#ifndef DRIVEBALLANDKICK_H
#define DRIVEBALLANDKICK_H
#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/point.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/*! @brief Uses the DriveBall, Kick, and Stop skills. to drive then kick the ball
 * @author Narges Ghaedi
 * @details The robot drives the ball to a kickPoint ( Point(1600, 0) ),
 * and kicks the ball into the goal. */

class DriveBallAndKick : public Behavior
{
public:
    DriveBallAndKick();
    void perform(Robot *);

private:
    enum states {initial, driving, kicking, idling} state;
    Skill::Skill * skill;
    Point *behindBall;
    Point *ballPoint;
    Movement::GoToPosition move;
};

#endif // DRIVEBALLANDKICK_H
