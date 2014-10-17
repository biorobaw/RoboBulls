#ifndef NXTROBCOMM_H
#define NXTROBCOMM_H

#pragma once

#include <stdio.h>
#include <vector>
#include <stdint.h>

#include "include/serialib.h"
#include "model/robot.h"
#include "utilities/measurments.h"
#include "robcomm.h"


class NXTRobComm : public RobComm
{
public:
     NXTRobComm();
    ~NXTRobComm();
    void sendVelsLarge(std::vector<Robot*>&);

    int gui_left_front;    //LF wheel velocity
    int gui_left_back;     //LB wheel velocity
    int gui_right_front;   //RF wheel velocity
    int gui_right_back;    //RB wheel velocity
    int gui_left;
    int gui_right;
    int gui_kick;
    int gui_dribble;

private:
    /* The standard communicaiton packet for each
     * robot type, contain all information nessecery
     * for communicating with the robots
     */
    struct packet_t {
        int8_t tilde;         //Always a tilde (~)
        int8_t id;            //Robot ID
        int8_t left_front;    //LF wheel velocity
        int8_t left_back;     //LB wheel velocity
        int8_t right_front;   //RF wheel velocity
        int8_t right_back;    //RB wheel velocity
        int8_t kick;          //Kick? 1/0
        int8_t chip_power;    //Chip kick power
        int8_t dribble_power; //Dribbler power
        int8_t dollar;        //Always a dollar ('$')
    };

    void send(unsigned);
    void send(char*, int size);
    serialib Xbee;
};

#endif // NXTROBCOMM_H
