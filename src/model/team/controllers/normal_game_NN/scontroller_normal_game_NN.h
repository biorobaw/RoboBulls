#ifndef SCONTROLLERNORMALGAMENN_H
#define SCONTROLLERNORMALGAMENN_H

#include "../normal_game/scontroller_normal_game.h"

namespace YAML {
    class Node;
}

class SControllerNormalGameNN : public SControllerNormalGame
{
public:
    SControllerNormalGameNN(RobotTeam* team, YAML::Node* c_node);


    TeamStrategy* loadStateStrategy(int state) override;

};

#endif // SCONTROLLERNORMALGAMENN_H
