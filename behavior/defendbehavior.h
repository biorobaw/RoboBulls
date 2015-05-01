#ifndef DEFENDBEHAVIOR_H
#define DEFENDBEHAVIOR_H

#include "utilities/point.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/behavior.h"
#include "skill/kicktopointomni.h"

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
    bool  robotsCloseToPoint(Robot*, const Point&);
    Point chosenPoint;
    bool  hasChosenPoint;
    int   ballComingCounter;
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
