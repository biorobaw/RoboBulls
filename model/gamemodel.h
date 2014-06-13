#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <iostream>
#include <sstream>
#include "utilities/measurments.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "strategy/strategycontroller.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"


using namespace std;

/**
 * @brief MAX_ROBOTS
 * defines the maximum number of robots that can exist in each team
 * Narges Ghaedi
 */
const int MAX_ROBOTS = 3;

class StrategyController;
class Robot;

/**
 * @brief The GameModel class
 * Game model class can be considered as the heart of robobulls project
 * all the information from other classes gets updated in gamemodel
 * you can recieve most up to date info about robots and ball from this class
 */
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
//    void setPenaltyPoint();

    //calculates the distance between robots and ball, and determines which robot has the ball
    void setHasBall();

    //gets
    const vector<Robot *>& getOponentTeam();
    const vector<Robot *>& getMyTeam();
    float getXBall();
    float getYBall();
    Point getBallPoint();
    int   getBlueGoals();
    int   getYellowGoals();
    float getRemainingTime();
    char  getGameState();
    Point getPenaltyPoint();
    Point getOpponentGoal();
    Point getMyGoal();

    //returns the robot which has the ball
//    Robot * getHasBall();
//    void play();

    //Other Functions
    Robot* find(int, const vector<Robot *>&);

    static GameModel * getModel();

    std::string toString();


private:
    static GameModel * model;

    //GameModel(StrategyController * sc);


    vector <Robot*> opTeam;
    vector <Robot*> myTeam;
    float x_ballPosition;
    float y_ballPosition;
    Point ballPoint;
    Point penaltyPoint;

    //State gameState;
    int blueGoals;
    int yellowGoals;
    float remainingTime;

    char gameState;

    StrategyController *sc;
};





#endif // GAMEMODEL_H
