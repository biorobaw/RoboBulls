#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

class GameModel;
class Behavior;
class Strategy;

class StrategyController
{
public:
    StrategyController();
    void setGameModel(GameModel *);
    
    /* Performs one iteration of the RoboCup Game.
     * Called by GameModel, which is called by VisionComm. 
     */
    void run();

private:
    /* Called at the beginning of a frame of the game.
     * Prepares any functions or states that are needed
     * for the frame before strategies are updated
     */
    void frameBegin();

    /* Called at the end of a frame.
     * This performs the behaviors on the robots, then
     * uses RobComm to send the velocities to the field
     */
    void frameEnd();

    /* Called when GameModel receives a new command from
     * the RefComm. Makes a new activeStrategy.
     * Calls end() on the activeStrategy 
     */
    void gameModelUpdated();

    /* Called when GameModel receives the same command
     * as the last frame. Updates current activeStrategy. 
     */
    void gameModelContinued();

    /* Switches the gamestate to assign a current strategy */
    void assignNewStrategy(char gameState);

    /* Removes the behaviors from each robot, and calls end()
     * on the activeStrategy. 
     */
    void clearCurrentStrategy();
    
private:
    Strategy*  activeStrategy;
    GameModel* model;
};

#endif // STRATEGYCONTROLLER_H
