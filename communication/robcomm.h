#ifndef ROBCOMM_H
#define ROBCOMM_H

#pragma once
#include "include/serialib.h"

class RobComm
{
public:
    static RobComm * getRobComm();
    virtual void sendVels(int leftVel, int rightVel, int robotId) = 0;
    virtual void sendKick(int robotId) = 0;

private:
    static RobComm * robComm;
};

#endif // ROBCOMM_H
