#include <sstream>
#include <iostream>
#include <fstream>

#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"

#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/guiinterface.h"
#include "model/game_state.h"
#include "model/ball.h"
#include "model/team.h"
#include <assert.h>
#include "ssl_referee.pb.h"


// Global static pointer used to ensure a single instance of the class.
Referee_Command  GameState::refereeCommand  = Referee_Command_HALT; //The current state of the game from RefComm
Referee_Command  GameState::previousCommand = Referee_Command_HALT; //The previous gamestate
char GameState::blueGoals       = 0;           //Number of scores yellow goals
char GameState::yellowGoals     = 0;           //Number of scores yellow goals
int  GameState::remainingTime   = 0;

/*******************************************************************/
/************************ Public Methods ***************************/
/*******************************************************************/


//! @brief From the RefComm, Returns the number of Blue goals
char GameState::getBlueGoals()
{
    return blueGoals;
}

//! @brief From the RefComm, Returns the number of Yellow goals
char GameState::getYellowGoals()
{
    return yellowGoals;
}

//! @brief Returns the remaining time in seconds
int GameState::getRemainingTime()
{
    return remainingTime;
}

//! @brief Returns the current game state, used by StrategyController
Referee_Command GameState::getRefereeCommand()
{
    return refereeCommand;
}


//! @brief Returns the last different game state before this one
Referee_Command GameState::getPreviousCommand()
{
    return previousCommand;
}


/*******************************************************************/
/************************ Private Methods **************************/
/*******************************************************************/

/* Called by VisionComm */
/* Don't overlook this function, it's more important than you think */
/*! @brief Used by VisionComm; The main shebang; calls the run functions of the
 * StrategyController and makes the project work. This also updates the GUI system. */
void GameState::notifyObservers()
{
    //std::cout << "at GameModel::notifyObservers()\n";
    Ball::setRobotWithBall();
    RobotTeam* t = RobotTeam::getTeam(ROBOT_TEAM_BLUE);
    if(t->isControlled()) t->controller->run();
    t = RobotTeam::getTeam(ROBOT_TEAM_YELLOW);
    if(t->isControlled()) t->controller->run();
}

/*! @brief Used by RefComm; Set the current game state (See RefComm reference)
 * @see RefComm
 * @see getGameState */
void GameState::setRefereeCommand(Referee_Command new_cmd)
{
    if(refereeCommand != new_cmd)
    {
        previousCommand = refereeCommand;
        refereeCommand = new_cmd;
        //signal new state to teams:
        for(int i=0; i<2; i++){
            if(RobotTeam::getTeam(i)->isControlled())
                RobotTeam::getTeam(i)->controller->signalNewCommand();
        }
    }
}


//! @brief Used by RefComm; Set the current game state (See RefComm reference)
void GameState::setTimeLeft(int time)
{
    remainingTime = time;
}

//! @brief Used by RefComm; Set the number of blue team goals
void GameState::setBlueGoals(char goals)
{
    blueGoals = goals;
}

//! @brief Used by RefComm; Set the number of yellow team goals
void GameState::setYellowGoals(char goals)
{
    yellowGoals = goals;
}














