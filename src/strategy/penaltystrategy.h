#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "src/strategy/strategy.h"
#include "src/strategy/normalgamestrategy.h"
#include "src/behavior/wall.h"
#include "src/model/game_state.h"


/*! @file
 * Strategy to implement a penalty kick game state 'P' and 'p'
 * According to RoboCup rules, the ball is already at the
 * penalty point when this is called, so we just aim at
 * the open part of the goal-post.
 */

class PenaltyStrategy:public Strategy
{
public:
    PenaltyStrategy(Team* _team);
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
