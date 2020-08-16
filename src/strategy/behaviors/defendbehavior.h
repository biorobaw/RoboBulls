#ifndef DEFENDBEHAVIOR_H
#define DEFENDBEHAVIOR_H
#include "genericmovementbehavior.h"
#include "../skills/kicktopointomni.h"


// ================ OBSERVATION =====================================
//
// THIS CLASS SEEMS DEPRECATED, IT'S NOT USED IN THE REST OF THE CODE
// THE CLASS HAS ISSUES THAT NEED SOLVING BEFORE IT CAN BE USED
// In particular it assumes only one team, and assumes exactly 5 robots
//
// ===================================================================



class DefendState;

/*! @brief DefendBehavior is the next iteration of a defense mode (circa May 2015).
 * @author JamesW
 * @details DefendBehavior sets robots to sit at points around the goal, and get in the
 * line of motion of the ball and kick it back if it is coming torwards the goal. In addition,
 * if the ball is stopped on our side a robot will break to kick it, and also
 * the robots sway formation to face the ball if it on our side.
 * This also serves as a first iteration of an "intelligent agents" type of behavior,
 * where each robot independently runs the same behavior to without the need for a
 * strategy. This is made up of `DefendStates` which link to one another. */

class DefendBehavior : public Behavior
{
public:
     DefendBehavior(Robot*);
    ~DefendBehavior();
    bool perform() override;
    string getName() override;
private:
    static int currentUsers;    //Number of robots currently using this behavior
    DefendState* state;         //Current state (one of the above)
};



/*! @file
    A base class, a state in the DefendBehavior state machine.
    States are changed by returning a new
    instance of one from `action`. Similar to the `perform` function
*/
class DefendState : virtual public Behavior
{
public:
    DefendState();
    virtual ~DefendState();

    //! @brief Perform a state and return a new one Returns `this` if no transition is made */
    virtual DefendState* action();







    //! @brief Initializes all claimed points to initial positions
    static void setupClaimedPoints();

    //! @brief Clears all robot's claimed points.
    //! @details by setting `defendPoints` back to the default
    //! positions and filling `claimed` with -1
    static void clearClaimedPoints();

protected:
    //Returns the point pointer this robot should go to, or nullptr if the
    //robot has none. Use searchClaimPoint to claim a point first.
    static Point* getClaimedPoint(int robot_id);

    //Loops through, looks, and sets the point internally for this robot to claim
    static Point* searchClaimPoint(Robot*, RobotTeam* team);

    static int   kicker_ID;     //Who is moving to kick the ball?

private:
    static int   claimed[10];      //Index of Points in `defendPoints` to idle at
    static Point defendPoints[];   //Points to sit robots at.
    static const Point defaultPoints[];//Default points to sit at
    static int   updateCount;      //Count to delay `action` updating of points

};

//! @cond

/************************************************************/

/*! @brief DefendState To choose a point to idle at.
 * @details If activeKick is true, goes to kick the ball away if it is coming close.
 */
class DSIdle : public DefendState, public GenericMovementBehavior
{
public:
     DSIdle(Robot* robot);
     DefendState* action() override;
     string getName() override;
private:
    bool activeKicking;
};


/************************************************************/

/*! @brief DefendState that merely does KickToPointOmni to kick the ball away.
 * @dtails Happens when the ball stops close to a robot on our side
 */
class DSKick: public DefendState, public GenericMovementBehavior
{
public:
    DSKick(Robot* robot,Point opponent_goal);
   ~DSKick();
    DefendState* action() override;
    string getName() override;
private:
    KickToPointOmni* ktpo;
};


/************************************************************/

/*! @brief DefendState to sit in the ball's incoming path.
 * @details Then, kick the ball away and return to idle when finished */
class DSIntercept : public DefendState, public GenericMovementBehavior
{
public:
    DSIntercept(Robot* robot);
   ~DSIntercept();
    DefendState* action() override;
    string getName() override;
private:
    KickToPointOmni* ktpo;
    bool  chosenLinePoint;
    bool  kickingBall;
    int   kickBallTimeout;
    Point linePoint;
    bool  tryGetValidLinePoint();
};

//! @endcond

/************************************************************/



#endif // DEFENDBEHAVIOR_H
