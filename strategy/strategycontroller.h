#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

#include <iostream>
#include "strategy.h"
#include "../Model/gamemodel.h"
#include "stopstrategy.h"
#include "Robot/robot.h"
#include "Performance/behavior.h"
#include "Performance/stopbehavior.h"

using namespace std;

class GameModel;
class Behavior;

class StrategyController
{
public:
    StrategyController();
    void gameModelUpdated();
    void setGameModel(GameModel *);

private:
    strategy *activeStrategy;
    GameModel * model;
//    StopStrategy stopS;
    Behavior * beh;

};

#endif // STRATEGYCONTROLLER_H
