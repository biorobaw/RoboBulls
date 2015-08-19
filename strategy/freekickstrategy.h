#ifndef FREEKICKSTRATEGY_H
#define FREEKICKSTRATEGY_H
#include "strategy/strategy.h"
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
    Robot* kickerRobot;  //Robot that is sent to kick the ball
};

#endif // FREEKICKSTRATEGY_H
