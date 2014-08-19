#ifndef ROBCOMM_H
#define ROBCOMM_H

#pragma once
#include "include/serialib.h"
#include <vector>

class Robot;

/*
 * RobComm
 * It is used to send signal to robots
 * Narges Ghaedi
 */
class RobComm
{
public:
    static RobComm * getRobComm();
    virtual void sendVelsLarge(std::vector <Robot*>)=0;
    virtual void sendVelsThreeOmni(int left,int right, int back, int ID)=0;
    virtual void sendKick(int robotId) = 0;
    //virtual void sendVels(int leftVel, int rightVel, int robotId) = 0;

private:
    static RobComm * robcomm;
};

#endif // ROBCOMM_H
