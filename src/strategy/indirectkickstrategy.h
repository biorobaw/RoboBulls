#ifndef INDIRECTKICKSTRATEGY_H
#define INDIRECTKICKSTRATEGY_H

#include "src/strategy/strategy.h"
#include "src/model/game_state.h"
#include "src/model/robot.h"

/*! @file
    Strategy to support an indirect kick game state.
    Assigns behaviors for all the robots.
    A goal cannot be scored directly from a free kick.
*/
class IndirectKickStrategy : public Strategy
{
public:
    IndirectKickStrategy(Team* _team);
    void assignBeh();
    char getNextStrategy() override;

private:
    Robot* kicker; // Robot recieving pass to kick ball
    Point initial_bp;
};

#endif // INDIECTKICKSTRATEGY_H
