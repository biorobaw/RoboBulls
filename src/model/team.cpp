#include <algorithm>
#include <assert.h>


#include "team.h"
#include "robot/robot.h"
#include "robot/robcomm.h"
#include "strategy/strategycontroller.h"
#include "field.h"

#include "yaml-cpp/yaml.h"

RobotTeam* RobotTeam::teams[2] = {NULL};

RobotTeam::RobotTeam(YAML::Node* t_node, int _color) {

    std::cout << "--TEAM_" << (_color == ROBOT_TEAM_BLUE ? "BLUE" : "YELLOW") << std::endl;
    std::cout << "        SIDE        : " << (*t_node)["SIDE"] <<std::endl;
    if((*t_node)["ROLES"].IsDefined()){
        std::cout << "        ROLES : " << std::endl;
        std::cout << "            GOALIE  : " << (*t_node)["ROLES"]["GOALIE" ] << std::endl;
        std::cout << "            ATTACK1 : " << (*t_node)["ROLES"]["ATTACK1"] << std::endl;
        std::cout << "            ATTACK2 : " << (*t_node)["ROLES"]["ATTACK2"] << std::endl;
        std::cout << "            ATTACK3 : " << (*t_node)["ROLES"]["ATTACK3"] << std::endl;
        std::cout << "            DEFEND1 : " << (*t_node)["ROLES"]["DEFEND1"] << std::endl;
        std::cout << "            DEFEND2 : " << (*t_node)["ROLES"]["DEFEND2"] << std::endl;
        std::cout << "            DEFEND3 : " << (*t_node)["ROLES"]["DEFEND3"] << std::endl;
    }


    color = _color;
    side =  (*t_node)["SIDE"].as<int>();
    assert(side == FIELD_SIDE_NEGATIVE || side == FIELD_SIDE_POSITIVE);

    if((*t_node)["ROLES"].IsDefined()){
        if((*t_node)["ROLES"]["GOALIE" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["GOALIE" ].as<int>()] = RobotRole::GOALIE;

        if((*t_node)["ROLES"]["ATTACK1" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["ATTACK1"].as<int>()] = RobotRole::ATTACK1;

        if((*t_node)["ROLES"]["ATTACK2" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["ATTACK2"].as<int>()] = RobotRole::ATTACK2;

        if((*t_node)["ROLES"]["ATTACK3" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["ATTACK3"].as<int>()] = RobotRole::ATTACK3;

        if((*t_node)["ROLES"]["DEFEND1" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["DEFEND1"].as<int>()] = RobotRole::DEFEND1;

        if((*t_node)["ROLES"]["DEFEND2" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["DEFEND2"].as<int>()] = RobotRole::DEFEND2;

        if((*t_node)["ROLES"]["DEFEND3" ].IsDefined())
            idToRole[(*t_node)["ROLES"]["DEFEND3"].as<int>()] = RobotRole::DEFEND3;
    }



    std::cout << "        ROBOT_TYPE  : " << (*t_node)["ROBOT_TYPE"] <<std::endl;
    robot_type = (*t_node)["ROBOT_TYPE"].as<std::string>();
    auto rob_comm = (*t_node)["ROB_COMM"];
    comm = RobComm::loadRobComm(robot_type,&rob_comm);

    auto s_controller = (*t_node)["STRATEGY_CONTROLLER"];
    if(robot_type != "none"){
        controller = StrategyController::loadController(this,&s_controller);
        controller_name = s_controller["ID"].as<string>();
    }




    teams[color] = this;
    std::cout << "--Team_" << (_color == ROBOT_TEAM_BLUE ? "blue" : "yellow") << " DONE" <<std::endl;

}

RobotTeam::~RobotTeam(){
    if(teams[color] == this) teams[color] = NULL;
    closeCommunication();
    if(comm!=nullptr) {
        delete comm;
        comm=nullptr;
    }
    if(controller!=nullptr){
        delete controller;
        controller = nullptr;

    }
}


void RobotTeam::load_teams(YAML::Node* team_nodes){
    auto tb = (*team_nodes)["TEAM_BLUE"], ty = (*team_nodes)["TEAM_YELLOW"];
    teams[ROBOT_TEAM_BLUE] = new RobotTeam(&tb, ROBOT_TEAM_BLUE);
    teams[ROBOT_TEAM_YELLOW] = new RobotTeam(&ty, ROBOT_TEAM_YELLOW);

    if(teams[0]->side == teams[1]->side){
        std::cout << "WARNING: both teams were assigned the same side" << std::endl
                  << "         moving TEAM_YELLOW to other side of the field." << std::endl;
        teams[ROBOT_TEAM_YELLOW]->side = teams[ROBOT_TEAM_YELLOW]->getOpponentSide();
    }
    auto blue_side = teams[ROBOT_TEAM_BLUE]->side;
    Field::NEGATIVE_SIDE_TEAM = blue_side == FIELD_SIDE_NEGATIVE ? ROBOT_TEAM_BLUE : ROBOT_TEAM_YELLOW;

}

RobotTeam* RobotTeam::getTeam(int id){
    return teams[id];
}


Robot* RobotTeam::addRobot(int id){
//    std::cout << robot_type << " type\n";
    auto role = idToRole[id];
    auto r = Robot::loadRobot(robot_type,id,color,role);

    robotById[id] = r;
    robotByRoles[idToRole[id]] = r;
    all_robots.insert(r);
    return r;
}

void RobotTeam::removeRobot(int id){
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
Robot* RobotTeam::getRobot(int id)
{
    return robotById[id];
}

Robot* RobotTeam::getRobotByRole(RobotRole role){
    return robotByRoles[role];
}

std::set<Robot*>& RobotTeam::getRobots(){
    return all_robots;
}


int RobotTeam::getColor(){
    return color;
}
std::string RobotTeam::getRobotType(){
    return robot_type;
}

std::string RobotTeam::getControllerName(){
    return controller_name;
}

std::string RobotTeam::getStrategyName(){
    return controller!=nullptr ? controller->getStrategyName() : "none";
}

int RobotTeam::getSide(){
    return side;
}

int RobotTeam::getOpponentSide(){
    return side == FIELD_SIDE_NEGATIVE ?
                FIELD_SIDE_POSITIVE :
                FIELD_SIDE_NEGATIVE;
}

bool RobotTeam::isControlled(){
    return controller != nullptr;
}

void RobotTeam::closeCommunication(){
    if(comm!=nullptr){
        comm->close_communication(all_robots);
    }
}

void RobotTeam::sendVels(){
    if(comm!=nullptr){
        comm->sendVels(all_robots);
    }
}

