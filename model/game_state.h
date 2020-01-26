#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <vector>
#include "utilities/point.h"
#include "utilities/measurements.h"
#include "model/robot.h"
#include "model/team.h"
#include <mutex>
#include "yaml-cpp/yaml.h"
#include "parameters/game_constants.h"

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
class GameState
{
public:

    /*! @name Game access functions
     * @{*/

    char   getBlueGoals();
    char   getYellowGoals();
    short  getRemainingTime();
    char   getState();
    char   getPreviousState();

    Point  getPenaltyPoint();
    Point  getOppGoal();
    Point  getMyGoal();
    //! @}

    //! @name grSim Replacement functions
    //! @{
    void addRobotReplacement(int id, int team, float x, float y, float dir = -10);
    void addBallReplacement(float x, float y, float vx = 0, float vy = 0);
    //! @}

    //! @name Misc. Functions
    //! @{
    bool isNewCommand();
    void onCommandProcessed();
    std::string toString();
    //! @}

    static std::mutex my_team_mutex;
    static std::mutex opp_team_mutex;


private:
    /* General Game Information */


    char   state           = '\0';        //The current state of the game from RefComm
    char   previousState   = '\0';      //The previous gamestate
    bool   hasNewCommand   = false;       //True on the iteration that gameState has changed
    char   blueGoals       = 0;           //Number of scores yellow goals
    char   yellowGoals     = 0;           //Number of scores yellow goals
    short  remainingTime   = 0;           //Remaining time in seconds

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code. */
    friend class VisionComm;
    friend class RefComm;
    friend class SimRobComm;
    void onRobotUpdated(Robot*);


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
extern GameState* gameState;
//! @}

#endif // GAMEMODEL_H
