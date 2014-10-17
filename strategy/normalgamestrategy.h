#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H

#include "strategy/strategy.h"

/* NormalGameStrategy
 * This strategy is meant for the ' ' gamestate (space) which means
 * normal play. The key difference is that this strategy switches between
 * both attack and defend behaviors, and assigns them accordingly.
 * This strategy also exists to provide an improvement to TwoVOne and a
 * not-as-complex alterntiive to AttackStrategy, both of which are broken.
 *
 * Attack/Defend switch criteria:
 *  None.
 * Attacking:
 *  The closest robot to the ball is assign to kick to the goal
 * Defending:
 *  None.
 */

class NormalGameStrategy : public Strategy
{
public:
    void assignBeh() override;
    bool update() override;

private:
    static bool isOnAttack;
};

#endif // NORMALGAMESTRATEGY_H
