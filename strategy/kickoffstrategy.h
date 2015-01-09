#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H

#include "strategy/strategy.h"

class KickOffStrategy : public Strategy
{
public:
    void assignBeh();
private:
    static Point myKickoffPoints[10];
    static Point opKickoffPoints[10];
};

#endif // KICKOFFSTRATEGY_H
