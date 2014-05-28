#ifndef NXTROBCOMM_H
#define NXTROBCOMM_H

#pragma once
#include "include/serialib.h"

#include "robcomm.h"

class NXTRobComm : public RobComm
{
public:
    NXTRobComm();
    ~NXTRobComm();

    void sendVels(int leftVel, int rightVel, int robotId);
    void sendKick(int robotId);
private:
    void send(unsigned);
    void send(char*, int size);
    serialib Xbee;
};

#endif // NXTROBCOMM_H
