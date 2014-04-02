#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Robot/robot.h"
#include "../strategy/strategycontroller.h"

const int MAX_ROBOTS = 3;

class StrategyController;

class GameModel
{
public:
    GameModel();

    //sets
    void setOponentTeam(Robot**);
    void setMyTeam(Robot**);
    void setXBall(int);
    void setYball(int);
    void setGameState(char gameState);
    void setStrategyController(StrategyController * sc);

    //gets
    Robot **getOponentTeam();
    Robot **getMyTeam();
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


    Robot *oponentTeam[MAX_ROBOTS];
    Robot *myTeam[MAX_ROBOTS];
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
