#ifndef ROBCOMM_H
#define ROBCOMM_H

#pragma once
#include "include/serialib.h"

/*
 * RobComm
 * It is used to send signal to robots and control the robots
 * Narges Ghaedi
 */
class robComm
{
    serialib Xbee;
public:
    robComm();
    ~robComm();

    //Sends given velocities to left and right wheels a robot recognized by robot ID
    void sendVels(int leftVel, int rightVel, int robotId);

    //Sends a kick signal to a robot recognized by robot ID
    void sendKick(int robotId);

    static robComm * getnxtbee();
private:
    static robComm * nxtbee;
    void send(unsigned);
    void send(char*, int size);
};

#endif // ROBCOMM_H
