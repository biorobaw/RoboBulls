#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H

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
 *
 * This strategy was designed for the Nov.26 presentation..
 * then Feb 13-14 Engineering Expo...
 * then Summer '15 revival...
 */

class NormalGameStrategy : public Strategy
{
public:
     NormalGameStrategy();
    ~NormalGameStrategy();
    void assignBeh() override;
    bool update() override;

public:
    /* Static function: Moves robots to an idle line,
     * a line near the goal and spaced by ID. */
    static void moveRobotToIdleLine(Robot* robot, bool waiter = false);

    /* Static functon: Assigns DefendFarFromBall to ID 5
     * if it is on the team */
    static void assignGoalieIfOk();

private:
    bool considerSwitchCreiteria();
    void assignAttackBehaviors(bool switchSides = false);
    void assignDefendBehaviors();
    void assignGoalKickBehaviors();
    void assignRetreatBehaviors();

private:
    static bool isOnAttack;
    static bool hasStoppedForThisKickoff;
    Robot* currentMainAttacker;
    Robot* currentSuppAttacker;
    void findMostValidRobots(Point, Robot*&, Robot*&, Robot *&c_out);
    Point ballOriginalPos;
};

#endif // NORMALGAMESTRATEGY_H
