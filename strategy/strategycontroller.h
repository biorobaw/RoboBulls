#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

class GameModel;
class Behavior;
class Strategy;

class StrategyController
{
public:
    StrategyController();
    void gameModelUpdated();
    void setGameModel(GameModel *);

private:
    Strategy *activeStrategy;
    GameModel * model;
    bool sent = false;
};

#endif // STRATEGYCONTROLLER_H
