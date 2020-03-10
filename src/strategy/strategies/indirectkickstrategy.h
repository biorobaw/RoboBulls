#ifndef INDIRECTKICKSTRATEGY_H
#define INDIRECTKICKSTRATEGY_H

#include "../strategy.h"
#include "model/game_state.h"
#include "robot/robot.h"

/*! @file
    Strategy to support an indirect kick game state.
    Assigns behaviors for all the robots.
    A goal cannot be scored directly from a free kick.
*/

class IndirectKickStrategy : public Strategy
{
public:
    enum Status {KICKING, KICKED};
    IndirectKickStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    int getStatus() override;

private:
    Robot* kicker; // Robot recieving pass to kick ball
    Point initial_bp;
};

#endif // INDIECTKICKSTRATEGY_H
