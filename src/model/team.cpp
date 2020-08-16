#include "team.h"

#include "field.h"
#include "game_state.h"
#include "robot/robot.h"
#include "robot/robot_proxy.h"
#include "strategy/strategycontroller.h"
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


    color = _color;
    side =  (*t_node)["SIDE"].as<int>();
    assert(side == FIELD_SIDE_NEGATIVE || side == FIELD_SIDE_POSITIVE);

    // load robot proxy
    auto proxy_node = (*t_node)["ROBOT_PROXY"];
    robot_proxy = RobotProxy::load(&proxy_node);
    robot_proxy->setParent(this);

    // load team controller
    auto s_controller = (*t_node)["STRATEGY_CONTROLLER"];
    controller = StrategyController::loadController(this, &s_controller);

    game_state = new GameState(this);
    game_state->setFlipXCoorinates(side!=FIELD_SIDE_NEGATIVE);

    for(int i=0; i<MAX_ROBOTS_PER_TEAM; i++)
        game_state->getRobot(color,i)->setRobotProxy(robot_proxy);


    teams[color] = this;
    qInfo() << "--Team_" << (_color == ROBOT_TEAM_BLUE ? "blue" : "yellow") << " DONE";

}

RobotTeam::~RobotTeam(){

    if(robot_proxy)
        robot_proxy->close_communication(getRobots());

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



GameState* RobotTeam::getGameState(){
    return game_state;
}

void RobotTeam::startControlLoop(){
    // create a thread
    thread = new QThread;
    qInfo().nospace() << "controller thread (" << getColor() << ") " << thread ;

    // create a timer to run the control loop
    // connect timeout to control cycle
    // and thread start to timer start
    timer = new QTimer(this); // timer must be created before moving controller to thread
    timer->setInterval(30); // run control loop every 30 ms
    connect(timer, &QTimer::timeout, this, &RobotTeam::runControlCycle);
    connect(thread, SIGNAL(started()), timer, SLOT(start()));
    connect(thread, &QThread::finished, this, &QObject::deleteLater);
    QTimer::singleShot(7000,controller, &StrategyController::resume);

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
        rob->performBehavior();

    robot_proxy->sendVels(game_state->getFieldRobots(color));

}
