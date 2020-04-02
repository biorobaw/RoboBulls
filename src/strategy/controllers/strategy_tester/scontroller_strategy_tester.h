#ifndef SCONTROLLERSTRATEGYTESTER_H
#define SCONTROLLERSTRATEGYTESTER_H

#include "../../strategycontroller.h"

class SControllerStrategyTester : public StrategyController
{
public:
    SControllerStrategyTester(RobotTeam* team, YAML::Node node);

protected:
    int getControllerState(Referee_Command game_command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    Strategy* loadStateStrategy(int state) override;

};

#endif // SCONTROLLERSTRATEGYTESTER_H
