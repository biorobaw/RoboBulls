#ifndef SCONTROLLERNORMALGAME_H
#define SCONTROLLERNORMALGAME_H

#include "../strategycontroller.h"
#include "yaml-cpp/yaml.h"
#include <map>
using std::map;

class SControllerNormalGame : public StrategyController
{
public:
    SControllerNormalGame(RobotTeam* team, YAML::Node c_node);


    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int last_strategy_return_code) override;
    Strategy* loadStateStrategy(int state) override;

private:
    map<Referee_Command,int> cmd_to_state;
    map<int , map<int, int>> state_transitions;

};

#endif // SCONTROLLERNORMALGAME_H
