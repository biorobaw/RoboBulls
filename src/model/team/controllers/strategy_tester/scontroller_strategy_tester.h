#ifndef SCONTROLLERSTRATEGYTESTER_H
#define SCONTROLLERSTRATEGYTESTER_H

#include "../../team_strategy_controller.h"


class SControllerStrategyTester : public TeamStrategyController
{
public:
    SControllerStrategyTester(RobotTeam* team, YAML::Node* node);

    std::string getName();
protected:
    int getControllerState(Referee_Command game_command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    TeamStrategy* loadStateStrategy(int state) override;

};

#endif // SCONTROLLERSTRATEGYTESTER_H
