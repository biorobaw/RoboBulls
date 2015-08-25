#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H
#include "strategy/strategy.h"
#include "utilities/point.h"

/*! @brief Strategy to implement the game stopped state 'S'
 * @author JamesW
 * @details To implement to stopped game state, we have the robots gather
 * in a circle around the ball. the radius at which they do this should comply
 * with RoboCup rules */

class StopStrategy:public Strategy
{
public:
    /*! @brief Constructor
     * @param radius Chooses the radius to circle around the ball at */
    StopStrategy(float radius = 1000);
    void assignBeh() override;
    bool update() override;

private:
    /*! @brief Rebuilds target points in case of ball movement or added robot
     * @details In case of ball movement or some other effect, the target points
     * for StopBehavior need to be rebuilt. */
    void rebuildTargetPoints();

    /*! @brief Keeping track of the previous ball point.
     * @details Need to keep track of the previous ball point, and rebuild target
     * points on a significant change. */
    Point prevBallPoint;

    /*! @brief Stored points of the target each Robot should go to.
     * @details Built from rebuildTargetPoints */
    Point robTargetPoints[10];

    //! @brief Radius to stay from the ball point
    float radius;
};

#endif // STOPSTRATEGY_H
