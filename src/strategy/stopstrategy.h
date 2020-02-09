#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H
#include "src/strategy/strategy.h"
#include "src/utilities/point.h"
#include "src/utilities/edges.h"

/*! @file
 * Strategy to implement the game stopped state 'S'
 * To implement to stopped game state, we have all robots
 * move to a position that is at least 500mm from the ball */

class StopStrategy:public Strategy
{
public:
    StopStrategy(Team* _team);
    void assignBeh() override;
    bool update() override;
};

#endif // STOPSTRATEGY_H
