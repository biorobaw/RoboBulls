#ifndef FREEKICKSTRATEGY_H
#define FREEKICKSTRATEGY_H
#include "strategy/strategy.h"
#include "utilities/point.h"
class Robot;

/*! @brief Strategy to implement a free kick game state 'F' and 'f'
 *  @author Muhaimen Shamsi*/

class FreeKickStrategy: public Strategy
{
public:
    FreeKickStrategy();
    void assignBeh();
    char getNextStrategy() override;

private:
    // Robot that is sent to kick the ball
    Robot* kicker = nullptr;

    // Stores the position of the ball when this strategy starts
    // so that we know when the opponents have moved the ball
    Point initial_bp;
};

#endif // FREEKICKSTRATEGY_H
