#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H

#include "strategy/strategy.h"
#include "behavior/behavior.h"

class KickOffStrategy:public Strategy
{
public:
    KickOffStrategy();
    void assignBeh();
private:
    Behavior *beh;
};

#endif // KICKOFFSTRATEGY_H
