#include <cmath>
#include <sys/time.h>
#include "include/config/simulated.h"
#include "include/config/team.h"
#include "include/config/communication.h"
#include "visioncomm.h"
#include "model/gamemodel.h"

using namespace std;

VisionComm::VisionComm(GameModel *gm)
{
    client = new RoboCupSSLClient(VISION_PORT, VISION_ADDRESS);
    client->open(true);
    gamemodel = gm;
    FOUR_CAMERA_MODE = SIMULATED || FOUR_CAMERA;
    kfilter = new KFBall();
    u.resize(4);
}

VisionComm::~VisionComm(void)
{
    client->close();
    delete client;
}

bool VisionComm::isFourCameraMode()
{
    client->receive(packet);
    return (packet.geometry().calib_size() == 4);
}

/* This function processes a DetectionRobot from the vision system and fills
 * out the information in the GameModel
 */
void VisionComm::receiveRobot(const SSL_DetectionRobot& robot, int detectedTeamColor)
{
//    std::lock_guard<std::mutex> my_team_guard(GameModel::my_team_mutex);
//    std::lock_guard<std::mutex> opp_team_guard(GameModel::opp_team_mutex);


    //std::cout << "VisionComm::receiveRobot\n";//Donglin
    vector<Robot*>* currentTeam = &gamemodel->getMyTeam();

    if (detectedTeamColor != OUR_TEAM)
        currentTeam = &gamemodel->getOppTeam();

    if (robot.has_robot_id())
    {
        //std::cout << "VisionComm::receiveRobot   has ID\n";//Donglin
        // 2 5 7 8
        int id = robot.robot_id();
        Robot* rob = gamemodel->find(id, *currentTeam);

        if (rob == NULL)
        {
            //std::cout<<"Did not find.........................................."<<std::endl;//Added by Bo Wu
            rob = new Robot();
            rob->setID(id);
            rob->setTeam(detectedTeamColor == OUR_TEAM);
            currentTeam->push_back(rob);
        }

        // Assumption: rob contains the robot with id == detected_id
        //std::cout << " positionReading("<<robot.x()<<","<< robot.y()<<");\n ";
        Point positionReading(robot.x(), robot.y());//Point
        float rotationReading = robot.orientation();
        #if SIDE == SIDE_POSITIVE
            positionReading *= -1;
            if(rotationReading > 0)
                rotationReading = -(M_PI - rotationReading);
            else
                rotationReading = -(-M_PI - rotationReading);
        #endif
        rob->setRobotPosition(positionReading);
        rob->setOrientation(rotationReading);
    }
}

// Used to check if a detection and its camera are on the same side of the field.
template<typename Detection>
static bool isGoodDetection
    (const Detection& detection, const SSL_DetectionFrame& frame, float confidence, bool fourCameraMode)
{
    bool isGoodConf = detection.confidence() > confidence;

    // For correct side, we look at the X and Y readings and the camera that
    // reported them. This is to help prevent duplicated readings on the edges
    bool isGoodSide = false;
    float x = detection.x();
    float y = detection.y();
  //std::cout << "detection.x()"<<detection.x() <<"detection.y()"<<detection.y()<<endl;//Donglin
    // Camera Config
    // 1 | 0
    // -----
    // 2 | 3

    float cam = frame.camera_id();
     //std::cout << "!!!!!cam: "<<  cam<<endl;//Donglin
    if(!fourCameraMode) {
        isGoodSide =
        (x >= 0 && cam == 1) ||
        (x  < 0 && cam == 0);
        //std::cout << "HAisGoodSide: "<<  isGoodSide<<endl;//Donglin
    } else {
        isGoodSide =
        (x >  0 && y > 0 && cam == 0) ||
        (x <= 0 && y > 0  && cam == 1) ||
        (x <= 0 && y <= 0 && cam == 2) ||
        (x >  0 && y <= 0 && cam == 3);
        //std::cout << "isGoodSide22222: "<<  isGoodSide<<endl;//Donglin
    }
    // isGoodSide = SIMULATED;    //Simulated overrides anything

    //return true;
    return isGoodConf && isGoodSide;
}

// Movement distance between detections within which the ball is said to be stationary
#define B_STOP_THRESH 5.0

/* Looks at all detected balls in the frame detection, and chooses
 * the best one based on confidence. Sets the GameModel's ballpoint
 * as this best one if that confidence is > CONF_THRESHOLD_BALL,
 * and it is detected on the correct side.
 */
void VisionComm::recieveBall(const SSL_DetectionFrame& frame)
{
    //Stop if no balls present
    if(frame.balls_size() <= 0)
        return;

    //Choose the best ball based on confidence
    auto bestDetect = std::max_element(frame.balls().begin(), frame.balls().end(),
                                       [](const SSL_DetectionBall& b1, const SSL_DetectionBall& b2)
                                         {return b1.confidence() < b2.confidence();});

    //If it is still a good detection...
    if(isGoodDetection(*bestDetect, frame, CONF_THRESHOLD_BALL, FOUR_CAMERA_MODE))
    {
        Point b_pos = Point(bestDetect->x(), bestDetect->y());
        #if SIDE == SIDE_POSITIVE
            b_pos *= -1;
        #endif

//        GuiInterface* gui = GuiInterface::getGuiInterface();
//        gui->drawPath(b_pos, b_pos, 0.1);

        // Record velocity history
        Point k_b_pos = Point(u(2), u(4));
        vel_hist[i_vel_hist] = k_b_pos - prev_k_b_pos;
        i_vel_hist = (i_vel_hist + 1) % (VEL_HIST_SIZE - 1);
        prev_k_b_pos = k_b_pos;

        // Calculate Average Velocity from history
        Point avg_vel = Point(0,0);
        for(Point v: vel_hist)
            avg_vel += v;

        avg_vel /= (VEL_HIST_SIZE * 0.0193);
        // 0.0193 is the seconds between frames

        // Kalman Filter
        if(!kfilter_init)
        {
            // Initialize Filter
            KFBall::Matrix P0;
            P0.resize(4,4);
            P0(1,1) = 500;
            P0(2,2) = 100;
            P0(3,3) = 500;
            P0(4,4) = 100;

            KFBall::Vector x(4);
            x(1) = 0.0;
            x(2) = b_pos.x;
            x(3) = 0.0;
            x(4) = b_pos.y;

            kfilter->init(x, P0);
            kfilter_init = true;
        }
        else
        {
            // Update Filter
            KFBall::Vector z(4);
            z(1) = avg_vel.x;
            z(2) = b_pos.x;
            z(3) = avg_vel.y;
            z(4) = b_pos.y;

            kfilter->step(u, z);

            KFBall::Vector state = kfilter->getX();

//            GuiInterface* gui = GuiInterface::getGuiInterface();
//            gui->drawPath(b_pos, b_pos + avg_vel, 0.1);

            for(int i = 1; i <= 4; ++i)
                u(i) = state(i);

            // Update GameModel
            gameModel->setBallPoint(Point(state(2), state(4)));

            double b_vel = hypot(state(1), state(3));
            if(b_vel < B_STOP_THRESH)
                gameModel->setBallVelocity(Point(0, 0));
            else
                gameModel->setBallVelocity(Point(state(1), state(3)));
        }
    }
}

/* Used to parse and recieve a generic Robot team and update GameModel with
 * the information, if we're confident on the Robot detection
 * Initialized to updating blue, but changes to yellow if not updating blue
 */
void VisionComm::recieveRobotTeam(const SSL_DetectionFrame& frame, int team)
{

    //std::cout<<"VisionComm::recieveRobotTeam\n"<<std::endl;

    auto* currentTeamDetection = &frame.robots_blue();
    auto* currentTeamVector    = &gamemodel->getMyTeam();
    int*  currentTeamCounts    = blue_rob_readings;

    if(team == TEAM_YELLOW) {
        currentTeamDetection = &frame.robots_yellow();
        currentTeamCounts    = yell_rob_readings;
    }
    if(team != OUR_TEAM)
        currentTeamVector = &gamemodel->getOppTeam();
    
    for(const SSL_DetectionRobot& robot : *currentTeamDetection)
    {

        //std::cout<<"VisionComm::robot.robot_id() robot detected: "<< robot.robot_id()<<std::endl;

        if(isGoodDetection(robot, frame, CONF_THRESHOLD_BOTS, FOUR_CAMERA_MODE)) {
            //std::cout<<"VisionComm::robot.robot_id() GOOD DETECTION GOOD DETECTION!!!!!!!!!!!\n"<<std::endl;
            int robotID = robot.robot_id();
            if(gamemodel->find(robotID, *currentTeamVector) or currentTeamCounts[robotID] >= 80) {
                receiveRobot(robot, team);
            } else {
               ++currentTeamCounts[robotID];
            }
        }
    }
}

void VisionComm::receiveIfMSPassed(int ms_limit)
{
    timeval now;
    gettimeofday(&now, NULL);
    float seconds = now.tv_sec - lastRecvTime.tv_sec;
    float useconds = now.tv_usec - lastRecvTime.tv_usec;
    float ms_passed = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    if(ms_passed > ms_limit) {
        client->receive(packet);    //Recieve packet here
        lastRecvTime = now;
    }
}

void VisionComm::receive()
{

    static unsigned long int debugCounter = 0;

    //std::cout << "at VisionComm::receive() 0 "<<endl;
    //Receive a new packet if X ms has passed (0 FOR NOW)
    //receiveIfMSPassed(10);
    client->receive(packet);    //Recieve packet here

    //std::cout << "at VisionComm::receive() 1 "<<endl;

    // Update arrays used for tracking frames from each quadrant/half
    if(packet.has_detection())
    {
        const SSL_DetectionFrame& frame = packet.detection();
        frames[frame.camera_id()] = frame;
        frames_state[frame.camera_id()] = true;
    }

    //std::cout << "at VisionComm::receive() 2 "<<endl;

    bool all_frames_recv = true;
    int num_cams = FOUR_CAMERA_MODE? 4 : 2;
    //std::cout<<"Camera numberrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr:"<<num_cams<<std::endl;//Added by Bo Wu
    for(int i = 0; i < num_cams; ++i)
        if(!frames_state[i])
            all_frames_recv = false;

    //std::cout << "at VisionComm::receive()"<<endl;
    if(all_frames_recv)
    {
        //std::cout << debugCounter++<< "at VisionComm::receive()     all frames received\n"<<endl;
        for(int i = 0; i < num_cams; ++i)
        {
            //std::cout << "at VisionComm::receive()   processing cam "<<i <<endl;
            recieveBall(frames[i]);
            recieveRobotTeam(frames[i], TEAM_BLUE);
            recieveRobotTeam(frames[i], TEAM_YELLOW);
            frames_state[i] = false;
        }

        /* After we have had a chance to initially recieve all robots,
         * the RoboBulls game is run with the new information here. */
        if (++totalframes > 200){
//            std::cout << "at VisionComm::notifying()"<<endl;
            gamemodel->notifyObservers();
        }

        /* After 100 frames the "seen counts" of each team are set to 0. This prevents
         * ghost robots from appearing over time */
        if(++resetFrames > 100) {
            resetFrames = 0;
            for(int& reading : blue_rob_readings) reading = 0;
            for(int& reading : yell_rob_readings) reading = 0;
        }
    }
}

void VisionComm::run()
{
    //std::cout << "at VisionComm::run()\n";
    while(true){
        receive();
        //std::cout << "at VisionComm::run() looping \n";
    }
}
