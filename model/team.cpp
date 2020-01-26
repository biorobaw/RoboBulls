#include "model/team.h"
#include <algorithm>
#include "parameters/game_constants.h"
#include "communication/robcomm.h"
#include <assert.h>

Team* Team::teams[2] = {NULL};

Team::Team(YAML::Node t_node, int _color) {

    std::cout << "--TEAM_" << (_color == TEAM_BLUE ? "BLUE" : "YELLOW") << std::endl;
    std::cout << "        SIDE        : " << t_node["SIDE"] <<std::endl;
    std::cout << "        ROBOT_TYPE  : " << t_node["ROBOT_TYPE"] <<std::endl;
    if(t_node["ROLES"].IsDefined()){
        std::cout << "        ROLES : " << std::endl;
        std::cout << "            GOALIE  : " << t_node["ROLES"]["GOALIE" ] << std::endl;
        std::cout << "            ATTACK1 : " << t_node["ROLES"]["ATTACK1"] << std::endl;
        std::cout << "            ATTACK2 : " << t_node["ROLES"]["ATTACK2"] << std::endl;
        std::cout << "            ATTACK3 : " << t_node["ROLES"]["ATTACK3"] << std::endl;
        std::cout << "            DEFEND1 : " << t_node["ROLES"]["DEFEND1"] << std::endl;
        std::cout << "            DEFEND2 : " << t_node["ROLES"]["DEFEND2"] << std::endl;
        std::cout << "            DEFEND3 : " << t_node["ROLES"]["DEFEND3"] << std::endl;
    }


    color = _color;

    side =  t_node["SIDE"].as<int>();
    assert(side == SIDE_NEGATIVE || side == SIDE_POSITIVE);

    robot_type = t_node["ROBOT_TYPE"].as<std::string>();

    assert( robot_type == "grsim" ||
            robot_type == "yisibot" ||
            robot_type == "pi32019" ||
            robot_type == "none" );

    if( robot_type != "none"){

        std::cout << "        REFBOX_ENABLED  : " << t_node["REFBOX_ENABLED"] <<std::endl;
        controller.setRefboxEnabled(t_node["REFBOX_ENABLED"].as<bool>());
    }

    if(t_node["ROLES"].IsDefined()){
        idToRole[t_node["ROLES"]["GOALIE" ].as<int>()] = RobotRole::GOALIE;
        idToRole[t_node["ROLES"]["ATTACK1"].as<int>()] = RobotRole::ATTACK1;
        idToRole[t_node["ROLES"]["ATTACK2"].as<int>()] = RobotRole::ATTACK2;
        idToRole[t_node["ROLES"]["ATTACK3"].as<int>()] = RobotRole::ATTACK3;
        idToRole[t_node["ROLES"]["DEFEND1"].as<int>()] = RobotRole::DEFEND1;
        idToRole[t_node["ROLES"]["DEFEND2"].as<int>()] = RobotRole::DEFEND2;
        idToRole[t_node["ROLES"]["DEFEND3"].as<int>()] = RobotRole::DEFEND3;
    }

    teams[color] = this;

    // set robot communication:
    RobComm::open_communication(t_node);


    std::cout << "--Team_" << (_color == TEAM_BLUE ? "blue" : "yellow") << " DONE" <<std::endl;

}

Team::~Team(){
    if(teams[color] == this) teams[color] = NULL;
}


void Team::load_teams(YAML::Node team_nodes){
    teams[TEAM_BLUE] = new Team(team_nodes["TEAM_BLUE"], TEAM_BLUE);
    teams[TEAM_YELLOW] = new Team(team_nodes["TEAM_YELLOW"], TEAM_YELLOW);

    if(teams[0]->side == teams[1]->side){
        std::cout << "WARNING: both teams were assigned the same side" << std::endl
                  << "         moving TEAM_YELLOW to other side of the field." << std::endl;
        teams[TEAM_YELLOW]->side = 1-teams[TEAM_YELLOW]->side;
    }
}

Team* Team::getTeam(int id){
    return teams[id];
}


Robot* Team::addRobot(int id){
    std::cout << robot_type << " type\n";
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


int Team::getColor(){
    return color;
}
std::string Team::getRobotType(){
    return robot_type;
}
int Team::getSide(){
    return side;
}

bool Team::isControlled(){
    return robot_type != "none";
}



