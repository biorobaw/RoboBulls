#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy
{
public:
    /* assignBeh
     * Function that decides how to assign behaviors to each robot
     * on the team. This function is called when StrategyController
     * is ran with a new command recieved from GameModel
     */
    virtual void assignBeh() = 0;

    /* update
     * Function (optional) that updates this object.
     * This function is called when StrategyController is ran with
     * a non-new command from GameModel. Return true to notify the strategy
     * is finished and have a new one assigned, false otherwise.
     */
    virtual bool update();

    /* getNextStrategy
     * Function used to control strategy flow.
     * Return an ASCII character corrisponding to a game state to switch
     * the StrategyController to. return '\0' or the current gate state
     * (via GameModel) to keep the current state.
     */
    virtual char getNextStrategy();

    /* Destructor
     * Implement a distructor for a "ending routine" when this object
     * is deleted due to an update in StrategyController
     */
    virtual ~Strategy();
};

#endif // STRATEGY_H
