#ifndef VISIONCOMM_H
#define VISIONCOMM_H
#include <string>

//#include "gui/interface/gui_interface.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "messages_robocup_ssl_refbox_log.pb.h"

#include <QThread>
#include <QUdpSocket>
#include <QMutex>
#include "model/moving_object.h"
#include "model/constants.h"


class GameState;
class MyKalmanFilter;

namespace YAML {
    class Node;
}




/**
 * @brief The VisionComm class recieves information from the vision cameras
 * @author Origin. Narges Ghaedi, JamesW
 * @details Detects the robots and ball and puts each robot in the corresponding team
 * based on robot's color (Blue team/ Yellow team)
 */
class SSLVisionListener: public QObject
{
    Q_OBJECT
public:

    SSLVisionListener( YAML::Node* comm_node);
    static void copyState(GameState* state);

    ~SSLVisionListener();


public slots:
    void restart_socket();
private slots:
    void process_package();

private:

    static SSLVisionListener* instance;
    static QThread* thread;
    QUdpSocket* socket = new QUdpSocket(this);
    QMutex mutex;

    QString net_addr;
    int     net_port;

    MovingObject robots[2][MAX_ROBOTS_PER_TEAM];
    MovingObject ball;
    MyKalmanFilter* kfilter;        //! Kalman filter instance

    int num_frame_detected = 0; //! Number of frames detected by vision, used in algorithm to remove false positive detections
    int robot_detection_counts[2][MAX_ROBOTS_PER_TEAM]={{0}};  //! Number of detections of each blue robot
    int num_cameras = 4; // Currently we only support 2 or 4 cameras




    //! @brief Parses an SSL_DetectionFrame and fills out GameModel
    void recieveRobotTeam(const SSL_DetectionFrame& frame, int whichTeam);

    //! @brief Parses an SSL_DetectionFrame and fills out GameModel
    void recieveBall(const SSL_DetectionFrame& frame);

    // checks if coordinate (x,y) is within the cam's field of view
    bool isGoodDetection(float x, float y, int cam);

    double time_stamp = 0;

};

#endif // VISIONCOMM_H


