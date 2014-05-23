#ifndef ROBOT_H
#define ROBOT_H

#include "Utilities/point.h"
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

//    //*******************************************************
//    //Testing Shamsi's code
//    void setRightWheelVelocity(int);
//    void setLeftWheelVelocity(int);
//    //*******************************************************


    //gets
    Point getRobotPosition();
    float getOrientation();
    int getID();
    int getR();
    int getL();
    Behavior * getCurrentBeh();

//    //*******************************************************
//    //Testing Shamsi's code
//    int getRightWheelVelocity();
//    int getLeftWheelVelocity();
//    //*******************************************************

    stringstream& toString();

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

//    //*******************************************************
//    //Testing Shamsi's code
//    int right_wheel_vel;
//    int left_wheel_vel;
//    //*******************************************************

};

#endif // ROBOT_H
