#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H
#include "../../../strategy.h"
class Robot;

/*! @file
 * Class to test behaviors and other code
 *
 * Not part of the main game, TestStrategy exists as a sandbox
 * to test code. To use, set the variable REFBOX_ENABLED to true in
 * the configuration file "config/comm.yaml". By doing this, StrategyController
 * will always select and run TestStrategy instead of the strategy
 * given by the RefBox.
 * @see StrategyController
 * @see config/comm.yaml */

class TestStrategy : public Strategy
{
public:
    TestStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    int getStatus() override;
    virtual std::string getName() override;
};

#endif // TESTSTRATEGY_H
