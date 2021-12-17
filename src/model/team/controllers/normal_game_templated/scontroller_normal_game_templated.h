#ifndef SCONTROLLERNORMALGAMETEMPLATED_H
#define SCONTROLLERNORMALGAMETEMPLATED_H

#include "../normal_game/scontroller_normal_game.h"

#include <QMap>

/*!
 *  This normal game controller is templated to allow for reuse of
 *  the Normal Game Strategy without using inheritance or copy/pasting
 *  the entire file/code structure everywhere. At the team_strategy_controller
 *  level the input will be used to call this controller passing the behaviors
 *  or other templated items for easy swapping in and out of individual behaviors
 *  /skills.
 *   - Justin Rodney
 *  */

namespace YAML {
    class Node;
}
template<class ATK, class SPT>
class SControllerNormalGameTemplated : public TeamStrategyController
{
public:
    SControllerNormalGameTemplated(RobotTeam* team, YAML::Node* c_node);


    int getControllerState(Referee_Command command) override;
    int getNextControllerState(int current_state,int strategy_status) override;
    TeamStrategy* loadStateStrategy(int state) override;

private:
    QMap<Referee_Command,int> cmd_to_state;
    QMap<int , QMap<int, int>> state_transitions;


};


#endif // SCONTROLLERNORMALGAMETEMPLATED_H
