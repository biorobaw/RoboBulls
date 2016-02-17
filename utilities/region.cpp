#include <sstream>
#include <cmath>
#include "region.h"
#include "include/config/globals.h"

/**************************************************/

Region::Region()
{
    minX = maxX = minY = maxY = 0;
}

Region::Region(float X1,float X2,float Y1,float Y2)
{
    minX = std::min(X1,X2);
    maxX = std::max(X1,X2);
    minY = std::min(Y1,Y2);
    maxY = std::max(Y1,Y2);
}


bool Region::contains(const Point& point)
{
    return (point.x >= minX && point.x < maxX && point.y >= minY && point.y < maxY);
}

//See http://stackoverflow.com/questions/1585525/
bool Region::containsLine(const Point& p0, const Point& p1)
{
    float x1 = p0.x;
    float x2 = p1.x;
    float y1 = p0.y;
    float y2 = p1.y;

    //Completely outside
    if ((x1 <= minX && x2 <= minX) ||
        (y1 <= minY && y2 <= minY) ||
        (x1 >= maxX && x2 >= maxX) ||
        (y1 >= maxY && y2 >= maxY))
        return false;

    float m = (y2 - y1) / (x2 - x1);

    float y = m * (minX - x1) + y1;
    if (y > minY && y < maxY) return true;

    y = m * (maxX - x1) + y1;
    if (y > minY && y < maxY) return true;

    float x = (minY - y1) / m + x1;
    if (x > minX && x < maxX) return true;

    x = (maxY - y1) / m + x1;
    if (x > minX && x < maxX) return true;

    return false;
}

int Region::numOfRobots()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getOponentTeam())
    {
        if(Region::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }
    for (Robot * robot : gm->getMyTeam())
    {
        if(Region::contains(robot->getPosition()))
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
        if(Region::contains(robot->getPosition()))
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
        if(Region::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }

    return number_of_robots;
}

int Region::numOfRobots(std::vector<Robot*>& ignoreOpponents, std::vector<Robot*>& ignoreTeammates)
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robotAdd : gm->getOponentTeam())
    {
        if( Region::contains(robotAdd->getPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreOpponents)
        {
            if( Region::contains(robotSubtract->getPosition())
            &&  robotSubtract->getID() == robotAdd->getID())
            {
                number_of_robots--;
            }
        }
    }

    for (Robot * robotAdd : gm->getMyTeam())
    {
        if( Region::contains(robotAdd->getPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreTeammates)
        {
            if( Region::contains(robotSubtract->getPosition())
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

std::string Region::toString()
{
    std::stringstream ss;

    ss << "minX, maxX, minY, maxY: "
    << minX << ", " << maxX << ", "
    << minY << ", " << maxY << std::endl;

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

Region Region::lowerLeftRegion = Region(-FIELD_LENGTH,0,-FIELD_WIDTH,0);
Region Region::topLeftRegion = Region(-FIELD_LENGTH,0,FIELD_WIDTH,0);
Region Region::topRightRegion = Region(0,FIELD_LENGTH,0,FIELD_WIDTH);
Region Region::lowerRightRegion = Region(0,FIELD_LENGTH,0,-FIELD_WIDTH);
Region Region::goalLeftRegion = Region(-FIELD_LENGTH,-FIELD_LENGTH+GOAL_LENGTH,-GOAL_WIDTH,GOAL_WIDTH);
Region Region::goalRightRegion = Region(FIELD_LENGTH-GOAL_LENGTH, FIELD_LENGTH, -GOAL_WIDTH, GOAL_WIDTH);


