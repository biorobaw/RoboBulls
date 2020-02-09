#ifndef VISIONCOMM_H
#define VISIONCOMM_H
#include <string>
#include <QtCore/QThread>
#include "ssl-vision/robocup_ssl_client.h"
#include "model/game_state.h"
#include "model/robot.h"
#include "kfball.h"
#include "gui/guiinterface.h"
#include "yaml-cpp/yaml.h"
#include <atomic>


#define VEL_HIST_SIZE 20

//! @brief Sets the minimum confidence to consider a ball reading as valid
//#if SIMULATED
//const float CONF_THRESHOLD_BALL = 0.9;
//#else
const float CONF_THRESHOLD_BALL = 0.8;//CNM
//#endif

//! @brief Sets the minimum confidence to consider a robot as a valid reading
const float CONF_THRESHOLD_BOTS = 0.90;

/**
 * @brief The VisionComm class recieves information from the vision cameras
 * @author Origin. Narges Ghaedi, JamesW
 * @details Detects the robots and ball and puts each robot in the corresponding team
 * based on robot's color (Blue team/ Yellow team)
 */
class VisionComm: public QThread
{
public:
    VisionComm( YAML::Node comm_node, int _side);
    ~VisionComm();

    //! @brief Recieves an SSL_WrapperPacket and fills in the GameModel information
    void receive();
    
    //! @brief QThread run method
    void run();
    void close();

protected:

    std::atomic_bool stop_listening = { false };

    //! @brief Parses an SSL_DetectionFrame and fills out GameModel
    void recieveRobotTeam(const SSL_DetectionFrame& frame, int whichTeam);

    //! @brief Parses an SSL_DetectionFrame and fills out GameModel
    void recieveBall(const SSL_DetectionFrame& frame);

    //! @brief Updates GameModel information to fill out a robot
    void receiveRobot(const SSL_DetectionRobot& robot, int detectedTeamColor);

    //! @brief Recieves a new packet only if `ms_limit` ms has passed since last call
    void receiveIfMSPassed(int ms_limit);

    //! @brief Returns true if we are using four or two cameras.
    bool isFourCameraMode();

    SSL_WrapperPacket packet;       //! Packet recieved by client
    SSL_DetectionFrame frames[4];   //! Accumulates frames
    bool frames_state[4]{false};    //! Marks whether frames are dirty or clean
    RoboCupSSLClient * client;      //! client to receive packets
    int resetFrames = 0;            //! Frames passed up remove all potential robot detections
    int totalframes = 0;            //! Total frames passed since start
    int rob_readings[2][MAX_ROBOTS]={{0}};  //! Number of detections of each blue robot
    timeval lastRecvTime;           //! When did we last receive a packet? Used to not recieve every one
    bool FOUR_CAMERA_MODE = false;    //! Are we in four-camera mode (true)? Or Two-camera mode?

    KFBall* kfilter;                //! Kalman filter instance
    bool kfilter_init = false;      //! Has the kalman filter been initialized with the first measurement?
    KFBall::Vector u;               //! Stores the previous state estimate of the kalman filter

    Point prev_k_b_pos;
    Point vel_hist[VEL_HIST_SIZE];
    int i_vel_hist = 0;


    int side = 0;

};

#endif // VISIONCOMM_H


