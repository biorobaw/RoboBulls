#include <stddef.h>  // defines NULL
#include "include/config/team.h"
#include "include/config/model_averages.h"
#include "gamemodel.h"

// Global static pointer used to ensure a single instance of the class.
GameModel* GameModel::model = NULL;


GameModel::GameModel()
{
    gameState  = '\0';
}

void GameModel::setStrategyController(StrategyController *sc)
{
    this->sc = sc;
}

Robot* GameModel::getHasBall()
{
    return robotWithBall;
}

Robot* GameModel::findMyTeam(int id)
{
    return find(id, myTeam);
}

Robot* GameModel::findOpTeam(int id)
{
    return find(id, opTeam);
}

vector<Robot*>& GameModel::getOponentTeam()
{
    return opTeam;
}

vector<Robot *>& GameModel::getMyTeam()
{
    return myTeam;
}

Point GameModel::getBallPoint()
{
    return ballPoint;
}

unsigned char GameModel::getBlueGoals()
{
    return blueGoals;
}

unsigned char GameModel::getYellowGoals()
{
    return yellowGoals;
}

unsigned char GameModel::getRemainingTime()
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

bool GameModel::isNewCommand()
{
    return this->hasNewCommand;
}

Point GameModel::getPenaltyPoint()
{
#if TEAM == TEAM_BLUE
        return Point(2045, 22);
#else
        return Point(-2045, 22);
#endif
}

Point GameModel::getOpponentGoal()
{
#if TEAM == TEAM_BLUE
        return Point(3000,0);
#else
        return Point(-3000, 0);
#endif
}

Point GameModel::getMyGoal()
{
#if TEAM == TEAM_BLUE
        return Point(-3000, 0);
#else
        return Point(3000,0);
#endif
}

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


/*******************************************************************/
/************************ Private Methods **************************/
/*******************************************************************/


/* Called by VisionComm */
/**************************************************/
/* Don't overlook this function, it's more important
 * than you think
 */
void GameModel::notifyObservers()
{
    setRobotHasBall();
    sc->run();
}
/**************************************************/

/* Called by RefComm */
/* I don't think both RefComm and VisionComm should both
 * make the game run. This opens up the possibility for running the
 * loop twice, and possible interference. Before, this function also
 * caused the StrategyController to update if the state was different
 */
void GameModel::setGameState(char gameState)
{
    char lastGameState = this->gameState;
    if(lastGameState != gameState)
        hasNewCommand = true;
    this->gameState = gameState;
}

void GameModel::onCommandProcessed()
{
    this->hasNewCommand = false;
}


void GameModel::setBallPoint(Point bp)
{
#if MODEL_USE_AVERAGES
    ballPoint  = ballAverage.update(bp);
#else
    ballPoint = bp;
#endif
}


void GameModel::setRobotHasBall()
{
    static int lastSeenWithoutBallCount = 0;

    static auto ptIsInFrontOfRob = [](Robot* rob, const Point& pt) {
        Point robPos = rob->getRobotPosition();
        float robAngle = rob->getOrientation();
        float angleBetween = Measurments::angleBetween(robPos, pt);
        return Measurments::isClose(robAngle, angleBetween, 5*M_PI/12);
        };
    auto calculateHasBall = [&](Robot* rob) {
        if(rob == NULL)
            return false;
        if(Measurments::distance(rob->getRobotPosition(), ballPoint) > 300.0)
            return false;
        if(!ptIsInFrontOfRob(rob, ballPoint))
            return false;
        return true;
        };

    for(Robot* robot : myTeam)
        robot->hasBall = false;
    for(Robot* robot : opTeam)
        robot->hasBall = false;


    if(!calculateHasBall(this->robotWithBall))
    {
        ++lastSeenWithoutBallCount;
        if(lastSeenWithoutBallCount > 240) {
            lastSeenWithoutBallCount = 0;
            auto ballBot = std::find_if(myTeam.begin(), myTeam.end(), calculateHasBall);
            if(ballBot == myTeam.end()) {            //Not found in myTeam
                ballBot = std::find_if(opTeam.begin(), opTeam.end(), calculateHasBall);
                if(ballBot == opTeam.end()) {        //Not found in opTeam
                    this->robotWithBall = NULL;
                    return;
                }
            }
            this->robotWithBall = *ballBot;
        } else {
            if(robotWithBall != NULL) {
                this->robotWithBall->hasBall = true;
            }
        }
    } else {
        this->robotWithBall->hasBall = true;
    }
}

void GameModel::setTimeLeft(unsigned short time)
{
    remainingTime = time;
}

void GameModel::setBlueGoals(unsigned char goals)
{
    blueGoals = goals;
}

void GameModel::setYellowGoals(unsigned char goals)
{
    yellowGoals = goals;
}

Robot* GameModel::find(int id, std::vector<Robot*>& team)
{
    /* Often, the vision system (and also almost always on the simulator)
     * seems to report robots in order anyway. So first,
     * I think it would be reasonable to check if the team at that `id`
     * is actually that robot first.
     */
    try {
        if(team.at(id)->getID() == id)
            return team[id];
    }
    catch(...) {
    }

    for(Robot* rob : team)
    {
        if(rob->getID() == id)
            return rob;
    }

    return NULL;
}

/* Start of Averaging functions
 * The function setRobotUpdated is in place of the setMyTeam / setOponentTeam functions
 * due to how the vision system interacts with the GameModel--one robot is updated
 * at a time. The previous system copied the entire team vectors three times
 * to update one entry in it--for EACH robot on BOTH teams. This function
 * instead allows one robot to be updated at a time, and now their averages
 * can be efficiently computed individually as well.
 *
 * Because of the inefficient updating before, the averaging system would not function:
 * Consider the fact that the entire team vector was re-set each time one robot
 * was updated, for all robots. This means the average for each robot would erroneously
 * be made up of the same position multiple times. 2*MAX_ROBOTS times to be exact.
 *
 * The averaged positions for the robots and ball are then transparently available
 * from getRobotPosition()s and getBallPoint
 *
 * See VisionComm::updateInfo
 */
#if MODEL_USE_AVERAGES
static bool shouldAddAverageValue(const Point& averageValue, const Point& newValue) {
    return Measurments::distance(averageValue, newValue) < OUTLIER_POINT_TOLERANCE;
}
static bool shouldAddAverageValue(const float& averageValue, const float& newValue) {
    return Measurments::isClose(averageValue, newValue, OUTLIER_ANGLE_TOLERANCE);
}


template<typename T>
GameModel::AverageContainer<T>::AverageContainer()
    : numOutliers(MAX_OUTLIERS) //Important
    {
        values.resize(MAX_AVERAGES+1);
    }

template<typename T>
const T& GameModel::AverageContainer<T>::update(const T& newValue)
{
    if(shouldAddAverageValue(averageValue, newValue)) {
        /* A continuing average can be computed by first multiplying
         * the current average by the size, then adding the new element,
         * and finally dividing by the new size
         */
        numOutliers = 0;
        averageValue *= values.size();
        if(values.size() == MAX_AVERAGES) {
            averageValue -= values.front();
            values.pop_front();
        }
        values.push_back(newValue);
        averageValue += newValue;
        averageValue /= values.size();
    } else {
        if(++numOutliers >= MAX_OUTLIERS) {
            /* We've seen enough outliers in a row to
             * change our confidence on where the value is.
             */
            numOutliers = 0;
            values.clear();
            values.emplace_back(newValue);
            averageValue = newValue;
        }
    }
    return averageValue;    //Returns new average value
}
#endif

void GameModel::setRobotUpdated(Robot* robot, int whichTeam)
{
#if MODEL_USE_AVERAGES
    auto* currentAverages = &myTeamAverages;
    if(whichTeam != TEAM)
        currentAverages = &opTeamAverages;

    Point rawRobPoint = robot->getRobotPosition();
    float rawRobAngle = robot->getOrientation();
    Point newRobPoint;
    float newRobAngle;

    /* While the myTeam/opTeam vectors are not guaranteed to be stored in order
     * (i.e, ID i is at myTeam[i]), these average arrays are. They are initially
     * sized to hold MAX_ROBOTS, so no out-of-range errors occur. Also, there is
     * no iteration over these, so invalid entries are just ignored
     */
    newRobPoint = currentAverages->at(robot->getID()).first.update(rawRobPoint);
    newRobAngle = currentAverages->at(robot->getID()).second.update(rawRobAngle);

    robot->setRobotPosition(newRobPoint);
    robot->setOrientation(newRobAngle);
#else
    UNUSED_PARAM(robot);
    UNUSED_PARAM(whichTeam);
#endif
}
