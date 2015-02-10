#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H

#include <utility>
#include "model/robot.h"
#include "strategy/strategy.h"

/* NormalGameStrategy
 * This strategy is meant for the ' ' gamestate (space) which means
 * normal play. The key difference is that this strategy switches between
 * both attack and defend behaviors, and assigns them accordingly.
 * This strategy also exists to provide an improvement to TwoVOne and a
 * not-as-complex alternative to AttackStrategy; both of which are broken.
 *
 * **This is a Normal Game strategy which assumes a 3-robot team, which
 *   robot number 5 is the goalie, and two other robots exist.
 *
 * Attack/Defend switch criteria: (considerSwitchCreiteria)
 *  - If the enemy/our team has been seen with the ball NORMAL_SWITCH_COUNT
 *    times we switch from attack to defend or vice versa.
 * Attacking (assignAttackBehaviors):
 *  - The closest robot to the ball is assigned to kick to the goal.
 *  - Robot #5 is assigned to go to the goal and defend the goal.
 *  - The remaining robot is assigned to prowl the enemy field and assume
 *    a stretegic position to recieve a pass.
 * Defending (assignDefendBehaviors):
 *  - One robot is sent to the center to encourage a quick Attack switch
 *  - Robot #5 still defend the goal
 *  - The remaining robot is sent to block the enemy passer/receiver team
 *    by placing itself between them (Measurments::midPoint).
 */

class NormalGameStrategy : public Strategy
{
public:
    NormalGameStrategy();
    void assignBeh() override;
    bool update() override;

private:
    bool considerSwitchCreiteria();
    void assignAttackBehaviors();
    void assignDefendBehaviors();
    void assignGoalKickBehaviors();
    void assignRetreatBehaviors();

private:
    static bool isOnAttack;
    Robot* currentMainAttacker = NULL;
    Robot* currentSuppAttacker = NULL;
    void findMostValidRobots(Point, Robot*&, Robot*&, Robot *&c_out);
    Point ballOriginalPos;
};

#endif // NORMALGAMESTRATEGY_H
