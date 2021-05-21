#include "scontroller_joystick.h"
#include "model/team/team_strategy.h"

#include "mygamepad.h"
#include "model/robot/robot.h"
#include "model/team/team.h"

#include "utilities/my_yaml.h"
#include "utilities/debug.h"

#include <QDebug>

#define SLOW_MULT 0.2    // Multiplier for slow-mode
#define MAX_ANGUAR 3.14  // 2pi radians per second
#define MAX_LINEAR 500   // 500mm/s = 0.5m/s



QThread* SControllerJoystick::thread = new QThread;
RobotTeam* SControllerJoystick::teams[2] = {nullptr};
QList<MyGamepad*> SControllerJoystick::gamepads;


void SControllerJoystick::init_module(RobotTeam* teams[2]){
//    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
//    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SControllerJoystick::teams[0] = teams[0];
    SControllerJoystick::teams[1] = teams[1];
    debug::registerFunction("mapjoy", map_joystick_fun);
    debug::registerFunction("p", print_joymap);

    // Add gamepad manager to thread (otherwise thread does not process joy events)
    QGamepadManager::instance()->moveToThread(thread);
    thread->start();




}




SControllerJoystick::SControllerJoystick(RobotTeam* _team, YAML::Node* n)
  : TeamStrategyController(_team, n)
{
    teams[_team->getID()] = _team;
    auto node_map = (*n)["JOY_TO_ROBOT_MAP"];
    qInfo() << "            JOY_TO_ROBOT_MAP  : ";
    for(auto it : node_map){
        qInfo() << "                - ROBOT :" << it["ROBOT"];
        qInfo() << "                  JOY   :" << it["JOY"]  ;
        map_joystick(it["JOY"].as<int>(),team->getID(),it["ROBOT"].as<int>(),false);
    }

}

int SControllerJoystick::getControllerState(Referee_Command game_command){
    (void)game_command;
    return 0; // this controller only has one state;
}
int SControllerJoystick::getNextControllerState(int current_state,int strategy_status){
    (void) current_state;
    (void) strategy_status;
    return 0; // we only have one state
}
TeamStrategy* SControllerJoystick::loadStateStrategy(int state){
    (void) state;
    return new TeamStrategy(team); // this strategy will not really be used
}



void SControllerJoystick::map_joystick(int joy_id, int team_id, int robot_id, bool override_controller)
{
    // make sure we got a valid team_id
    if(team_id!=ROBOT_TEAM_BLUE && team_id!=ROBOT_TEAM_YELLOW){
        qWarning() << "ERROR: team can only be" << ROBOT_TEAM_BLUE << " for the blue team";
        qWarning() << "                     or" << ROBOT_TEAM_YELLOW << " for the yellow team";
        return;
    }

    if(joy_id >= QGamepadManager::instance()->connectedGamepads().size()){
        qWarning() << "WARNING: requested joy_id does not yet exist, connecting joystick anyway";
    }

    auto gpad  = new MyGamepad(joy_id, nullptr);
    gamepads.push_back(gpad);



    gpad->connectToRobot(teams[team_id]->getRobot(robot_id), override_controller);
    gpad->moveToThread(thread);
    connect(thread, &QThread::finished, gpad, &QObject::deleteLater);

    qDebug() << gpad;

}


void SControllerJoystick::map_joystick_fun(const std::vector<std::string>& args)
{
    if(args.size() != 3) {
        qWarning() << "Usage: mapjoy <joy-id> <team> <rob-id>";
        return;
    }

    // parse arguments
    int joy_id = std::atoi(args[0].c_str());
    int team_id = std::atoi(args[1].c_str());
    int robot_id = std::atoi(args[2].c_str());

    // map joystick to robot
    map_joystick(joy_id, team_id, robot_id, true);
}

void SControllerJoystick::print_joymap(const std::vector<std::string>& args)
{
    (void) args;

    for(auto pad : gamepads) qDebug() << pad;

}




void SControllerJoystick::stop_module(){
    thread->exit();
    thread->wait();
}





