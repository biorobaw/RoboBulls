#ifndef ROBCOMM_H
#define ROBCOMM_H

#pragma once
#include "include/serialib.h"

class robComm
{
    serialib Xbee;
public:
    robComm();
    ~robComm();

    void sendVels(int leftVel, int rightVel, int robotId);
    void sendKick(int robotId);

    static robComm * getnxtbee();
private:
    static robComm * nxtbee;
    void send(unsigned);
    void send(char*, int size);
};

#endif // ROBCOMM_H
