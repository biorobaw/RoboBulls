#ifndef HALTSTRATEGY_H
#define HALTSTRATEGY_H
#include "../strategy.h"

/*! @brief Strategy to implement the Halt game state 'H'
 *  @author Narges G */

class HaltStrategy : public Strategy
{
public:
    HaltStrategy(RobotTeam* _team);
    void assignBehaviors();
    virtual std::string getName() override;
};

#endif // HALTSTRATEGY_H
