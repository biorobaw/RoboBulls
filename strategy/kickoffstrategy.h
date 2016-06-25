#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H
#include "strategy/strategy.h"

/*! @brief Strategy to implement a kickoff game state 'K' and 'k'
 *  @author JamesW
 *  @details The KickOffStrategy moves the robots in a static formation
 *    based on the order that they appear in the GameModel.
 *    There are configurations for both enemy and our kickoffs */

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
