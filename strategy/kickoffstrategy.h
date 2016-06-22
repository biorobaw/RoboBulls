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
    static Point myKickoffPoints[5];
    static Point opKickoffPoints[5];
};

#endif // KICKOFFSTRATEGY_H
