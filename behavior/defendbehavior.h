#ifndef DEFENDBEHAVIOR_H
#define DEFENDBEHAVIOR_H

#include "behavior/genericmovementbehavior.h"
#include "skill/kicktopointomni.h"

/* DEFEND BEHAVIOR
 * DefendBehavior is the next iteration of a defense mode (circa May 2015).
 * It sets robots to sit at points around the goal, and get in the line of motion
 * of the ball and kick it back if it is coming torwards the goal. In addition,
 * if the ball is stopped on our side a robot will break to kick it, and also
 * the robots sway formation to face the ball if it on our side.
 *
 * This also serves as a first iteration of an "intelligent agents" type of behavior,
 * where each robot independently runs the same behavior to without the need for a
 * strategy. This is made up of `DefendStates` which link to one another.
 */
 
/************************************************************/
/* DefendState -- A base class, represents a state in the 
 * DefendBehavior state machine. States are changed by return a new
 * instance of one from `action`. Similar to the `perform` function
 */
class DefendState
{
public:
    virtual ~DefendState();

    /* Perform a state and return a new one
     * Returns `this` if no transition is made */
    virtual DefendState* action(Robot* robot);

    //Initializes all claimed points to initial positions
    static void setupClaimedPoints();

    //Clears all robot's claimed points, by setting `defendPoints`
    //back to the default positions and filling `claimed` with -1
    static void clearClaimedPoints();

protected:
    //Returns the point pointer this robot should go to, or nullptr if the
    //robot has none. Use searchClaimPoint to claim a point first.
    static Point* getClaimedPoint(Robot*);

    //Loops through, looks, and sets the point internally for this robot to claim
    static Point* searchClaimPoint(Robot*);

protected:
    static int   whoIsKicking;     //Who is moving to kick the ball?

private:
    static int   claimed[10];      //Index of Points in `defendPoints` to idle at
    static Point defendPoints[];   //Points to sit robots at.
    static const Point defPoints[];//Default points to sit at
    static int   updateCount;      //Count to delay `action` updating of points
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
};


/************************************************************/
/* DefendState that merely does KickToPointOmni to kick the ball away. Happens
 * when the ball stops close to a robot on our side
 */
class DefendStateIdleKick: public DefendState
{
public:
    DefendStateIdleKick();
   ~DefendStateIdleKick();
    DefendState* action(Robot* robot) override;
private:
    Skill::KickToPointOmni* ktpo;
};


/************************************************************/
/* DefendState to sit in the ball's incoming path, then
 * kick the ball away and return to idle when finished
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
    bool  tryGetValidLinePoint(Robot*);
};


/************************************************************/

class DefendBehavior : public Behavior
{
public:
     DefendBehavior();
    ~DefendBehavior();
    void perform(Robot *) override;
private:
    static int currentUsers;    //Number of robots currently using this behavior
    DefendState* state;         //Current state (one of the above)
};

#endif // DEFENDBEHAVIOR_H
