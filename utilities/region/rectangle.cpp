#include "rectangle.h"

Rectangle::Rectangle()
{
}


Rectangle::Rectangle(float X1,float Y1,float X2,float Y2)
{
    min_x = std::min(X1,X2);
    max_x = std::max(X1,X2);
    min_y = std::min(Y1,Y2);
    max_y = std::max(Y1,Y2);
}

bool Rectangle::contains(const Point& point)
{
    return point.x >= min_x && point.x <= max_x && point.y >= min_y && point.y <= max_y;
}

//See http://stackoverflow.com/questions/1585525/
bool Rectangle::containsLine(const Point& p0, const Point& p1)
{
    float x1 = p0.x;
    float x2 = p1.x;
    float y1 = p0.y;
    float y2 = p1.y;

    //Completely outside
    if ((x1 <= min_x && x2 <= min_x) ||
        (y1 <= min_y && y2 <= min_y) ||
        (x1 >= max_x && x2 >= max_x) ||
        (y1 >= max_y && y2 >= max_y))
        return false;

    float m = (y2 - y1) / (x2 - x1);

    float y = m * (min_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    y = m * (max_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    float x = (min_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    x = (max_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    return false;
}

int Rectangle::numOfRobots()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getOppTeam().getRobots())
    {
        if(Rectangle::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }
    for (Robot * robot : gm->getMyTeam().getRobots())
    {
        if(Rectangle::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }
    return number_of_robots;
}

int Rectangle::numOfTeammates()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getMyTeam().getRobots())
    {
        if(Rectangle::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }
    return number_of_robots;
}

int Rectangle::numOfOpponents()
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robot : gm->getOppTeam().getRobots())
    {
        if(Rectangle::contains(robot->getPosition()))
        {
           number_of_robots++;
        }
    }

    return number_of_robots;
}

int Rectangle::numOfRobots(std::vector<Robot*>& ignoreOpponents, std::vector<Robot*>& ignoreTeammates)
{
    GameModel* gm = GameModel::getModel();

    int number_of_robots = 0;

    for (Robot * robotAdd : gm->getOppTeam().getRobots())
    {
        if( Rectangle::contains(robotAdd->getPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreOpponents)
        {
            if( Rectangle::contains(robotSubtract->getPosition())
            &&  robotSubtract->getID() == robotAdd->getID())
            {
                number_of_robots--;
            }
        }
    }

    for (Robot * robotAdd : gm->getMyTeam().getRobots())
    {
        if( Rectangle::contains(robotAdd->getPosition()) )
        {
           number_of_robots++;
        }
        for (Robot * robotSubtract : ignoreTeammates)
        {
            if( Rectangle::contains(robotSubtract->getPosition())
            &&  robotSubtract->getID() == robotAdd->getID())
            {
                number_of_robots--;
            }
        }
    }

    //cout << number_of_robots << endl;
    return number_of_robots;
}

Point Rectangle::centre()
{
    return Point((min_x+max_x)/2,(min_y+max_y)/2);
}

std::string Rectangle::toString()
{
    std::stringstream ss;

    ss << "minX, maxX, minY, maxY: "
    << min_x << ", " << max_x << ", "
    << min_y << ", " << max_y << std::endl;

    return ss.str();
}

void Rectangle::draw()
{
    for(int x = min_x; x <= max_x; x+=10)
        for(int y = min_y; y <= max_y; y+=10)
            if(this->contains(Point(x,y)))
                GuiInterface::getGuiInterface()->drawPoint(Point(x,y));
}
