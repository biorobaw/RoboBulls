#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H
#include "strategy.h"
class Robot;

/*! @file
 * Class to test behaviors and other code
 *
 * Not part of the main game, TestStrategy exists as a sandbox
 * to test code. To use, change REFBOX_LISTEN_ENABLED to 0 in
 * include/config/communication.h. By doing this, StrategyController
 * will always select and run TestStrategy instead of the strategy
 * given by the RefBox.
 * @see StrategyController
 * @see include/config/communication.h */

class TestStrategy : public Strategy
{
public:
    void assignBeh() override;
    bool update() override;
};

#endif // TESTSTRATEGY_H

