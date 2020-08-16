#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include <string>
using std::string;
class Robot;
class RobotTeam;
class GameState;

/*! @addtogroup baseclasses Base Classes
 * Base classes for inherited functionality
 * @{ */

/*! Abstract Behavior class which all other behaviors inherit.
 * @author Orign. Narges Ghaedi; JamesW
 * @ingroup Base Classes
 *
 * This is the base class for all behaviors in the RoboBulls project.
 * It contains a virtual perform(Robot*) function, which is called every
 * loop that a robot has that behavior. The idea is to override this in
 * derived behaviors to perform actions. Also included is a virtual isFinished()
 * function. Child behaviors should override this and return true when the
 * behavior is “finished". It may be sometimes necessary to add other
 * queries such as hasKicked() or readyToAssist() to aid Strategy design.
 *
 * <b>Creating a Behavior</b><br>
 * First, in a file, #include “src/behavior/behavior.h” and override it. A behavior is defined
 * mostly by its perform(Robot*) ​function; the perform function of a ​
 * Behavior ​ typically is a *switch statement over some state variable* to perform a
 * number of skills. This perform function is called each time the game runs
 * (about 55 times per second) so it is to instantaneously update any skills or
 * movement objects. Users should do any expensive initial calculations in the
 * Behavior’s constructor. After a behavior is created, it can be assigned to
 * any number of robots with robot->assignBeh, typically in a Strategy.
 */

class Behavior
{
public:
    Behavior(Robot*);
    virtual ~Behavior();
    
    /*! Perform this behavior's action
     * The <i>perform</i> function
     * of a Behavior defines how to
     * run the behavior on its robot. */
    virtual bool perform() = 0;
    
    /*! Return if behavior is "finished".
     * This could be reaching a target, or whenever a derived behavior
     * is considered finished. Can be used in Stratigies to effectively
     * assign behaviors one after another.
     * @return True if the behavior is considered finished, false otherwise. */
    virtual bool isFinished();


    virtual string getName() = 0;

protected:
    Robot* robot;
    RobotTeam* team;
    GameState* game_state;
};

//! @}

#endif
