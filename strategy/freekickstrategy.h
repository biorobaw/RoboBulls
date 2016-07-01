#ifndef FREEKICKSTRATEGY_H
#define FREEKICKSTRATEGY_H
#include "strategy/strategy.h"
#include "utilities/point.h"
class Robot;

/*! @brief Strategy to implement a free kick game state 'F' and 'f'
 *  @author Narges G */

class FreeKickStrategy: public Strategy
{
public:
    FreeKickStrategy();
    void assignBeh();
    char getNextStrategy() override;

private:
    Robot* kicker = nullptr;  //!<Robot that is sent to kick the ball
    Point initial_bp;
};

#endif // FREEKICKSTRATEGY_H
