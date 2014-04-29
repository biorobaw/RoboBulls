#include "gamemodel.h"
#include "Robot/robot.h"
#include <stddef.h>  // defines NULL

const float CONF_THRESHOLD=0.0;
// Global static pointer used to ensure a single instance of the class.
GameModel* GameModel::model = NULL;

GameModel::GameModel()
{
   //this->sc = sc;
    gameState = '\0';
}


void GameModel::setOponentTeam(vector<Robot*> team )
{
    opTeam = team;
    sc->gameModelUpdated();
}

void GameModel::setMyTeam(vector <Robot*> team)
{
    myTeam = team;
    sc->gameModelUpdated();

}

void GameModel::setXBall(float x)
{
    x_ballPosition = x;
}

void GameModel::setYball(float y)
{
    y_ballPosition = y;
}

void GameModel::setBallPoint(Point bp)
{
    ballPoint = bp;
}

void GameModel::setGameState(char gameState)
{
    char previousState = this->gameState;
    this->gameState = gameState;

    if (previousState != gameState)
    {
        sc->gameModelUpdated();
    }

}

void GameModel::setStrategyController(StrategyController *sc)
{
    this->sc = sc;
}

vector<Robot*> GameModel::getOponentTeam()
{
    return opTeam;
}

vector<Robot*> GameModel::getMyTeam()
{
    return myTeam;
}

float GameModel::getXBall()
{
    return x_ballPosition;
}

float GameModel::getYBall()
{
    return y_ballPosition;
}

Point GameModel::getBallPoint()
{
    return ballPoint;
}


int GameModel::getBlueGoals()
{
    return blueGoals;
}

int GameModel::getYellowGoals()
{
    return yellowGoals;
}

float GameModel::getRemainingTime()
{
    return remainingTime;
}

char GameModel::getGameState()
{
    return gameState;
}

GameModel * GameModel::getModel()
{
    if (model == NULL)
        model = new GameModel();

    return model;
}

/**
 * @brief GameModel::find : find robot in team, compare ids, return robot if ids are same
 * @param robot
 * @param team
 * @return
 */
Robot * GameModel::find(int detected_id, vector<Robot*> team)
{
    Robot *rob = NULL;
//    cout << "finding robot" << endl;
    for(vector<Robot*>::iterator it = team.begin(); it != team.end(); it++)
    {
        if ((*it)->getID() == detected_id)
        {
            // TODO: Compare with current's confidence - low priority
            rob = (*it);
//            cout<<" id = \n" <<(*it)->getID()<<endl;
        }
    }

//    if (rob == NULL)
//        cout <<"Robot was NULL"<<endl;
//    else
//        cout << "Robot was not NULL" << endl;

    return rob;
}


 string GameModel::toString()
 {
//     stringstream myString;
//     cout<<"Trying to print ball position!" <<endl;
//     myString << "Ball Position: " << ballPoint.toString() <<endl;
     cout << "Ball Position: ";
     cout<< ballPoint.toString() <<endl;

     cout<<"\nMy Team Robots: \n";
     for (vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
     {
         cout << (*it)->getRobotPosition().toString()<<endl;
     }

     cout<<"\nOponent Team Robots: \n";
     for (vector<Robot*>::iterator it = opTeam.begin(); it != opTeam.end(); it++)
     {
         cout << (*it)->getRobotPosition().toString()<<endl;
     }
     return ""; //myString.str();
//     cout<< "Ball Position: " << ballPoint.toString() <<endl;
 }


