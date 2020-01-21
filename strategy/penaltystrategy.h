#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "strategy/strategy.h"
#include "strategy/normalgamestrategy.h"
#include "behavior/wall.h"
#include "model/gamemodel.h"


/*! @file
 * Strategy to implement a penalty kick game state 'P' and 'p'
 * According to RoboCup rules, the ball is already at the
 * penalty point when this is called, so we just aim at
 * the open part of the goal-post.
 */

class PenaltyStrategy:public Strategy
{
public:
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
