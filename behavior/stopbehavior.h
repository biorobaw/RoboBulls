#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include "behavior/genericmovementbehavior.h"

/*
 * Stop Behavior
 * All robots position themself one meter away from the ball
 * and face the ball
 * Narges Ghaedi
 */
class StopBehavior : public GenericMovementBehavior
{
public:
    StopBehavior(const ParameterList &list);
    void perform(Robot *);
};

#endif // STOPBEHAVIOR_H
