#include <sstream>
#include <cmath>
#include "region.h"

Region::Region()
{
    minX = maxX = minY = maxY = 0;
}

Region::Region(float X1,float X2,float Y1,float Y2)
{
    minX = min(X1,X2);
    maxX = max(X1,X2);
    minY = min(Y1,Y2);
    maxY = max(Y1,Y2);
}


bool Region::contains(const Point& point)
{
    if (point.x >= minX && point.x < maxX && point.y >= minY && point.y < maxY)
        return true;
    else
        return false;
}

int Region::numOfRobots()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getOponentTeam())
    {
        if(Region::contains(robot->getRobotPosition()))
        {
           number_of_robots++;
        }
    }
    for (Robot * robot : gm->getMyTeam())
    {
        if(Region::contains(robot->getRobotPosition()))
        {
           number_of_robots++;
        }
    }
    return number_of_robots;
}

int Region::numOfTeammates()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getMyTeam())
    {
        if(Region::contains(robot->getRobotPosition()))
        {
           number_of_robots++;
        }
    }
    return number_of_robots;
}

int Region::numOfOpponents()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getOponentTeam())
    {
        if(Region::contains(robot->getRobotPosition()))
        {
           number_of_robots++;
        }
    }

    return number_of_robots;
}

int Region::numOfRobots(vector<Robot *> ignoreOpponents, vector<Robot *> ignoreTeammates)
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robotAdd : gm->getOponentTeam())
    {
        if( Region::contains(robotAdd->getRobotPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreOpponents)
        {
            if( Region::contains(robotSubtract->getRobotPosition())
            &&  robotSubtract->getID() == robotAdd->getID())
            {
                number_of_robots--;
            }
        }
    }

    for (Robot * robotAdd : gm->getMyTeam())
    {
        if( Region::contains(robotAdd->getRobotPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreTeammates)
        {
            if( Region::contains(robotSubtract->getRobotPosition())
            &&  robotSubtract->getID() == robotAdd->getID())
            {
                number_of_robots--;
            }
        }
    }

    //cout << number_of_robots << endl;
    return number_of_robots;
}

Point Region::centre()
{
    Point centre = Point((minX+maxX)/2,(minY+maxY)/2);
    return centre;
}

string Region::toString()
{
    stringstream ss;

    ss << "minX, maxX, minY, maxY: "
    << minX << ", " << maxX << ", "
    << minY << ", " << maxY << endl;

    return ss.str();
}

bool Region::goalScored()
{
    GameModel * gm = GameModel::getModel();

    Point oppGoalPoint = gm->getOpponentGoal() ;
    Point myGoalPoint = gm->getMyGoal() ;
    Point bp = gm->getBallPoint();

    float oppGoalDirection = (oppGoalPoint.x)/abs(oppGoalPoint.x);
    float myGoalDirection = -oppGoalDirection;

    Region oppGoalRegion = Region(oppGoalPoint.x, oppGoalPoint.x+oppGoalDirection*160, 330, -330);
    Region myGoalRegion = Region(myGoalPoint.x, myGoalPoint.x+myGoalDirection*160, 330, -330);

    if(oppGoalRegion.contains(bp) || myGoalRegion.contains(bp))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Region::setStrategicValue(int value)
{
    strategic_value = value;
}

int Region::getStrategicValue()
{
    return strategic_value;
}

Region Region::lowerLeftRegion = Region(-3000,0,-2000,0);
Region Region::topLeftRegion = Region(-3000,0,2000,0);
Region Region::topRightRegion = Region(0,3000,0,2000);
Region Region::lowerRightRegion = Region(0,3000,0,-2000);
Region Region::goalLeftRegion = Region(-3000,2500,-700,700);
Region Region::goalRightRegion = Region(2500, 3000, -700, 700);


