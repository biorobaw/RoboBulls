#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include <iostream>
#include "behavior/genericmovementbehavior.h"
#include "skill/skill.h"
#include "skill/stop.h"
#include "skill/kick.h"
#include "movement/gotopositionwithorientation.h"
#include "model/gamemodel.h"
#include "skill/kicktopoint.h"

#define PENALTY_BEHAVIOR_DEBUG 0

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
    PenaltyBehavior();
    void perform(Robot *);
private:
    enum penaltyBehavior_t {initial, changeSign, kicking, idling} pb;
    Point target;
    Skill::KickToPoint *kickToPoint;
    bool sign;  //0 is positive and 1 is negative
    bool targetSign;
};

#endif // PENALTYBEHAVIOR_H
