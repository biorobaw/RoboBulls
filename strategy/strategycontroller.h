#ifndef STRATEGYCONTROLLER_H
#define STRATEGYCONTROLLER_H

using namespace std;

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
//    StopStrategy stopS;
    Behavior * beh;

};

#endif // STRATEGYCONTROLLER_H
