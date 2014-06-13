#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H

#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behavior.h"
#include "behavior/penaltybehavior.h"

class PenaltyStrategy:public Strategy
{
public:
    PenaltyStrategy();
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
