#ifndef DEFENDBEHAVIOR_H
#define DEFENDBEHAVIOR_H

#include <array>
#include "utilities/point.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/behavior.h"
#include "skill/kicktopointomni.h"

/* DEFEND BAHVIOR
 * DefendBehavior is the next iteration of a defense mode (circa May 2015).
 * It sets robots to sit at points around the goal, and get in the line of motion
 * of the ball and kick it back if it is coming torwards the goal.
 * This also serves as a first iteration of an "intelligent agents" type of behavior,
 * where each robot independently runs the same behavior to without the need for a
 * strategy. This is made up of `DefendStates` which link to one another.
 */

class DefendState
{
public:
    DefendState();
    virtual ~DefendState();

    /* Perform a state and return a new one
     * Returns `this` if no change is made
     */
    virtual DefendState* action(Robot* robot) = 0;
};

/************************************************************/
/* DefendState To choose a point to idle at and
 * go to kick the ball away if it is coming close
 */
class DefendStateIdle : public DefendState, public GenericMovementBehavior
{
public:
     DefendStateIdle();
     DefendState* action(Robot* robot) override;
private:
    bool   robotsCloseToPoint(Robot*, const Point&);
    Point  chosenPoint;
    bool   hasChosenPoint;
    static std::array<Point, 10> claimed;
};

/************************************************************/
/* DefendState to kick the ball away and return to idle
 * if finished
 */
class DefendStateKick : public DefendState, public GenericMovementBehavior
{
public:
    DefendStateKick();
   ~DefendStateKick();
    DefendState* action(Robot* robot) override;
private:
    Skill::KickToPointOmni* ktpo;
    bool  chosenLinePoint;
    bool  kickingBall;
    int   kickBallTimeout;
    Point linePoint;
    bool  ballMovingAway();
    static int whoIsKicking;
};

/************************************************************/


class DefendBehavior : public Behavior
{
public:
     DefendBehavior();
    ~DefendBehavior();
    void perform(Robot *) override;
private:
    DefendState* state;
};

#endif // DEFENDBEHAVIOR_H
