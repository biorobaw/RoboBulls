#ifndef SCONTROLLERJOYSTICK_H
#define SCONTROLLERJOYSTICK_H

#include "../../strategycontroller.h"
#include "joystick.h"
#include <atomic>
#include <set>
using std::set;

class SControllerJoystick : public StrategyController
{
private:
    struct MapEntry;
    static set<MapEntry*> map;


public:
    static void map_joystick(int joy_id, int team_id, int robot_id);
    static void map_joystick_fun(const std::vector<std::string>& args);
    static void print_joymap(const std::vector<std::string>& args);
    static void init_module();
    static void stop_module();

    SControllerJoystick(RobotTeam* _team,  YAML::Node n);


private:
    static std::atomic_bool done;
    static void listen_commands();
    static void perform_commands();

protected:
    int getControllerState(Referee_Command game_command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    Strategy* loadStateStrategy(int state) override;

};

#endif // SCONTROLLERJOYSTICK_H
