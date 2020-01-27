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
#include <assert.h>



// Global static pointer used to ensure a single instance of the class.
char   GameState::state           = '\0';        //The current state of the game from RefComm
char   GameState::previousState   = '\0';      //The previous gamestate
char   GameState::blueGoals       = 0;           //Number of scores yellow goals
char   GameState::yellowGoals     = 0;           //Number of scores yellow goals
short  GameState::remainingTime   = 0;

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
short GameState::getRemainingTime()
{
    return remainingTime;
}

//! @brief Returns the current game state, used by StrategyController
char GameState::getState()
{
    return state;
}


//! @brief Returns the last different game state before this one
char GameState::getPreviousState()
{
    return previousState;
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
    Team* t = Team::getTeam(TEAM_BLUE);
    if(t->isControlled()) t->controller.run();
    t = Team::getTeam(TEAM_YELLOW);
    if(t->isControlled()) t->controller.run();
}

/*! @brief Used by RefComm; Set the current game state (See RefComm reference)
 * @see RefComm
 * @see getGameState */
void GameState::setGameState(char newState)
{
    if(state != newState)
    {
        previousState = state;
        state = newState;
        //signal new state to teams:
        for(int i=0; i<2; i++){
            if(Team::getTeam(i)->isControlled())
                Team::getTeam(i)->controller.signalNewCommand();
        }
    }
}


//! @brief Used by RefComm; Set the current game state (See RefComm reference)
void GameState::setTimeLeft(short time)
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














