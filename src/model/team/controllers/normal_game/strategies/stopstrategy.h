#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H
#include "model/team/team_strategy.h"


/*! @file
 * Strategy to implement the game stopped state 'S'
 * To implement to stopped game state, we have all robots
 * move to a position that is at least 500mm from the ball */

class StopStrategy:public TeamStrategy
{
public:
    StopStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    virtual QString getName() override;
};

#endif // STOPSTRATEGY_H
