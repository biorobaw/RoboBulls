#ifndef HALTSTRATEGY_H
#define HALTSTRATEGY_H
#include "model/team/team_strategy.h"

/*! @brief Strategy to implement the Halt game state 'H'
 *  @author Narges G */

class HaltStrategy : public TeamStrategy
{
public:
    HaltStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    virtual QString getName() override;
};

#endif // HALTSTRATEGY_H
