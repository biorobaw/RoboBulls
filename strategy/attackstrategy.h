#ifndef ATTACKSTRATEGY_H
#define ATTACKSTRATEGY_H

#include <vector>
#include "behavior/attackbehaviors.h"
#include "strategy/strategy.h"
#include "utilities/point.h"

class AttackStrategy : public Strategy
{
public:
   ~AttackStrategy();

    /* Called first frame when the game state switches to
     * "1" or "2" or "o" or "O" and an attack configuration
     * has been selected.
     */
    void assignBeh() override;

    /* When the game state has not switched and the strategy has
     * already been started, updates.
     */
    bool update() override;
    
private:
    static Robot* lastReciever;
    static Robot* lastDriver;
    std::pair<Robot*, Robot*> passingRobots;
    std::vector<Robot*> blockingRobots;
    std::vector<Robot*> defendingRobots;
};

#endif // ATTACKSTRATEGY_H
