#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H
#include "strategy/strategy.h"

/*! @file
 *  Strategy to implement a kickoff game state.
 *  The KickOffStrategy moves the robots in a static formation.
 *  There are different configs for both enemy and our kickoffs.
 */

class KickOffStrategy : public Strategy
{
public:
    void assignBeh();
private:
#define NUM_KICK_OFF_POINTS 5
    static Point myKickoffPoints[NUM_KICK_OFF_POINTS];
    static Point opKickoffPoints[NUM_KICK_OFF_POINTS];
};

#endif // KICKOFFSTRATEGY_H
