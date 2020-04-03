#ifndef TEAM_H
#define TEAM_H

#include "yaml-cpp/yaml.h"
#include <set>
#include <string>
#include "robot/robot.h"
#include "robot/robcomm.h"
#include "strategy/strategycontroller.h"
#include "constants.h"
 // limited by vision that recognizes only 16 patterns per team



class RobotTeam {

public:

    RobotTeam(YAML::Node t_node, int _color);
    ~RobotTeam();

    static void load_teams(YAML::Node team_nodes);
    static RobotTeam* getTeam(int id);


    Robot* addRobot(int id);
    void removeRobot(int id);

    Robot* getRobot(int id);
    Robot* getRobotByRole(RobotRole role);
    std::set<Robot*>& getRobots();




    int getColor();
    std::string getRobotType();
    int getSide();
    int getOpponentSide();
    bool isControlled();


    StrategyController* controller = nullptr;
    void closeCommunication();
    void sendVels();


private:

    int color;
    std::string robot_type = "";
    int side;

    std::set<Robot*> all_robots;
    Robot* robotByRoles[MAX_ROLES] = { NULL }; // maps roles to robots
    Robot* robotById[MAX_ROBOTS_PER_TEAM] = { NULL };
    RobotRole idToRole[MAX_ROBOTS_PER_TEAM] = {RobotRole::NONE}; // defined by user



    static RobotTeam* teams[2];
    RobComm* comm;


};


#endif // TEAM_H

