#ifndef VISIONCOMM_H
#define VISIONCOMM_H

#include "include/netraw.h"
#include <string>
#include <iostream>
#include <QtCore>
#include "Model/gamemodel.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"
#include "Robot/robot.h"
#include "Measure/point.h"

using namespace std;

/**
 * @brief CONF_THRESHOLD sets the minimum confidence to consider the ball as detected.
*/
const float CONF_THRESHOLD = 0;

/**
 * @brief DISCARD_RATE sets the rate in which packets are discarded. only 1/DISCARD_RATE packages are processed.
 */
const int DISCARD_RATE = 1;

class VisionComm: public QThread
{
public:
    VisionComm(GameModel *gm);
    ~VisionComm();


    //communicating with reciver
    bool receive();

    //void VisionThread();
    void run();

protected:
    GameModel *gamemodel;
    SSL_WrapperPacket packet;
    RoboCupSSLClient client;
    int count;
};

#endif // VISIONCOMM_H


