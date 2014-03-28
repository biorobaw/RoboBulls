#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

#include "strategy.h"
#include "../Model/gamemodel.h"
#include "stopstrategy.h"
#include "Robot/robot.h"

class GameModel;

class StrategyController
{
public:
    StrategyController();
    void gameModelUpdated();
    void setGameModel(GameModel *);

private:
    strategy *activeStrategy;
    GameModel * model;
    StopStrategy stopS;

};

#endif // STRATEGYCONTROLLER_H
