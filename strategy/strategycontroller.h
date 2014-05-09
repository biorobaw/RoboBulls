#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

#include <iostream>
#include "strategy/strategy.h"
#include "../Model/gamemodel.h"
#include "strategy/stopstrategy.h"
#include "Model/robot.h"
#include "behavior/behavior.h"
#include "behavior/stopbehavior.h"
#include "behavior/penaltybehavior.h"
#include "strategy/penaltystrategy.h"

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
