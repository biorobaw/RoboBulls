#include "model/team.h"
#include <algorithm>
#include "include/game_constants.h"
#include <assert.h>



Team::Team(){

}

Team::Team(YAML::Node config){


    std::cout << "--TEAM" << std::endl;
    std::cout << "        COLOR : " << config["COLOR"] <<std::endl;
    std::cout << "        SIDE  : " << config["SIDE"] <<std::endl;
    std::cout << "        ROLES : " << std::endl;
    std::cout << "            GOALIE  : " << config["ROLES"]["GOALIE" ] << std::endl;
    std::cout << "            ATTACK1 : " << config["ROLES"]["ATTACK1"] << std::endl;
    std::cout << "            ATTACK2 : " << config["ROLES"]["ATTACK2"] << std::endl;
    std::cout << "            ATTACK3 : " << config["ROLES"]["ATTACK3"] << std::endl;
    std::cout << "            DEFEND1 : " << config["ROLES"]["DEFEND1"] << std::endl;
    std::cout << "            DEFEND2 : " << config["ROLES"]["DEFEND2"] << std::endl;
    std::cout << "            DEFEND3 : " << config["ROLES"]["DEFEND3"] << std::endl;


    color = config["COLOR"].as<int>();
    assert(color == TEAM_BLUE || color == TEAM_YELLOW);

    side =  config["SIDE"].as<int>();
    assert(side == SIDE_NEGATIVE || side == SIDE_POSITIVE);

    robot_type = config["ROBOT_TYPE"].as<std::string>();
    assert( robot_type == "grsim" ||
            robot_type == "yisibot" ||
            robot_type == "pi32019");


    idToRole[config["ROLES"]["GOALIE" ].as<int>()] = RobotRole::GOALIE;
    idToRole[config["ROLES"]["ATTACK1"].as<int>()] = RobotRole::ATTACK1;
    idToRole[config["ROLES"]["ATTACK2"].as<int>()] = RobotRole::ATTACK2;
    idToRole[config["ROLES"]["ATTACK3"].as<int>()] = RobotRole::ATTACK3;
    idToRole[config["ROLES"]["DEFEND1"].as<int>()] = RobotRole::DEFEND1;
    idToRole[config["ROLES"]["DEFEND2"].as<int>()] = RobotRole::DEFEND2;
    idToRole[config["ROLES"]["DEFEND3"].as<int>()] = RobotRole::DEFEND3;
\
    std::cout << "--Team DONE" <<std::endl;

}

/*! @brief Look for a robot in the team with id `id`
 * @param id The id of the robot to look for
 * @see Robot class
 * \return A robot pointer if found, or NULL if not on the team */
Robot* Team::getRobot(int id)
{
    return robotById[id];
}

Robot* Team::getRobotByRole(RobotRole role){
    return robotByRoles[role];
}

std::set<Robot*>& Team::getRobots(){
    return all_robots;
}

Robot* Team::addRobot(int id){
    auto* aux = new Robot(id, color, robot_type, idToRole[id]);
    robotById[id] = aux;
    robotByRoles[idToRole[id]] = aux;
    all_robots.insert(aux);
    return aux;
}

void Team::removeRobot(int id){
    Robot* aux = robotById[id];
    robotById[id] = NULL;
    robotByRoles[aux->getRole()] = NULL;
    all_robots.erase(aux);
    delete aux;

}




