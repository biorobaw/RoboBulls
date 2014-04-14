#ifndef GAMEMODEL_H
#define GAMEMODEL_H


#include "Measure/point.h"
#include "../strategy/strategycontroller.h"
#include <vector>
#include <iostream>

using namespace std;

const int MAX_ROBOTS = 3;

class StrategyController;
class Robot;

class GameModel
{
public:
    GameModel();

    //sets
    void setOponentTeam(vector<Robot*>);
    void setMyTeam(vector<Robot*>);
    void setXBall(float);
    void setYball(float);
    void setBallPoint(Point);
    void setGameState(char gameState);
    void setStrategyController(StrategyController * sc);

    //gets
    vector <Robot *> getOponentTeam();
    vector <Robot *> getMyTeam();
    float getXBall();
    float getYBall();
    Point getBallPoint();
    int getBlueGoals();
    int getYellowGoals();
    float getRemainingTime();
    char getGameState();
//    void play();

    static GameModel * getModel();
private:
    static GameModel * model;

    //GameModel(StrategyController * sc);


    vector <Robot*>oponentTeam;
    vector <Robot*>myTeam;
    float x_ballPosition;
    float y_ballPosition;
    Point ballPoint;

    //State gameState;
    int blueGoals;
    int yellowGoals;
    float remainingTime;

    char gameState;

    StrategyController *sc;
};





#endif // GAMEMODEL_H
