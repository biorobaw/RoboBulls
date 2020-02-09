#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <vector>
#include "utilities/point.h"
#include "utilities/measurements.h"
#include "model/robot.h"
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

    static char   getState();
    static char   getBlueGoals();
    static char   getYellowGoals();
    static short  getRemainingTime();
    static char   getPreviousState();

    //! @}



private:
    /* General Game Information */


    static char   state        ;  //The current state of the game from RefComm
    static char   previousState;  //The previous gamestate
    static char   blueGoals    ;  //Number of scores yellow goals
    static char   yellowGoals  ;  //Number of scores yellow goals
    static short  remainingTime;  //Remaining time in seconds

    /* Functions to update gamemodel from vision system.
     * Provides *the* link between vision detection and
     * referee box with our code. */
    friend class SSLVisionListener;
    friend class SSLRefBoxListener;
    friend class SimRobComm;
    static void onRobotUpdated(Robot*);


    static void setGameState(char);
    static void setTimeLeft(short);
    static void setBlueGoals(char);
    static void setYellowGoals(char);
    static void notifyObservers();

};

//Global singleton pointer to access GameModel
//! @}

#endif // GAMEMODEL_H
