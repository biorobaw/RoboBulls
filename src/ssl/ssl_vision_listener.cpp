#include <cmath>

#include "ssl_vision_listener.h"
#include "model/game_state.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"

#include <QThread>
#include "utilities/my_yaml.h"
#include "utilities/my_kalman_filter.h"


// Sets the minimum confidence to consider a ball reading as valid
#define CONF_THRESHOLD_BALL 0.8
#define ROBOT_CONFIDENCE_THRESHOLD 0.90

SSLVisionListener* SSLVisionListener::instance = nullptr;
QThread* SSLVisionListener::thread = nullptr;

SSLVisionListener::SSLVisionListener( YAML::Node* comm_node)
{

    qInfo() << "--VISION";
    qInfo() << "        VISION_ADDR :" << (*comm_node)["VISION_ADDR"].Scalar().c_str();
    qInfo() << "        VISION_PORT :" << (*comm_node)["VISION_PORT"].Scalar().c_str();
    qInfo() << "        FOUR_CAMERA :" << (*comm_node)["FOUR_CAMERA"].Scalar().c_str();

    net_addr = (*comm_node)["VISION_ADDR"].as<string>().c_str();
    net_port = (*comm_node)["VISION_PORT"].as<int>();
    num_cameras = (*comm_node)["FOUR_CAMERA"].as<bool>() ? 4 : 2;

    kfilter = new MyKalmanFilter();

    restart_socket();

    thread = new QThread;
    this->moveToThread(thread);
    connect(thread, &QThread::finished, this, &QObject::deleteLater);

    instance = this;
    qInfo() << "--Vision DONE - thread: " << thread;
    thread->start();


}

void SSLVisionListener::restart_socket(){
    if(socket->isOpen()) socket->close();

    if(!socket->bind(QHostAddress::AnyIPv4, net_port, QUdpSocket::ShareAddress)){
        qFatal("ERROR: could not bind refbox port %d", net_port);
    }

    if(!socket->joinMulticastGroup(QHostAddress(net_addr ))){
        qFatal("ERROR: ssl-refbox could not join multicast group %s", net_addr.toUtf8().constData());
    }

    connect(socket, &QUdpSocket::readyRead, this, &SSLVisionListener::process_package);


}


SSLVisionListener::~SSLVisionListener(){
    if(kfilter!=nullptr) {
        delete kfilter;
        kfilter = nullptr;
    }
    thread->exit();
    thread->wait();
    instance = nullptr;
}


// Used to check if a detection and its camera are on the same side of the field.
bool SSLVisionListener::isGoodDetection(float x, float y, int cam)
{
    // Camera Config (in ssl-vision)
    //   y
    // 1 | 3
    // ----- x
    // 0 | 2


    // Camera config (in grsim)
    //   y
    // 3 | 0
    // ----- x
    // 2 | 1

    //TODO: ssl-vision and grsim sort cameras differently, thus the following code is not useful
    if(num_cameras == 2) {

        return  (x >= 0 && cam == 1) ||
                (x  < 0 && cam == 0);
    } else {
        bool grsim=false; if(grsim)
                            return true; //This NEEDS to be fixed if you are using grSIM this MIGHT be okay
        else
        return  (cam == 0 && x >  -0.01 && y >  -0.01 ) ||
                (cam == 1 && x >  -0.01 && y <=  0.01 ) ||
                (cam == 2 && x <=  0.01 && y <=  0.01 ) ||
                (cam == 3 && x <=  0.01 && y >  -0.01 );
    }

}



/* Looks at all detected balls in the frame detection, and chooses
 * the best one based on confidence. Sets the GameModel's ballpoint
 * as this best one if that confidence is > CONF_THRESHOLD_BALL,
 * and it is detected on the correct side.
 */
void SSLVisionListener::recieveBall(const SSL_DetectionFrame& frame)
{
    //Stop if no balls present
    if(frame.balls_size() <= 0) return;

    //Choose the best ball based on confidence
    auto detection = std::max_element(frame.balls().begin(), frame.balls().end(),
                                       [](const SSL_DetectionBall& b1, const SSL_DetectionBall& b2)
                                         {return b1.confidence() < b2.confidence();});

    //If it is still a good detection...

    if( detection->confidence() > CONF_THRESHOLD_BALL &&
        isGoodDetection(detection->x(),detection->y(),frame.camera_id()))
    {
        kfilter->newObservation(Point(detection->x(),detection->y()));
        mutex.lock();
            ball = kfilter->getPosition();
            ball.velocity = kfilter->getVelocity();
            ball.in_field = true;
            ball.time_stamp = time_stamp;
        mutex.unlock();

    }
//    else
//        qInfo()<<"Serious issue here";
}

/* Used to parse and recieve a generic Robot team and update GameModel with
 * the information, if we're confident on the Robot detection
 * Initialized to updating blue, but changes to yellow if not updating blue
 */
void SSLVisionListener::recieveRobotTeam(const SSL_DetectionFrame& frame, int which_team)
{
//    static int f = frame.frame_number();
    // get robot dection of team and detection counts
    auto* teamDetection = which_team == ROBOT_TEAM_YELLOW ? &frame.robots_yellow() : &frame.robots_blue();
    int*  num_detections    = robot_detection_counts[which_team];

    
    for(const SSL_DetectionRobot& detection : *teamDetection)
    {
        if( detection.confidence() > ROBOT_CONFIDENCE_THRESHOLD &&
            isGoodDetection(detection.x(),detection.y(), frame.camera_id()))
        {
            int robotID = detection.robot_id();


            mutex.lock();

                auto& robot =  robots[which_team][robotID];

                // check if robot is considered to be in the field, if it is, track its position
                bool was_in_field = robot.in_field;
                if(num_detections)
                robot.in_field = was_in_field || ++num_detections[robotID] >= 80;
                if(robot.in_field){
                    auto newPosition = Point(detection.x(), detection.y());

                    // why not use ssls tracked detection so we dont need to do speed calculations? messages_robocup_ssl_detection_tracked.proto
                    // looks like were using an old version so might be useful to switch over to that

                    // if robot was already in field, calculate time derivatives
                    if(was_in_field){
                        float delta_t = time_stamp - robot.time_stamp;
                        if(delta_t > 0){
                            robot.velocity = (newPosition - robot)/delta_t;
                            robot.angular_speed = (detection.orientation()-robot.orientation)/delta_t;
                        }
                    }

                    robot = newPosition;
                    robot.orientation = detection.orientation();
                    robot.time_stamp = time_stamp;
                }
            mutex.unlock();
        }
    }

}


void SSLVisionListener::process_package(){
    QByteArray datagram;

    while(socket->hasPendingDatagrams()){

        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());

        // read datagram and process it if it has a detection
        SSL_WrapperPacket packet;
        if( !packet.ParseFromArray(datagram.data(),datagram.size()) ||
            !packet.has_detection()) continue;


        auto& detection = packet.detection();
        time_stamp = detection.t_capture();
        recieveBall(detection);
        recieveRobotTeam(detection, ROBOT_TEAM_BLUE);
        recieveRobotTeam(detection, ROBOT_TEAM_YELLOW);

        if(++num_frame_detected > 100*num_cameras) {
            num_frame_detected = 0;
            for( int i=0; i <2 ; i++)
                for(int& reading : robot_detection_counts[i])
                    reading = 0;
        };
    }
}


void SSLVisionListener::copyState(GameState* state){
    if(instance==nullptr) return;
    instance->mutex.lock();
        // copy ball state
        state->ball->copyFromSSLVision(&instance->ball);

        //copy robot states
        for(int i=0; i<2; i++)
            for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
                state->robots[i][j]->copyFromSSLVision(&instance->robots[i][j]);

        state->time_stamp = instance->time_stamp;
    instance->mutex.unlock();
}



