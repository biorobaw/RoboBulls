#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

#include <QObject>


namespace YAML {
    class Node;
}

class TeamStrategy;
class RobotTeam;
class GameState;
enum Referee_Command : int;

#define UNINITIALIZED_STATE -1

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


class TeamStrategyController : public QObject
{
    Q_OBJECT
public:

    static TeamStrategyController* loadController(RobotTeam* team, YAML::Node*);
    virtual ~TeamStrategyController();

    QString getStrategyName();

    void runControlCycle(GameState* game_state);

public slots:
    void setPause(bool pause = true);
    void resume();


protected:
    TeamStrategyController(RobotTeam*, YAML::Node*);
    virtual int getControllerState(Referee_Command command) = 0;
    virtual int getNextControllerState(int current_state,int strategy_status) = 0;
    virtual TeamStrategy* loadStateStrategy(int state) = 0;

    RobotTeam* team = nullptr;
    
private:
    TeamStrategy*  activeStrategy = nullptr;
    int controller_state = UNINITIALIZED_STATE;
    bool is_paused = true;

};

#endif // STRATEGYCONTROLLER_H
