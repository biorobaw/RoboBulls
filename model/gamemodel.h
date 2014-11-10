#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <tuple>
#include <deque>
#include <iostream>
#include <sstream>
#include "utilities/measurments.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "strategy/strategycontroller.h"

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
    Point      getBallPoint();
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

    // Ryan
    bool    guiOverride = false;

    void setStrategyController(StrategyController * sc);
    void onCommandProcessed();
    std::string toString();

private:
    /* Average System.
     * This takes the raw input from the vision system; we need to take
     * the newly-reported points and see if they are outliers (unlikely to be
     * the true positions) and if they are not, average them in with previous 
     * readings to get a more accurate reading of the position
     */
#if MODEL_USE_AVERAGES
    template<typename T>
    struct AverageContainer
    {
        AverageContainer();
        const T& update(const T&);
        std::deque<T> values;
          T averageValue;
        int numOutliers;
    };
    typedef AverageContainer<Point> PointAverage;
    typedef AverageContainer<float> AngleAverage;
    typedef std::pair<PointAverage, AngleAverage> RobotAverage;
    PointAverage ballAverage;
    std::array<RobotAverage, MAX_ROBOTS> myTeamAverages;
    std::array<RobotAverage, MAX_ROBOTS> opTeamAverages;
#endif

    /* Static GameModel pointer and SC */
    static GameModel* model;
    StrategyController *sc;

    /* General Game Information */
    vector <Robot*> opTeam;
    vector <Robot*> myTeam;
    Robot* robotWithBall = NULL;
    Point ballPoint      = Point(0,0);
    char  gameState      = '\0';
    bool  hasNewCommand  = false;
    unsigned char  blueGoals     = 0;
    unsigned char  yellowGoals   = 0;
    unsigned short remainingTime = 0;

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
};


#endif // GAMEMODEL_H
