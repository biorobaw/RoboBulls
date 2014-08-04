#ifndef ROBCOMM_H
#define ROBCOMM_H

#pragma once
#include "include/serialib.h"
#include <vector>

class Robot;

/*
 * RobComm
 * It is used to send signal to robots and control the robots
 * Narges Ghaedi
 */
class RobComm
{
public:
    static RobComm * getRobComm();
    //Sends given velocities to left and right wheels a robot recognized by robot ID
    virtual void sendVels(int leftVel, int rightVel, int robotId) = 0;
    virtual void sendVelsLarge(std::vector <Robot*>)=0;
    //Sends a kick signal to a robot recognized by robot ID
    virtual void sendKick(int robotId) = 0;

private:
    static RobComm * robcomm;
};

#endif // ROBCOMM_H
