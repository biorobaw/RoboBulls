#include "scontroller_strategy_tester.h"
#include "strategies/test_strategy.h"
#include "model/team/controllers/normal_game/strategies/kickoffstrategy.h"

namespace  {
    enum ControllerState { INITIAL,
                           STARTED
                         };
}

SControllerStrategyTester::SControllerStrategyTester(RobotTeam* team, YAML::Node* node)
    : TeamStrategyController(team,node)
{

}

int SControllerStrategyTester::getControllerState(Referee_Command game_command){
    (void)game_command;
    return 0;
}


int SControllerStrategyTester::getNextControllerState(int current_state,int strategy_status){
    (void)current_state;
    (void)strategy_status;
    return 0;
}

TeamStrategy* SControllerStrategyTester::loadStateStrategy(int state){
    (void) state;
    return new TestStrategy(team);
    //return new KickOffStrategy(team);
}


