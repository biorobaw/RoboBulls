#ifndef TEAM_H
#define TEAM_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMap>
#include "constants.h"
#include "robot/robot.h"

class RobotProxy;
class StrategyController;
class GameState;
namespace YAML {
    class Node;
}


class RobotTeam : public QObject
{
    Q_OBJECT
public:

     // ======= STATIC FUNCTIONS ===================================

    static RobotTeam** load_teams(YAML::Node* team_nodes);


     // ======= Constructor and basic info ========================

    RobotTeam(YAML::Node* t_node, int _color);
    ~RobotTeam();
    int getID();

    // ======= Robot Getter functions ==============================

    Robot* getRobot(int robot_id);
    Robot* getRobotByRole(int role);
    QSet<Robot*>& getRobots();

    Robot* getOpponent(int robot_id);
    QSet<Robot*>& getOpponents();

    // ======= Control Related function ===========================

    void startControlLoop();
    GameState* getGameState();

private slots:
    void runControlCycle();

    // signals for gui
signals:
    void strategyChanged(int team_id, QString new_strategy);

private:

    static RobotTeam* teams[2];

    QThread* thread;
    QTimer* timer;

    StrategyController* controller = nullptr; // controller to control the team
    RobotProxy*         robot_proxy; // proxy to communicate with robots and create new pilots for them

    GameState*          game_state; // game state as seen by the team (each team has its own view)
    QMap<int,Robot*> robotsByRoles; // maps robots to roles, defined by the team controller

    int team_id;
    int opponent_id;
    int side;

    // Friend functions
    friend RobotTeam* Robot::getTeam();
};


#endif // TEAM_H

