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
    static int mySide; //-1 or 1 representing side of field
    static int opSide; //Opposite of mySide

    /* Game access functions
     * These should be utilized by normal code to retrieve the state
     * of the game from anywhere in the code */
    vector<Robot*>& getOponentTeam();
    vector<Robot*>& getMyTeam();
    vector<Robot*>& getBlueTeam();
    vector<Robot*>& getYellowTeam();
    Point  getBallPoint();
    Point  getBallVelocity();
    Point  getBallAcceleration();
    Point  getBallPrediction();
    bool   getBallIsStopped();
    float  getBallSpeed();
    char   getBlueGoals();
    char   getYellowGoals();
    short  getRemainingTime();
    char   getGameState();
    char   getPreviousGameState();
    Point  getPenaltyPoint();
    Point  getOpponentGoal();
    Point  getMyGoal();
    Robot* getHasBall();
    Robot* findMyTeam(int);
    Robot* findOpTeam(int);
    Robot* find(int, std::vector<Robot*>&);
    bool   isNewCommand();
    void   removeRobot(int id, int team);

private:
    /* StrategyController link */
    StrategyController *sc = NULL;          //!< Link to strategy controller

    /* General Game Information */
    vector <Robot*> opTeam;               //!< The team of Robot on my team
    vector <Robot*> myTeam;               //!< The team of Robot on the opponent team
    Robot* robotWithBall   = NULL;        //!< Robot currently holding the ball
    Point  ballPoint       = Point(0,0);  //!< The current point fo the ball on the field
    Point  ballVelocity    = Point(0,0);  //!< The current velocity of the ball on the field
    Point  ballAcceleration= Point(0,0);  //!< The current acceleration of the ball on the field
    Point  ballPrediction  = Point(0,0);  //!< Prediciton point of the ball
    char   gameState       = '\0';        //!< The current state of the game from RefComm
    bool   hasNewCommand   = false;       //!< True on tje iteration that gameState has changed
    char   blueGoals       = 0;           //!< Number of scores yellow goals
    char   yellowGoals     = 0;           //!< Number of scores yellow goals
    short  remainingTime   = 0;           //!< Remaining time in seconds
    char   previousGameState = '\0';      //!< The previous gamestate
    bool   ballStopped = false;           //!< Is the ball stationary?

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code. */
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
    
public:
    //Old legacy functions
    void setStrategyController(StrategyController * sc);
    void onCommandProcessed();
    std::string toString();
};

//Global singleton pointer to access GameModel
extern GameModel* gameModel;

#endif // GAMEMODEL_H
