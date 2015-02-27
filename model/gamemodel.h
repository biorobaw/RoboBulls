#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "utilities/point.h"
#include "utilities/measurments.h"
#include "model/robot.h"

class StrategyController;
class VisionComm;
class Robot;

/**
 * @brief The GameModel class
 * Game model class can be considered as the heart of robobulls project
 * all the information from other classes gets updated in gamemodel
 * you can receive most up to date info about robots and ball from this class
 */
class GameModel
{
public:
    GameModel();
    static GameModel* getModel();

    /* Game access functions
     * These should be utilized by normal code to retrieve the state
     * of the game from anywhere in the code
     */
    vector<Robot*>& getOponentTeam();
    vector<Robot*>& getMyTeam();
    Point  getBallPoint();
    Point  getBallVelocity();
    Point  getBallAcceleration();
    Point  getBallPrediction();
    float  getBallSpeed();
    char   getBlueGoals();
    char   getYellowGoals();
    short  getRemainingTime();
    char   getGameState();
    char   getPreviousGameState();
    Point  getPenaltyPoint();
    Point  getOpponentGoal();
    Point  getMyGoal();
    Robot* getHasBall();                    //returns the robot which has the ball
    Robot* findMyTeam(int);                 //Looks for a robot with specified id in myTeam
    Robot* findOpTeam(int);                 //Looks for a robot with specified id in opTeam
    Robot* find(int, std::vector<Robot*>&); //General-case find
    bool   isNewCommand();

private:
    /* StrategyController link */
    StrategyController *sc;

    /* General Game Information */
    vector <Robot*> opTeam;
    vector <Robot*> myTeam;
    Robot* robotWithBall     = NULL;
    Point  ballPoint         = Point(0,0);
    Point  ballVelocity      = Point(0,0);
    Point  ballAcceleration  = Point(0,0);
    Point  ballPrediction    = Point(0,0);
    char   gameState         = '\0';
    bool   hasNewCommand     = false;
    char   blueGoals         = 0;
    char   yellowGoals       = 0;
    short  remainingTime     = 0;
    char   previousGameState = '\0';

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code.
     */
    friend class VisionComm;
    friend class RefComm;
    void onRobotUpdated(Robot*);
    void setBallPoint(Point);
    void setRobotHasBall();
    void setGameState(char);
    void setTimeLeft(short);
    void setBlueGoals(char);
    void setYellowGoals(char);
    void notifyObservers();
    void setPreviousGameState(char);
    
public:
    //Old legacy functions
    bool guiOverride = false; // Ryan
    void setStrategyController(StrategyController * sc);
    void onCommandProcessed();
    std::string toString();
};

//Global singleton pointer to access GameModel
extern GameModel* gameModel;

#endif // GAMEMODEL_H
