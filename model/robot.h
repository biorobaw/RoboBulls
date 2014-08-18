#ifndef ROBOT_H
#define ROBOT_H

#include "utilities/point.h"
#include "behavior/behavior.h"
#include <sstream>

class Behavior;

/**
 * @brief The Robot class
 * The robot class includes the required infromation about robot
 * such position, orientation, id, and etc.
 * you can asssign these information to robot or access them when required
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
    void setKick(int);
    void setDrible(bool);
    void setCurrentBeh(Behavior *);
    

    //gets
    Point getRobotPosition();
    float getOrientation();
    int getID();
    int getR();
    int getL();
    int getKick();
    bool getDrible();
    Behavior * getCurrentBeh();
	
	void clearCurrentBeh();
    std::string toString();


    int count;
    bool hasBall;
    bool hasBeh;

private:
    Point robotPosition;
    float orientation; //orientation of the robot
    int id;
    float L, R; // used for robot's movements
    Behavior *currentBehavior;
    unsigned kick;
    bool drible;

};

#endif // ROBOT_H
