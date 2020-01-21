#ifndef TEAM_H
#define TEAM_H

#include "yaml-cpp/yaml.h"
#include <set>
#include <string>
#include "model/robot.h"

#define MAX_ROBOTS 16 // limited by vision that recognizes only 16 patterns per team


class Team {

public:

    Team();
    Team(YAML::Node config);

    bool color;
    std::string robot_type = "";
    int side;


    Robot* getRobot(int id);
    Robot* getRobotByRole(RobotRole role);
    std::set<Robot*>& getRobots();

    Robot* addRobot(int id);
    void removeRobot(int id);

private:
    std::set<Robot*> all_robots;
    Robot* robotByRoles[MAX_ROLES] = { NULL }; // maps roles to robots
    Robot* robotById[MAX_ROBOTS] = { NULL };
    RobotRole idToRole[MAX_ROBOTS] = {RobotRole::NONE}; // defined by user


};


#endif // TEAM_H

