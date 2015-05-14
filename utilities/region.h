#ifndef REGION_H
#define REGION_H


#include "point.h"
#include "model/gamemodel.h"
#include "model/robot.h"

class Region
{
public:

    Region();

    Region(float X1,float X2,float Y1,float Y2);

    bool contains(const Point&);

    int numOfRobots();

    int numOfOpponents();

    int numOfTeammates();

    //Excludes the robots in the vectors passed passed in.
    int numOfRobots(vector <Robot *> ignoreOpponents,vector <Robot *>ignoreTeammates);

    Point centre();

    //Returns true if the ball is in either goal-post.
    static bool goalScored();

    string toString();

    void setStrategicValue(int);

    int getStrategicValue();

public:
    static Region lowerLeftRegion;
    static Region topLeftRegion;
    static Region topRightRegion;
    static Region lowerRightRegion;
    static Region goalLeftRegion;
    static Region goalRightRegion;

private:
    float minX, maxX, minY, maxY;
    int strategic_value;
};

#endif // REGION_H


