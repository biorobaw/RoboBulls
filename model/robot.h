#ifndef ROBOT_H
#define ROBOT_H

#include "utilities/point.h"
#include "behavior/behavior.h"
#include <sstream>

class Behavior;

class Robot
{
public:
    Robot();

    //sets
    void setRobotPosition(Point);
    void setOrientation(float);
    void setID(int);
    void setR(float);
    void setL(float);
    void setCurrentBeh(Behavior *);


    //gets
    Point getRobotPosition();
    float getOrientation();
    int getID();
    int getR();
    int getL();
    Behavior * getCurrentBeh();

    std::string toString();

//    bool hasTheBall();

    unsigned kick;
    int count;
    bool hasBall;
    bool hasBeh;

private:
    Point robotPosition;
    float orientation; //orientation of the robot
    int id;
    float L, R; // used for robot's movements
    Behavior *currentBehavior;

};

#endif // ROBOT_H
