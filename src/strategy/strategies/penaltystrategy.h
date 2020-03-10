#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "../strategy.h"
//#include "normalgamestrategy.h"
#include "../behaviors/wall.h"
#include "model/game_state.h"


/*! @file
 * Strategy to implement a penalty kick game state 'P' and 'p'
 * According to RoboCup rules, the ball is already at the
 * penalty point when this is called, so we just aim at
 * the open part of the goal-post.
 */

class PenaltyStrategy:public Strategy
{
public:
    PenaltyStrategy(RobotTeam* _team);
    void assignBehaviors();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
