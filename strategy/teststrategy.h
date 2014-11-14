#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include "strategy.h"

/**
  Class to test behaviors
**/

class Robot;
class TestStrategy : public Strategy
{
public:
    TestStrategy();

    void assignBeh() override;
    bool update() override;
};

#endif // TESTSTRATEGY_H

