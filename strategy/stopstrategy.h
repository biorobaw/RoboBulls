#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H

#include "strategy/strategy.h"
#include "model/gamemodel.h"

/*! @brief Strategy to implement the game stopped state 'S'
 * @author JamesW
 * @details To implement to stopped game state, we have the robots gather
 * in a circle around the ball */

class StopStrategy:public Strategy
{
public:
    void assignBeh() override;
    bool update() override;

private:
    /* In case of ball movement or some other effect, the target points
     * for StopBehavior need to be rebuilt. */
    void rebuildTargetPoints();

    /* Need to keep track of the previous ball point, and rebuild target
     * points on a significant change */
    static Point prevBallPoint;
};

#endif // STOPSTRATEGY_H
