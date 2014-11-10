
#ifndef VISIONCOMM_H
#define VISIONCOMM_H

#include <string>
#include <iostream>
#include <QtCore>
#include <string>
#include "include/netraw.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"
#include "model/gamemodel.h"
#include "model/robot.h"
using namespace std;

/**
 * @brief CONF_THRESHOLD sets the minimum confidence to consider the ball as detected.
*/

const float CONF_THRESHOLD_BALL = 0.6;
const float CONF_THRESHOLD_BOTS = 0.8;

/**
 * @brief DISCARD_RATE sets the rate in which packets are discarded. only 1/DISCARD_RATE packages are processed.
 */
const int DISCARD_RATE = 1;

/**
 * @brief The VisionComm class
 * Detects the robots and ball and puts each robot in the corresponding team
 * based on robot's color (Blue team/ Yellow team)
 * Narges Ghaedi
 */
class VisionComm: public QThread
{
public:
    VisionComm(GameModel *gm);
    ~VisionComm();

    Robot detection();

    //communicating with reciver
    bool receive();

    //void VisionThread();
    void run();

    void updateInfo(const SSL_DetectionRobot& robot, int detectedTeamColor);

protected:
    GameModel *gamemodel;
    SSL_WrapperPacket packet;
    RoboCupSSLClient * client;
    int count;
    int frames = 0;
    int blue_rob[10]={0};
    int yellow_rob[10]={0};
};

#endif // VISIONCOMM_H


