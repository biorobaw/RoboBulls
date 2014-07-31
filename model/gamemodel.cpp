#include "gamemodel.h"
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

void GameModel::setHasBall()
{
/* 	static auto ballDetermineFn = [](Robot* rob) {
		rob->hasBall = Measurments::isClose(rob->getRobotPosition(), getBallPoint(), 30.00);
		};
	std::for_each(myTeam.begin(), myTeam.end(), ballDetermineFn);
	std::for_each(opTeam.begin(), opTeam.end(), ballDetermineFn); */
	
    for (vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
    {
        if (Measurments::isClose((*it)->getRobotPosition(), getBallPoint(), 30.00))
        {
            (*it)->hasBall=true;
            cout << (*it)->getID() << " has the ball!!!!!"<<endl;
        }
        else
        {
            (*it)->hasBall=false;
        }
    }
    for (vector<Robot*>::iterator it = opTeam.begin(); it != opTeam.end(); it++)
    {
        if (Measurments::isClose((*it)->getRobotPosition(), getBallPoint(), 30.00))
        {
            (*it)->hasBall=true;
            cout << (*it)->getID() << " has the ball!!!!!"<<endl;
        }
        else
        {
            (*it)->hasBall=false;
        }
    }
}


const vector<Robot*>& GameModel::getOponentTeam()
{
    return opTeam;
}

const vector<Robot*>& GameModel::getMyTeam()
{
    return myTeam;
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

Point GameModel::getPenaltyPoint()
{
    if (TEAM == 0)
        return Point(2045, 22);
    else
        return Point(-2045, 22);
}

Point GameModel::getOpponentGoal(){
    if (TEAM == 1)
        return Point(-3000, 0);
    else
        return Point(3000,0);
}

Point GameModel::getMyGoal(){
    if (TEAM == 0)
        return Point(-3000, 0);
    else
        return Point(3000,0);
}

/**
 * @brief GameModel::find : find robot in team, compare ids, return robot if ids are same
 * @param detected_id : The ID to look for
 * @param team : The team to look in
 * @return : The robot with id detected_id, or NULL if not found.
 */
Robot * GameModel::find(int detected_id, const vector<Robot *>& team)
{
    Robot *rob = NULL;

    for(vector<Robot*>::const_iterator it = team.cbegin(); it != team.cend(); it++)
    {
        if ((*it)->getID() == detected_id)
        {
            // TODO: Compare with current's confidence - low priority
            rob = (*it);
            break;
        }
    }
    return rob;
}

//Robot * GameModel::getHasBall()
//{

//}


std::string GameModel::toString()
{
	stringstream myString;

	myString << "Ball Position: " << ballPoint.toString() <<endl;

	myString<<"\nMy Team Robots: \n";
	for (vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
	{
		myString << "\t" << (*it)->toString()<<endl;
	}

	myString<<"\nOponent Team Robots: \n";
	for (vector<Robot*>::iterator it = opTeam.begin(); it != opTeam.end(); it++)
	{
		myString << "\t" << (*it)->toString() <<endl;
	}

	return myString.str();
}


