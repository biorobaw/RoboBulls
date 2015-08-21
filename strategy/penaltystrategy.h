#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behavior.h"
#include "behavior/penaltybehavior.h"

/*! @brief Strategy to implement a penalty kick game state 'P' and 'p'
 *  @author Narges G */

class PenaltyStrategy:public Strategy
{
public:
    PenaltyStrategy();
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
