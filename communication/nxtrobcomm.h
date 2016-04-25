#ifndef NXTROBCOMM_H
#define NXTROBCOMM_H
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include "include/serialib.h"
#include "model/robot.h"
#include "utilities/measurements.h"
#include "robcomm.h"

/*! @brief RobComm to send packets to real Robots
 * @details The NXTRobComm is a communication module for sending commands over
 * an Xbee to robots on the field. This is contrast to SimRobComm which
 * sends grSim packets. */

class NXTRobComm : public RobComm
{
public:
     NXTRobComm();
    ~NXTRobComm();
    void sendVelsLarge(std::vector<Robot*>&);

private:
    /*! @brief Field robot communication packet
     * @details The standard communicaiton packet for each
     * robot type, containing all information nessecery
     * for communicating with the robots
     */
    struct packet_t {
        int8_t tilde;         //!<"Tilde", '~ 'for NXT, 255 for Arduino
        int8_t id;            //!<Robot ID
        int8_t left_front;    //!<LF wheel velocity
        int8_t left_back;     //!<LB wheel velocity
        int8_t right_front;   //!<RF wheel velocity
        int8_t right_back;    //!<RB wheel velocity
        int8_t kick;          //!<Kick? 1/0 for Arduino, 'k'/0 for NXT
        int8_t chip_power;    //!<Chip kick power
        int8_t dribble_power; //!<Dribbler power
        int8_t dollar;        //!<Dollar", '$' for NXT, 255 for Arduino
    };

    serialib Xbee;            //!<seriallib Interface to /dev/xbee
};

#endif // NXTROBCOMM_H
