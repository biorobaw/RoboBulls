#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H
#include "strategy/strategy.h"
#include "utilities/point.h"
#include "utilities/edges.h"

/*! @brief Strategy to implement the game stopped state 'S'
 * @author JamesW
 * @details To implement to stopped game state, we have all robots
 * move to a position that is at least 500mm from the ball */

class StopStrategy:public Strategy
{
public:
    /*! @brief Constructor */
    StopStrategy();
    void assignBeh() override;
    bool update() override;

private:
    Point prev_point = Point(-9999, 9999);
    bool need_assign[10] = {true};
};

#endif // STOPSTRATEGY_H
