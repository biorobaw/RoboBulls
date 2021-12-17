#ifndef SCONTROLLERNORMALGAMERL_H
#define SCONTROLLERNORMALGAMERL_H

#include "../normal_game/scontroller_normal_game.h"

namespace YAML {
    class Node;
}

class SControllerNormalGameRL : public SControllerNormalGame
{
public:
    SControllerNormalGameRL(RobotTeam* team, YAML::Node* c_node);


    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    TeamStrategy* loadStateStrategy(int state) override;

};

#endif // SCONTROLLERNORMALGAMERL_H
