#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <vector>
#include "utilities/point.h"
#include "utilities/measurements.h"
#include "model/robot.h"
#include "model/team.h"
#include <mutex>
#include "yaml-cpp/yaml.h"
#include "include/game_constants.h"

class StrategyController;
class VisionComm;
class Robot;

/*! @addtogroup everydayuse Everyday Use
 * Classes and functions used in everyday code
 * @{
 */

/*! The GameModel class
 * Game model class can be considered as the heart of robobulls project.
 * All the information from other classes gets updated in gamemodel.
 * You can receive most up to date info about robots and ball from this class.
 *
 * <b>Example Everyday Usage</b>
 * @include example_gamemodel.cpp
 */
class GameModel
{
public:
    static GameModel* getModel();


    /*! @name Game access functions
     * @{*/
    void  setTeams(YAML::Node team_node);
    Team& getOppTeam();
    Team& getMyTeam();
    Team& getTeam(int color);

    Point  getBallPoint();
    Point  getBallVelocity();
    Point  getBallStopPoint();
    float  getBallSpeed();
    char   getBlueGoals();
    char   getYellowGoals();
    short  getRemainingTime();
    char   getGameState();
    char   getPreviousGameState();
    Point  getPenaltyPoint();
    Point  getOppGoal();
    Point  getMyGoal();
    Robot* getHasBall();
    Robot* findMyTeam(int);
    Robot* findOpTeam(int);
    bool is_simulation;
    //! @}

    //! @name grSim Replacement functions
    //! @{
    void addRobotReplacement(int id, int team, float x, float y, float dir = -10);
    void addBallReplacement(float x, float y, float vx = 0, float vy = 0);
    //! @}

    //! @name Misc. Functions
    //! @{
    //Robot* find(int, std::vector<Robot*>&);
    bool isNewCommand();
    void removeRobot(int id, int team);
    void setStrategyController(StrategyController * sc);
    void onCommandProcessed();
    std::string toString();
    //! @}

    static std::mutex my_team_mutex;
    static std::mutex opp_team_mutex;

    static bool OUR_TEAM;     // Added as intermediate while removing all hard coded values

private:
    /* StrategyController link */
    StrategyController *sc = NULL;        //Link to strategy controller

    /* General Game Information */
//    std::vector<Robot*> opTeam;           //The team of Robot on my team
//    std::vector<Robot*> myTeam;           //The team of Robot on the opponent team
    Team myTeam;
    Team opTeam;


    Robot* robotWithBall   = NULL;        //Robot currently holding the ball
    Point  ballPoint       = Point(0,0);  //The current point fo the ball on the field
    Point  ballVelocity    = Point(0,0);  //The current velocity of the ball on the field
    Point  ballStopPoint   = Point(0,0);  //The predicted stop point of the ball
    Point  ballPrediction  = Point(0,0);  //Prediciton point of the ball
    char   gameState       = '\0';        //The current state of the game from RefComm
    bool   hasNewCommand   = false;       //True on the iteration that gameState has changed
    char   blueGoals       = 0;           //Number of scores yellow goals
    char   yellowGoals     = 0;           //Number of scores yellow goals
    short  remainingTime   = 0;           //Remaining time in seconds
    char   previousGameState = '\0';      //The previous gamestate

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code. */
    friend class VisionComm;
    friend class RefComm;
    friend class SimRobComm;
    void onRobotUpdated(Robot*);
    void setBallPoint(Point);
    void setBallVelocity(Point);
    void setBallStopPoint(Point);
    void setRobotHasBall();
    void setGameState(char);
    void setTimeLeft(short);
    void setBlueGoals(char);
    void setYellowGoals(char);
    void notifyObservers();

    //! @cond
    //grSim Replacement data
    struct RobotReplacement {
        int id;
        int team;
        float x, y, dir;
    };
    struct BallReplacement {
        float x, y;
        float vx, vy;
    };

    //! @endcond
    //Vector of queued replacements for robots
    std::vector<RobotReplacement> robotReplacements;
    //An optimal replacement of the ball
    BallReplacement ballReplacement;
    //Do the above two fields contains replacemant data?
    bool hasRobotReplacements = false;
    bool hasBallReplacement = false;
};

//Global singleton pointer to access GameModel
extern GameModel* gameModel;
//! @}

#endif // GAMEMODEL_H
