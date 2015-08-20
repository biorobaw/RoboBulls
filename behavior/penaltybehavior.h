#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include "behavior/genericmovementbehavior.h"
#include "skill/skill.h"
#include "model/gamemodel.h"
#include "utilities/point.h"

#define PENALTY_BEHAVIOR_DEBUG 0

/*! @brief Behavior to perform a penalty kick
 * @author Narges Ghaedi
 * @details The which has been chosen to kick the ball during penalty (in our the robot
 * that is the closest to the ball at that moment excluding goalie)
 * The robots kicks the ball toward the goal in the opposite position
 * of opponent team's goalie
 * would move toward the ball, stops when it reaches the ball, and kicks the ball.
 * We assume the ball is at the penalty point*/

class PenaltyBehavior : public GenericMovementBehavior
{
public:
    PenaltyBehavior();
    ~PenaltyBehavior();
    void perform(Robot *);
private:
    enum penaltyBehavior_t {initial, kicking, idling} pb;
    Point target;
    Skill::Skill *kickToPoint = nullptr;
    Point ballOrig;
};

#endif // PENALTYBEHAVIOR_H
