#ifndef STOPSTRATEGY_H
#define STOPSTRATEGY_H

#include <unordered_map>
#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behavior.h"


class StopStrategy:public Strategy
{
public:
    StopStrategy();
    void assignBeh() override;
    bool update() override;

private:
    /* In case of ball movement or some other effect, the target points
     * for StopBehavior need to be rebuilt.
     */
    void rebuildTargetPoints();


    /* Contains all the points that each robot should be going to
     * during StopBehavior. Mapped by ID.
     */
    static std::unordered_map<int, Point> robTargetPoints;


    /* Need to keep track of the previous ball point, and rebuild target
     * points on a significant change
     */
    static Point prevBallPoint;
};

#endif // STOPSTRATEGY_H
