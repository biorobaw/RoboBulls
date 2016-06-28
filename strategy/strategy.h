#ifndef STRATEGY_H
#define STRATEGY_H

//! @addtogroup baseclasses
//! @{

/*! @brief Strategy is the abstract base class for other strategy classes.
 * @ingroup Base Classes
 * @details All the strategy classes inherit from the Strategy class
 * and override and assignBeh() (assign behavior) function
 * that assigns behaviors to robots. In addition, derived classes
 * should/can override update() which updates the strategy after
 * behaviors have been assigned. An example use of the update
 * function is to keep track of information regarding the robots
 * assigned behaviors in assignBeh, but not actually assign anybody a
 * behavior unless a new condition is met. This allows for more dynamic
 * and complex strategies.
 *
 * <b>Creating a Strategy</b><br>
 * Similar to a behavior, users should include “strategy/strategy.h”
 * and override from the base class.
 * There are two functions that define a strategy--­­​assignBeh ​and ​update​.
 * Having at least ​assignBeh ​defined is required for a strategy. ​
 * assignBeh ​is a function that should use the ​GameModel ​and other
 * tools to figure out ​how ​to assign behaviors to robots. Next, to
 * use this strategy, it must be placed somewhere as a choosable option
 * in StrategyController.cpp ​. in ​StrategyController’s ​gameModelUpdated()
 * function,  users will see a “testing macro macro”­­ this is used by us
 * to ignore the RefBox’s commands to test only one strategy. Here, you
 * can add your own strategy in. */

class Strategy
{
public:
    /*! @brief Decides How to assign behaviors to each robot on the team.
     * @details This function is called when StrategyController is run with a
     * new command recieved from GameModel */
    virtual void assignBeh() = 0;

    /*! @brief Function (optional) that updates the Strategy object.
     * @details This function is called when StrategyController is run with
     * a non-new command from GameModel. Return true to notify the strategy
     * is finished and have a new one assigned, false otherwise. */
    virtual bool update();

    /*! @brief Function used to control strategy flow.
     * @details Allows strategies to be switched within a single game state.
     * Return an ASCII character corresponding to a game state to switch
     * the StrategyController to. return '\0' or the current gate state
     * (via GameModel) to keep the current state. */
    virtual char getNextStrategy();

    /*! @brief Destructor
     * @details Implement a distructor for a "ending routine" when this object
     * is deleted due to an update in StrategyController */
    virtual ~Strategy();
};

//! @}

#endif // STRATEGY_H
