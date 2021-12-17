#include "scontroller_normal_game_RL.h"
#include "../normal_game_templated/strategies/normalgamestrategytemplated.h"

#include "../normal_game/normal_game_roles.h"

#include "../normal_game/strategies/stopstrategy.h"
#include "../normal_game/strategies/kickoffstrategy.h"
#include "../normal_game/strategies/freekickstrategy.h"
#include "../normal_game/strategies/penaltystrategy.h"
#include "../normal_game/strategies/indirectkickstrategy.h"
#include "../normal_game/strategies/haltstrategy.h"
#include "../normal_game/strategies/ballplacementstrategy.h"
#include "ssl_referee.pb.h"
#include "model/team/team.h"

#include "model/team/controllers/normal_game_RL/behaviors/attackmainRL.h"
#include "model/team/controllers/normal_game_RL/behaviors/attacksupportRL.h"



namespace  {
    enum ControllerState { INITIAL,
                           STOP,
                           PENALTY,
                           KICK_OFF,
                           FREE_KICK,
                           INDIRECT_KICK,
                           HALT,
                           NORMAL_GAME,
                           BALL_PLACEMENT
                         };
}

SControllerNormalGameRL::SControllerNormalGameRL(RobotTeam* team, YAML::Node* c_node) : SControllerNormalGame(team, c_node){};


TeamStrategy* SControllerNormalGameRL::loadStateStrategy(int state){
    //NormalGameStrategyRL<AttackMainRL, AttackSupportRL> newgame();
    //TeamStrategy *newg = (TeamStrategy*)new NormalGameStrategyRL(team, encodeType<AttackMainRL>(),encodeType<AttackSupportRL>());

    switch (state) {
    case INITIAL :       return new StopStrategy(team);
    case STOP :          return new StopStrategy(team);
    case PENALTY :       return new PenaltyStrategy(team);
    case KICK_OFF :      return new KickOffStrategy(team);
    case FREE_KICK :     return new FreeKickStrategy(team);
    case INDIRECT_KICK : return new IndirectKickStrategy(team);
    case HALT :          return new HaltStrategy(team);
    case BALL_PLACEMENT: return new BallPlacementStrategy(team);
    case NORMAL_GAME :   return new NormalGameStrategyTemplated<AttackMainRL, AttackSupportRL>(team);;
    }
    return new HaltStrategy(team);
}

