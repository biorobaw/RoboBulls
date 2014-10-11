#ifndef ROBOT_H
#define ROBOT_H

#include "utilities/point.h"
#include "behavior/behavior.h"
#include "include/config/robot_types.h"
class Behavior;

/**
 * @brief The Robot class
 * The robot class includes the required information about robot
 * such position, orientation, id, and etc.
 * you can assign these information to robot or access them when required
 */
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
    void setB(float);
    void setLF(float left_forward);
    void setRF(float right_forward);
    void setLB(float left_backward);
    void setRB(float right_backward);
    void setKick(bool);
    void setDrible(bool);
    void setCurrentBeh(Behavior *);
    

    //gets
    Point getRobotPosition();
    float getOrientation();
    int   getID();
    int   getR();
    int   getL();
    int   getB();
    int   getLF();
    int   getRF();
    int   getLB();
    int   getRB();
    int   getKick();
    bool  getDrible();
    Behavior* getCurrentBeh();
    robotType type();
    
    void clearCurrentBeh();
    std::string toString();

    bool hasBall;
    bool hasBeh;

//    Behavior * currentBehavior; // TEST

private:
    Point robotPosition;
    float orientation; //orientation of the robot
    int id;
    float LF, RF, LB, RB; // used for robot's movements
    Behavior * currentBehavior;
    bool kick;
    bool drible;

};

#endif // ROBOT_H
