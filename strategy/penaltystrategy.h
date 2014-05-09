#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H

#include "strategy/strategy.h"
#include "Model/gamemodel.h"
#include "behavior/behavior.h"
#include "behavior/penaltybehavior.h"

class PenaltyStrategy:public strategy
{
public:
    PenaltyStrategy();
    void assignBeh();
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
