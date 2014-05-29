#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include <iostream>
#include "behavior/behavior.h"

#include "skill/skill.h"
#include "utilities/measurments.h"
#include "skill/basic_movement.h"
#include "skill/gotoposition.h"
#include "skill/rotate.h"
#include "skill/kick.h"

#include "model/gamemodel.h"
#include "utilities/paramlist.h"

/*
 * Penalty Behavior
 * The which has been chosen to kick the ball during penalty (in our case first robot entering the team)
 * would move toward the ball, stops when it reaches the ball, and kicks the ball.
 * We assume the ball is at the penalty point
 * Narges Ghaedi
 */


class PenaltyBehavior : public Behavior
{
public:
    PenaltyBehavior(){};
    PenaltyBehavior(const ParameterList& list);
    void perform(Robot *);

    enum penaltyBehavior_t {moving, kicking, idling} pb;
};

#endif // PENALTYBEHAVIOR_H
