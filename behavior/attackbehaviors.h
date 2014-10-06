#ifndef ATTACK_BEHAVIORS_H
#define ATTACK_BEHAVIORS_H

#include <deque>
#include "behavior/behavior.h"
#include "behavior/genericmovementbehavior.h"
#include "model/robot.h"
#include "skill/skill.h"
#include "utilities/point.h"
#include "utilities/region.h"

/*** Attack Behaviors ***/
/* Defines Four behaviors for use in the Attack Strategy #1
 * AttackDriver
 * AttackReceiver
 * AttackBlocker
 * AttackDefendGoal
 */

/************************************************
 * AttackDriver Behavior
 * This behavior judges the initial distance to the ball and determines
 * whether to drive the ball and eventually pass to the robot with AttackReceiver,
 * or just kick the ball towards the goal. It interacts with the blockers
 * such that if the robot with this behavior needs to kick, isAboutToKick() returns
 * true, and AttackStrategy ensures each blocker moves away using moveAwayFromDriver()
 */
class AttackDriver : public Behavior
{
public:
    AttackDriver(const ParameterList& list);
   ~AttackDriver();
    void perform(Robot* robot) override;
    bool isAboutToKick(Robot* robot);
    bool hasDoneKick();
    
private:
    enum {Initial, Driving, KTPing, Passing} state;
    Skill::Skill* current_skill;    //KickToPoint or DiveBall
    Point  driveStartPoint;
    Point  driveEndPoint;           //Only used when driving is needed
    Robot* recieveAttackBot;        //Only used when passing is needed
    bool   hasReachedBall;
    bool   hasKicked;
};


/************************************************
 * AttackReceiver Behavior
 * This behavior looks for the most "open" spot on the enemy side of the field,
 * but only moves if the attacker is not about to need to kick. If so, it stays
 * put and waits for the AttackDriver robot to pass the ball. Otherwise,
 * moves to an open spot.
 */
class AttackReceiver : public GenericMovementBehavior
{
public:
    AttackReceiver(const ParameterList& list);
    void perform(Robot* robot) override;
    void setMoveStatus(bool canMove);

private:
    Region* findMostStrategicRegion(Robot* robot);
    void    findStrategicValue(Region& r, Robot* receiver);
    Robot*  driveAttackBot;
    bool    isMoveOk;
};


/************************************************
 * AttackBlocker Behavior
 * This behavior gathers around the attack robot such that a maximum of three
 * robots are infront of it, helping prevent enemy robots from taking the ball.
 * When AttackDriver robot needs to kick, the AttackBlocker robots increase
 * their radius from the robot. Of course, this doesn't mean the ball won't hit
 * the blockers, but it helps.
 */
class AttackBlocker : public GenericMovementBehavior
{
public:
    AttackBlocker(const ParameterList& list);
   ~AttackBlocker();
    void perform(Robot *robot) override;
    void moveAwayFromDriver();

private:
    static int numAttackBlockers;
    static int nextSide;
    int myBlockerNum;
    int mySide;
    int isWaitingCount;
    Robot* theDriverRobot;
    float  curDistance;
};



/************************************************
 * AttackDefend Behavior
 * Although a paradoxical name, this behavior is meant to provide defence on our side
 * of the field while the other robots are attacking. The purpose is such that when
 * we are attacking, we still want a defence when the enemy team steals the ball
 * and brings it over.
 * So, what this behavior does is set up a number of robots to gather in a formation
 * around the goal, and sit in the direction of the ball.
 */
class AttackDefendGoal : public GenericMovementBehavior
{
public:
    AttackDefendGoal(const ParameterList& list);
   ~AttackDefendGoal();
    void perform(Robot* robot) override;
    
private:
    static int numGoalDefenders;
    static int nextSide;
    int myGoalDefenderSpot;
    int mySide;
};

#endif
