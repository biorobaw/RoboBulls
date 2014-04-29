#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H

#include "strategy.h"
#include "Model/gamemodel.h"
#include "Performance/behavior.h"


class StopStrategy:public strategy
{
public:
    StopStrategy();
    void assignBeh();
private:
    Behavior * beh;
};

#endif // STOPSTRATEGY_H
