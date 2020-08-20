#ifndef SCONTROLLERNORMALGAME_H
#define SCONTROLLERNORMALGAME_H

#include "../../strategycontroller.h"
#include <QMap>

namespace YAML {
    class Node;
}

class SControllerNormalGame : public StrategyController
{
public:
    SControllerNormalGame(RobotTeam* team, YAML::Node* c_node);


    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    Strategy* loadStateStrategy(int state) override;

private:
    QMap<Referee_Command,int> cmd_to_state;
    QMap<int , QMap<int, int>> state_transitions;


};

#endif // SCONTROLLERNORMALGAME_H
