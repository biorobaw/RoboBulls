#include "scontroller_joystick.h"
#include "../../strategies/haltstrategy.h"
#include <QtConcurrent/QtConcurrent>
#include "yaml-cpp/yaml.h"
#include <iostream>
#include "model/team.h"
using std::cout, std::endl, std::cerr;

#define SLOW_MULT 0.2    // Multiplier for slow-mode
#define MAX_ANGUAR 3.14  // 2pi radians per second
#define MAX_LINEAR 500   // 500mm/s = 0.5m/s


set<SControllerJoystick::MapEntry*> SControllerJoystick::map = set<SControllerJoystick::MapEntry*>();
std::atomic_bool SControllerJoystick::done = true;

SControllerJoystick::SControllerJoystick(RobotTeam* _team, YAML::Node* n)
  : StrategyController(_team, n)
{
    auto node_map = (*n)["JOY_TO_ROBOT_MAP"];
    std::cout << "            JOY_TO_ROBOT_MAP  : " <<std::endl;
    for(auto it : node_map){
        cout << "                - ROBOT : " << it["ROBOT"] << endl
             << "                  JOY   : " << it["JOY"] << endl;
        map_joystick(it["JOY"].as<int>(),team->getColor(),it["ROBOT"].as<int>());
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
Strategy* SControllerJoystick::loadStateStrategy(int state){
    (void) state;
    return new HaltStrategy(team); // this strategy will not really be used
}





struct SControllerJoystick::MapEntry {
    MapEntry(int team_id, int robot_id, JoyStick* joy)
        : team_id(team_id), robot_id(robot_id), joy(joy)
    {

    }
    int team_id;
    int robot_id;
    JoyStick* joy;
};


void SControllerJoystick::map_joystick(int joy_id, int team_id, int robot_id)
{
    // make sure we got a valid team_id
    if(team_id!=ROBOT_TEAM_BLUE && team_id!=ROBOT_TEAM_YELLOW){
        cerr << "ERROR: team can only be " << ROBOT_TEAM_BLUE << " for the blue team" << endl
             << "                     or " << ROBOT_TEAM_YELLOW << " for the yellow team" << endl;
        return;
    }

    // create joystick and add a mapping item
    JoyStick* aux = JoyStick::load_joystick(joy_id);
    if(aux!= nullptr) {
        map.insert(new MapEntry(team_id,robot_id,aux));
        cerr << "Joystick \"" << SDL_JoystickNameForIndex(joy_id) << "\" mapped to "
                  << robot_id << " from team " << team_id <<std::endl;
    }

}

void SControllerJoystick::map_joystick_fun(const std::vector<std::string>& args)
{
    if(args.size() != 3) {
        std::cerr << "Usage: mapjoy <joy-id> <team> <rob-id>";
        return;
    }

    // parse arguments
    int joy_id = std::atoi(args[0].c_str());
    int team_id = std::atoi(args[1].c_str());
    int robot_id = std::atoi(args[2].c_str());

    // map joystick to robot
    map_joystick(joy_id, team_id, robot_id);
}

void SControllerJoystick::print_joymap(const std::vector<std::string>& args)
{
    (void) args;
    for(auto& entry : map){
        cout << entry->joy->getJoyId() << ": \"" << entry->joy->getJoyName()
             << "\" connected to robot " << entry->robot_id
             << " from team " <<entry->team_id << endl;
    }
}


void SControllerJoystick::init_module(){
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    debug::registerFunction("mapjoy", map_joystick_fun);
    debug::registerFunction("p", print_joymap);
    QtConcurrent::run(listen_commands);
}

void SControllerJoystick::listen_commands(){
    done = false;
    while(!done)
    {
        SDL_Delay(30);
        SDL_Event event;
        while (SDL_PollEvent(&event))
            if(event.type == SDL_QUIT) {
                done = true;
                break;
            }

        perform_commands();

    }
    SDL_Quit();
}

void SControllerJoystick::stop_module(){
    done = true;
}



void SControllerJoystick::perform_commands(){
    // check whether teams have already been created
    if(!RobotTeam::getTeam(0) || !RobotTeam::getTeam(1))
        return;

    // perform joy commands for each robot
    for(MapEntry* entry : map){
        auto r = RobotTeam::getTeam(entry->team_id)->getRobot(entry->robot_id);
        if(r==nullptr) continue;

//        cerr << "joy: " << joy->getSlowMode() << " " << joy->getKick() << " "
//             << joy->getDribble() << " "
//             << joy->getAxisUp() << " " << joy->getAxisSide() << " " << joy->getAxisRotate() << endl;

        // Calculate velocities to set
        JoyStick* joy = entry->joy;
        float multiplier = joy->getSlowMode() ? SLOW_MULT : 1;
        float axis_rot = -joy->getAxisRotate();
        float axis_up  = joy->getAxisUp();

        float w  =  axis_rot* MAX_ANGUAR * multiplier;
        float v  =  sqrt(axis_rot*axis_rot + axis_up*axis_up)*MAX_LINEAR * multiplier;
        if(axis_up < 0) {
            v=-v;
            w=-w;
        }


        // Set velocities on robot object through movement interface
        r->getPilot()->setManualVelocity(Point(v, 0), w);
        if(joy->getKick()) r->setKick();
        r->setDribble(joy->getDribble());
    }
}


