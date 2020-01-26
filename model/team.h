#ifndef TEAM_H
#define TEAM_H

#include "yaml-cpp/yaml.h"
#include <set>
#include <string>
#include "model/robot.h"
#include "strategy/strategycontroller.h"

#define MAX_ROBOTS 16 // limited by vision that recognizes only 16 patterns per team

//! Tags for possible team colors
#define TEAM_BLUE 0
#define TEAM_YELLOW 1


class Team {

public:

    Team(YAML::Node t_node, int _color);
    ~Team();

    static void load_teams(YAML::Node team_nodes);
    static Team* getTeam(int id);


    Robot* addRobot(int id);
    void removeRobot(int id);

    Robot* getRobot(int id);
    Robot* getRobotByRole(RobotRole role);
    std::set<Robot*>& getRobots();



    int getColor();
    std::string getRobotType();
    int getSide();
    bool isControlled();


    StrategyController controller = StrategyController(this);


private:

    int color;
    std::string robot_type = "";
    int side;

    std::set<Robot*> all_robots;
    Robot* robotByRoles[MAX_ROLES] = { NULL }; // maps roles to robots
    Robot* robotById[MAX_ROBOTS] = { NULL };
    RobotRole idToRole[MAX_ROBOTS] = {RobotRole::NONE}; // defined by user

    static Team* teams[2];


};


#endif // TEAM_H

