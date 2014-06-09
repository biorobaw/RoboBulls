#include "robot.h"

Robot::Robot()
{
    hasBeh = false;
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
    hasBeh = true;
}

void Robot::clearCurrentBeh()
{
    hasBeh = false;
    delete currentBehavior;
    this->currentBehavior = NULL;
}

Point Robot::getRobotPosition()
{
    return robotPosition;
}

float Robot::getOrientation()
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

Behavior * Robot::getCurrentBeh()
{
    return currentBehavior;
}

std::string Robot::toString()
{
    stringstream ss;

    ss << "\t" << getRobotPosition().toString() << "\t ID: " << getID();


    return ss.str();

}

