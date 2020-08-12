#include <algorithm>
#include <assert.h>


#include "team.h"
#include "robot/robot.h"
#include "robot/robot_proxy.h"
#include "strategy/strategycontroller.h"
#include "field.h"
#include "model/game_state.h"

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


    std::cout << "        ROBOT_TYPE  : " << (*t_node)["ROBOT_TYPE"] <<std::endl;
    robot_type = (*t_node)["ROBOT_TYPE"].as<std::string>().c_str();
    auto rob_comm = (*t_node)["ROB_COMM"];
    robot_proxy = RobotProxy::load(robot_type, &rob_comm);

    auto s_controller = (*t_node)["STRATEGY_CONTROLLER"];
    if(robot_type != "none"){
        controller = StrategyController::loadController(this,&s_controller);
        team_controller_name = s_controller["ID"].as<string>();
    }

    game_state = new GameState();


    teams[color] = this;
    std::cout << "--Team_" << (_color == ROBOT_TEAM_BLUE ? "blue" : "yellow") << " DONE" <<std::endl;

}

RobotTeam::~RobotTeam(){
    if(teams[color] == this) teams[color] = NULL;
    closeCommunication();
    if(robot_proxy!=nullptr) {
        delete robot_proxy;
        robot_proxy=nullptr;
    }
    if(controller!=nullptr){
        delete controller;
        controller = nullptr;

    }
}


RobotTeam** RobotTeam::load_teams(YAML::Node* team_nodes){
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
    return teams;
}


/*! @brief Look for a robot in the team with id `id`
 * @param id The id of the robot to look for
 * @see Robot class
 * \return A robot pointer if found, or NULL if not on the team */
Robot* RobotTeam::getRobot(int id)
{
    return game_state->getRobot(color,id);
}

Robot* RobotTeam::getRobotByRole(int role){
    return robotsByRoles[role];
}

QSet<Robot*>& RobotTeam::getRobots(){
    return game_state->getFieldRobots(color);
}


int RobotTeam::getColor(){
    return color;
}
QString RobotTeam::getRobotType(){
    return robot_type;
}

std::string RobotTeam::getTeamControllerName(){
    return team_controller_name;
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

void RobotTeam::closeCommunication(){
    if(robot_proxy!=nullptr){
        robot_proxy->close_communication(game_state->getFieldRobots(color));
    }
}

void RobotTeam::sendVels(){
    if(robot_proxy!=nullptr){
        robot_proxy->sendVels(game_state->getFieldRobots(color));
    }
}

GameState* RobotTeam::getGameState(){
    return game_state;
}


