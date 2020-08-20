#ifndef SCONTROLLERJOYSTICK_H
#define SCONTROLLERJOYSTICK_H

#include "../../team_strategy_controller.h"
#include <QThread>
#include <QList>
#include "configuration/constants.h"


class MyGamepad;


class SControllerJoystick : public TeamStrategyController
{

public:
    static void init_module(RobotTeam* teams[2]);
    static void stop_module();

    static void map_joystick_fun(const std::vector<std::string>& args);
    static void print_joymap(const std::vector<std::string>& args);


    SControllerJoystick(RobotTeam* _team,  YAML::Node* n);

private:

    static void map_joystick(int joy_id, int team_id, int robot_id, bool override_controller);
    static RobotTeam* teams[2];

protected:
    int getControllerState(Referee_Command game_command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    TeamStrategy* loadStateStrategy(int state) override;

    static QThread*  thread;
    static QList<MyGamepad*> gamepads;

};

#endif // SCONTROLLERJOYSTICK_H
