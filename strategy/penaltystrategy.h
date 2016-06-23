#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behavior.h"

/*! @brief Strategy to implement a penalty kick game state 'P' and 'p'
 * @details ASSUMPTION: According to RoboCup rules, the ball is already at the
 * penalty point when this is called. Therefore we just do a normal kick-to-goal
 * when this happens.
 *  @author Narges G, James W */

class PenaltyStrategy:public Strategy
{
public:
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
