#include "robot.h"

Robot::Robot()
{
}

void Robot::setRobotPosition(Point rbtPoint)
{
    robotPosition = rbtPoint;
}

void Robot:: setOrientation(float ornt)
{
    orientation = ornt;
}

void Robot::setID(int ID)
{
    id = ID;
}

void Robot::setL(float left)
{
    L = left;
}

void Robot::setR(float right)
{
    R= right;
}

void Robot::setCurrentBeh(Behavior *currentBeh)
{
    currentBehavior = currentBeh;
}




Point Robot::getRobotPosition()
{
    return robotPosition;
}

int Robot::getOrientation()
{
    return orientation;
}

int Robot::getID()
{
    return id;
}

int Robot::getL()
{
    return L;
}

int Robot::getR()
{
    return R;
}

Behavior Robot::getCurrentBeh()
{
    return *currentBehavior;
}

