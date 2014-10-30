#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include <iostream>
#include "behavior/genericmovementbehavior.h"
#include "skill/skill.h"
#include "skill/stop.h"
#include "skill/kick.h"
#include "movement/gotopositionwithorientation.h"
#include "model/gamemodel.h"

#define PENALTY_BEHAVIOR_DEBUG 1

/*
 * Penalty Behavior
 * The which has been chosen to kick the ball during penalty (in our case first robot entering the team)
 * would move toward the ball, stops when it reaches the ball, and kicks the ball.
 * We assume the ball is at the penalty point
 * Narges Ghaedi
 */


class PenaltyBehavior : public GenericMovementBehavior
{
public:
    PenaltyBehavior(const ParameterList& list);
    void perform(Robot *);
private:
    enum penaltyBehavior_t {initial, moving, approching, kicking, idling} pb;
    Movement::GoToPosition move;
    Point target;
};

#endif // PENALTYBEHAVIOR_H
