#include "team.h"

#include "model/field.h"
#include "model/game_state.h"
#include "model/robot/robot.h"
#include "model/robot/robot_implementation.h"
#include "model/team/team_strategy_controller.h"
#include "utilities/my_yaml.h"

#include "gui/interface/gui_interface.h"

#include <algorithm>
#include <assert.h>
#include <QDebug>



RobotTeam* RobotTeam::teams[2] = {NULL};

RobotTeam::RobotTeam(YAML::Node* t_node, int _color)
{


    qInfo() << "--TEAM" << (_color == ROBOT_TEAM_BLUE ? "BLUE" : "YELLOW");
    qInfo() << "        SIDE              -" << (*t_node)["SIDE"];


    team_id = _color;
    opponent_id = 1-team_id;
    side =  (*t_node)["SIDE"].as<int>();
    assert(side == FIELD_SIDE_NEGATIVE || side == FIELD_SIDE_POSITIVE);

    // create game_state of the team
    game_state = new GameState(this);
    game_state->setFlipXCoorinates(side!=FIELD_SIDE_NEGATIVE);


    // load robot proxy
    auto proxy_node = (*t_node)["ROBOT_PROXY"];
    robot_proxy = RobotImplementation::load(&proxy_node);
    robot_proxy->setParent(this);

    // assign proxies and team to the robots in the team
    // obs: team must be assigned before the proxy
    for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++)
        game_state->getRobot(team_id,i)->setTeam(this)->specifyImplementation(robot_proxy);

    // load team controller
    auto s_controller = (*t_node)["STRATEGY_CONTROLLER"];
    controller = TeamStrategyController::loadController(this, &s_controller);




    teams[team_id] = this;
    qInfo() << "--Team_" << (_color == ROBOT_TEAM_BLUE ? "blue" : "yellow") << " DONE";

}

RobotTeam::~RobotTeam(){

    if(robot_proxy)
        robot_proxy->stopAndClose(getRobots());

    if(thread){
        thread->exit();
        thread->wait();
    }
}


RobotTeam** RobotTeam::load_teams(YAML::Node* team_nodes){
    auto tb = (*team_nodes)["TEAM_BLUE"], ty = (*team_nodes)["TEAM_YELLOW"];
    teams[ROBOT_TEAM_BLUE] = new RobotTeam(&tb, ROBOT_TEAM_BLUE);
    teams[ROBOT_TEAM_YELLOW] = new RobotTeam(&ty, ROBOT_TEAM_YELLOW);

    // Check sides are correct, if not fix it, but give warning
    if(teams[0]->side == teams[1]->side){
        qCritical().nospace()
                << "ERROR: Both teams were assigned the same field side." << endl
                << "       Fix, teams.yaml and reload the program." << endl;
        exit(-1);
    }
    auto blue_side = teams[ROBOT_TEAM_BLUE]->side;
    Field::NEGATIVE_SIDE_TEAM = blue_side == FIELD_SIDE_NEGATIVE ? ROBOT_TEAM_BLUE : ROBOT_TEAM_YELLOW;

    // start the controllers:
    for(auto t : teams) t->startControlLoop();

    return teams;
}


/*! @brief Look for a robot in the team with id `id`
 * @param id The id of the robot to look for
 * @see Robot class
 * \return A robot pointer if found, or NULL if not on the team */
Robot* RobotTeam::getRobot(int robot_id)
{
    return game_state->getRobot(team_id,robot_id);
}

Robot* RobotTeam::getRobotByRole(int role){
    return robotsByRoles[role];
}

QSet<Robot*>& RobotTeam::getRobots(){
    return game_state->getFieldRobots(team_id);
}

RobotTeam* RobotTeam::setRobotRole(int robot_id, int robot_role){
    auto r = getRobot(robot_id);
    r->setRole(robot_role);
    robotsByRoles[robot_role] = r;
    return this;
}

Robot* RobotTeam::getOpponent(int robot_id){
    return game_state->getRobot(opponent_id,robot_id);
}
QSet<Robot*>& RobotTeam::getOpponents(){
    return game_state->getFieldRobots(opponent_id);
}


int RobotTeam::getID(){
    return team_id;
}

int RobotTeam::getSide(){
    return side;
}



GameState* RobotTeam::getGameState(){
    return game_state;
}

void RobotTeam::startControlLoop(){
    // create a thread
    thread = new QThread;
    qInfo().nospace() << "controller thread (" << getID() << ") " << thread ;

    // create a timer to run the control loop
    // connect timeout to control cycle
    // and thread start to timer start
    timer = new QTimer(this); // timer must be created before moving controller to thread
    timer->setInterval(30); // run control loop every 30 ms
    connect(timer, &QTimer::timeout, this, &RobotTeam::runControlCycle);
    connect(thread, SIGNAL(started()), timer, SLOT(start()));
    connect(thread, &QThread::finished, this, &QObject::deleteLater);
    QTimer::singleShot(7000,controller, &TeamStrategyController::resume);

    // move QObject to thread and then start it
    moveToThread(thread);
    thread->start();
}

void RobotTeam::runControlCycle(){

    // first update game state:
    game_state->update();

    // then, run controller (assigns behaviors to each robot in the field)
    controller->runControlCycle(game_state);

    // then run robot behaviors for all robots that are not overriden by the gui
    for (Robot *rob :  getRobots())
        rob->runControlCycle();

    robot_proxy->sendCommands(game_state->getFieldRobots(team_id));

}
