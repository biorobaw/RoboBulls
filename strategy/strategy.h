#ifndef STRATEGY_H
#define STRATEGY_H

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
 * and complex strategies. Strategy also has a function called getNextStrategy()
 * (See indirectkickstrategy.cpp for an example).
 * This update allows strategies to be switched within the
 * StrategyController on a single game state. The idea is to have a
 * “pseudo-linked-list” of strategies, where any strategy can request
 * a change to another one at any point in time, by returning its ASCII
 * RefBox command to StrategyController from getNextStrategy. Return the
 * NULL character ‘\0’ to continue the current strategy. */

class Strategy
{
public:
    /*! @briefDecides How to assign behaviors to each robot on the team.
     * This function is called when StrategyController is ran with a
     * new command recieved from GameModel */
    virtual void assignBeh() = 0;

    /*! Function (optional) that updates the Strategy object.
     * This function is called when StrategyController is ran with
     * a non-new command from GameModel. Return true to notify the strategy
     * is finished and have a new one assigned, false otherwise. */
    virtual bool update();

    /*! Function used to control strategy flow.
     * Return an ASCII character corrisponding to a game state to switch
     * the StrategyController to. return '\0' or the current gate state
     * (via GameModel) to keep the current state. */
    virtual char getNextStrategy();

    /*! Destructor
     * Implement a distructor for a "ending routine" when this object
     * is deleted due to an update in StrategyController */
    virtual ~Strategy();
};

#endif // STRATEGY_H
