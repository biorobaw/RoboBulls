#ifndef TEAM_H
#define TEAM_H

#include <set>
#include <map>
#include <QMap>
#include <string>
#include "constants.h"
#include "robot/robot.h"
//class Robot;
class RobotProxy;
class StrategyController;
class GameState;
namespace YAML {
    class Node;
}


class RobotTeam {

public:

     // ======= STATIC FUNCTIONS ===================================

    static RobotTeam** load_teams(YAML::Node* team_nodes);


     // ======= Constructor and destructors ========================

    RobotTeam(YAML::Node* t_node, int _color);
    ~RobotTeam();

    // ======= Robot Getter functions ==============================

    Robot* getRobot(int id);
    Robot* getRobotByRole(int role);
    QSet<Robot*>& getRobots();

    // ======= Getter and predicate functions ======================

    int getColor();
    int getSide();
    int getOpponentSide();


    // ======= Functions required for the gui ======================

    QString getRobotType();
    std::string getTeamControllerName();
    std::string getStrategyName();


    // ======= Communication functions =============================

    void closeCommunication();
    void sendVels();

    GameState* getGameState();

private:

    static RobotTeam* teams[2];

    StrategyController* controller = nullptr; // controller to control the team
    RobotProxy* robot_proxy; // proxy to communicate with robots and create new pilots for them
    GameState* game_state; // game state as seen by the team (each team has its own view)

    int color;
    int side;

    QString robot_type = "";
    std::string team_controller_name = "none";

    // robot mappings
    QMap<int,Robot*> robotsByRoles; // maps robots to roles, defined by the team controller



    // Friend functions
    friend RobotTeam* Robot::getTeam();

};


#endif // TEAM_H

