#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H
#include "strategy.h"
class Robot;

/*! @brief Class to test behaviors and other code
 *
 * Not part of the main game, TestStrategy exists as a sandbox
 * to test code. To use, change USE_TEST_STRATEGY to 1 in StrategyController
 * @see StrategyController
 * @see USE_TEST_STRATEGY */

class TestStrategy : public Strategy
{
public:
    void assignBeh() override;
    bool update() override;
};

#endif // TESTSTRATEGY_H

