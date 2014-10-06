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
     * Function (optional) that updates this object
     * This function is called when StrategyController is ran with
     * a non-new command from GameModel. Return true to erase the 
     * current Strategy in StrategyController, false to keep on this 
     * strategy
     */
    virtual bool update();

    /* Destructor
     * Implement a distructor for a "ending routine" when this object
     * is deleted due to an update in StrategyController
     */
    virtual ~Strategy();
};

#endif // STRATEGY_H
