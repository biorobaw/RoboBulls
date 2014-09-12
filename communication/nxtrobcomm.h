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
    void sendVelsThreeOmni(int left, int right, int back, int ID);
    void sendVelsFourOmni(int front_left, int front_right, int rear_left, int rear_right, int ID);
    void sendVelsLarge(std::vector<Robot*>&);
    void sendKick(int robotId);

private:
    void send(unsigned);
    void send(char*, int size);
    serialib Xbee;
};

#endif // NXTROBCOMM_H
