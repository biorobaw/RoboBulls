#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H


class Behavior;
class Strategy;
class Team;

/*! @file
 * Controls selection of an active strategy (play) based on the game state
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
    StrategyController(Team*);

    /*! Performs one iteration of the RoboCup Game.
     * Called by GameModel, which is called by VisionComm.  */
    void run();

    Team* getTeam();
    void setRefboxEnabled(bool _enabled);

    void signalNewCommand();

private:
    /*! Clean-up called at the end of a frame.
     * This performs the behaviors on the robots, then
     * uses RobComm to send the velocities to the field */
    void sendRobotCommands();


    /*! Called when GameModel receives the same command
     * as the last frame. Updates current activeStrategy.  */
    void runActiveStrategy();

    //! Switches the gamestate to assign a current strategy */
    void assignNewStrategy(char gameState);

    /*! Removes the behaviors from each robot, and calls end()
     * on the activeStrategy.  */
    void clearCurrentStrategy();


    
private:
    Strategy*  activeStrategy = nullptr;
    Team* team = nullptr;
    bool refbox_enabled = false;
    bool received_new_command = false;
};

#endif // STRATEGYCONTROLLER_H
