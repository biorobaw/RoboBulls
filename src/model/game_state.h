#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <vector>
#include "ssl-game-controller/ssl_referee_includes.h"
#include "utilities/point.h"
#include "utilities/measurements.h"
#include "robot/robot.h"
#include "model/team.h"
#include <mutex>
#include "yaml-cpp/yaml.h"


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

    static Referee_Command getRefereeCommand();
    static Referee_Command getPreviousCommand();
    static char   getBlueGoals();
    static char   getYellowGoals();
    static int    getRemainingTime();

    //! @}



private:
    /* General Game Information */


    static Referee_Command refereeCommand;  //The last referee command received
    static Referee_Command previousCommand ;  //The previous gamestate
    static char   blueGoals     ;  //Number of scores yellow goals
    static char   yellowGoals   ;  //Number of scores yellow goals
    static int    remainingTime ;  //Remaining time in seconds

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code. */
    friend class SSLVisionListener;
    friend class SSLRefBoxListener;
    friend class SimRobComm;
    static void onRobotUpdated(Robot*);


    static void setRefereeCommand(Referee_Command);
    static void setTimeLeft(int);
    static void setBlueGoals(char);
    static void setYellowGoals(char);
    static void notifyObservers();

};

//Global singleton pointer to access GameModel
//! @}

#endif // GAMEMODEL_H
