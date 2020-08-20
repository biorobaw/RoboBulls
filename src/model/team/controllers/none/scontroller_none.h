#ifndef SCONTROLLERNONE_H
#define SCONTROLLERNONE_H

#include "../../team_strategy_controller.h"

class SControllerNone : public TeamStrategyController
{
    Q_OBJECT
public:
    SControllerNone(RobotTeam*, YAML::Node*);

protected:
    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    TeamStrategy* loadStateStrategy(int state) override;




};

#endif // SCONTROLLERNONE_H
