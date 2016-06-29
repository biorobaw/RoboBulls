#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H
class GameModel;
class Behavior;
class Strategy;

#include <sys/time.h>

/*! @brief Controls selection of an active strategy (play) based on the game state
 *
 * The StrategyController can be seen as the main driver of the
 * robobulls2 project. It does a switch statement over the current
 * gamestate from the RefBox, and chooses a current active strategy.
 * It also defines frameBegin() and frameEnd() functions, which are
 * called at the start and end of each frame, respectively. Viewers
 * can see that the frameEnd function calls the perform function of
 * each robot’s current Behavior, and is the routine that actually
 * uses the RobComm to sent the velocities to the robots for a frame. */

class StrategyController
{
public:
    StrategyController(GameModel*);

    /*! @brief Performs one iteration of the RoboCup Game.
     * Called by GameModel, which is called by VisionComm.  */
    void run();

private:
    /*! @brief Initialization called at the beginning of a frame of the game.
     * @details Prepares any functions or states that are needed
     * for the frame before strategies are updated */
    void frameBegin();

    /*! @brief Clean-up called at the end of a frame.
     * @details This performs the behaviors on the robots, then
     * uses RobComm to send the velocities to the field */
    void frameEnd();

    /*! @brief Called when GameModel receives a new command from
     * the RefComm.Makes a new activeStrategy. */
    void gameModelUpdated();

    /*! @brief Called when GameModel receives the same command
     * as the last frame. Updates current activeStrategy.  */
    void gameModelContinued();

    //! @brief Switches the gamestate to assign a current strategy */
    void assignNewStrategy(char gameState);

    /*! @brief Removes the behaviors from each robot, and calls end()
     * on the activeStrategy.  */
    void clearCurrentStrategy();
    
private:
    Strategy*  activeStrategy;
    GameModel* model;
    timeval lastSendTime;
};

#endif // STRATEGYCONTROLLER_H
