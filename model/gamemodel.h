#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <deque>
#include "utilities/point.h"
#include "utilities/measurments.h"
#include "model/robot.h"

/**
 * @brief MAX_ROBOTS
 * defines the maximum number of robots that can exist in each team
 * Narges Ghaedi
 */
#if SIMULATED
 const int MAX_ROBOTS = 6;
#else
 const int MAX_ROBOTS = 5;
#endif

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
    Point           getBallPoint();
    unsigned char getBlueGoals();
    unsigned char getYellowGoals();
    unsigned char getRemainingTime();
    char    getGameState();
    Point   getPenaltyPoint();
    Point   getOpponentGoal();
    Point   getMyGoal();
    Robot*  getHasBall();        //returns the robot which has the ball
    Robot*  findMyTeam(int);    //Looks for a robot with specified id in myTeam
    Robot*  findOpTeam(int);    //Looks for a robot with specified id in opTeam
    Robot*  find(int, std::vector<Robot*>&);    //General-case find
    bool    isNewCommand();
    char    getPreviousGameState();

    // Ryan
    bool    guiOverride = false;

    void setStrategyController(StrategyController * sc);
    void onCommandProcessed();
    std::string toString();

private:
    /* StrategyController link */
    StrategyController *sc;

    /* General Game Information */
    vector <Robot*> opTeam;
    vector <Robot*> myTeam;
    Robot* robotWithBall         = NULL;
    Point ballPoint              = Point(0,0);
    char  gameState              = '\0';
    bool  hasNewCommand          = false;
    unsigned char  blueGoals     = 0;
    unsigned char  yellowGoals   = 0;
    unsigned short remainingTime = 0;
    char previousGameState       = '\0';

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code.
     */
    friend class VisionComm;
    friend class RefComm;
    void setRobotUpdated(Robot*, int);
    void setBallPoint(Point);
    void setRobotHasBall();
    void setGameState(char);
    void setTimeLeft(unsigned short);
    void setBlueGoals(unsigned char);
    void setYellowGoals(unsigned char);
    void notifyObservers();
    void setPreviousGameState(char);
};

//Global singleton pointer to access GameModel
extern GameModel* gameModel;

#endif // GAMEMODEL_H
