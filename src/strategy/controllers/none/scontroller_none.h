#ifndef SCONTROLLERNONE_H
#define SCONTROLLERNONE_H

#include "../../strategycontroller.h"

class SControllerNone : public StrategyController
{
    Q_OBJECT
public:
    SControllerNone(RobotTeam*, YAML::Node*);

protected:
    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    Strategy* loadStateStrategy(int state) override;




};

#endif // SCONTROLLERNONE_H
