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
    virtual void sendVelsLarge(std::vector<Robot*>&)=0;

private:
    static RobComm * robcomm;
};

#endif // ROBCOMM_H
