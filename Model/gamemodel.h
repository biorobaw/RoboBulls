#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Robot/robot.h"
#include "../strategy/strategycontroller.h"
#include <vector>

using namespace std;

const int MAX_ROBOTS = 3;

class StrategyController;

class GameModel
{
public:
    GameModel();

    //sets
    void setOponentTeam(vector<Robot*>);
    void setMyTeam(vector<Robot*>);
    void setXBall(int);
    void setYball(int);
    void setGameState(char gameState);
    void setStrategyController(StrategyController * sc);

    //gets
    vector <Robot *> getOponentTeam();
    vector <Robot *> getMyTeam();
    int getXBall();
    int getYBall();
    int getBlueGoals();
    int getYellowGoals();
    float getRemainingTime();
    char getGameState();

    static GameModel * getModel();
private:
    static GameModel * model;

    //GameModel(StrategyController * sc);


    vector <Robot*>oponentTeam;
    vector <Robot*>myTeam;
    int x_ballPosition;
    int y_ballPosition;

    //State gameState;
    int blueGoals;
    int yellowGoals;
    float remainingTime;

    char gameState;

    StrategyController *sc;
};





#endif // GAMEMODEL_H
