#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H

#include "model/robot.h"
#include "strategy/strategy.h"

/*! @brief This strategy is meant for the ' ' gamestate (space) which means
 * normal play.
 * @author JamesW
 *
 * This strategy switches between both attack and defend behaviors.
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
    //! Moves robots to an idle line, a line near the goal and spaced by ID.
    static void moveRobotToIdleLine(Robot* robot, bool waiter = false);

    //! Assigns DefendFarFromBall to ID GOALIE_ID if it is on the team
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
    bool  needsAttackAssign;
    bool  needsDefenceAssign;
};

#endif // NORMALGAMESTRATEGY_H
