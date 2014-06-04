#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H

#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behavior.h"


class StopStrategy:public strategy
{
public:
    StopStrategy();
    void assignBeh();
private:
    Behavior * beh;
};

#endif // STOPSTRATEGY_H
