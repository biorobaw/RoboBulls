#ifndef INDIRECTKICKSTRATEGY_H
#define INDIRECTKICKSTRATEGY_H

#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "model/robot.h"

/*! @file
    Strategy to support an indirect kick game state.
    Assigns behaviors for all the robots.
    A goal cannot be scored directly from a free kick.
*/
class IndirectKickStrategy : public Strategy
{
public:
    IndirectKickStrategy();
    void assignBeh();
    char getNextStrategy() override;

private:
    Robot* kicker; // Robot recieving pass to kick ball
    Point initial_bp;
};

#endif // INDIECTKICKSTRATEGY_H
