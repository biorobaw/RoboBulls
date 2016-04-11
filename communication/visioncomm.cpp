#include <cmath>
#include <sys/time.h>
#include "include/config/simulated.h"
#include "include/config/team.h"
#include "include/config/communication.h"
#include "visioncomm.h"

using namespace std;

VisionComm::VisionComm(GameModel *gm)
{
    client = new RoboCupSSLClient(VISION_PORT, VISION_ADDRESS);
    client->open(true);
    gamemodel = gm;
    fourCameraMode = isFourCameraMode();
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
void VisionComm::updateInfo(const SSL_DetectionRobot& robot, int detectedTeamColor)
{
    vector<Robot*>* currentTeam = &gamemodel->getMyTeam();

    if (detectedTeamColor != TEAM)
        currentTeam = &gamemodel->getOponentTeam();

    if (robot.has_robot_id())
    {
        int id = robot.robot_id();
        Robot* rob = gamemodel->find(id, *currentTeam);

        if (rob == NULL)
        {
            rob = new Robot();
            rob->setID(id);
            rob->setTeam(detectedTeamColor == TEAM);
            currentTeam->push_back(rob);
        }

        // Assumption: rob contains the robot with id == detected_id
        Point positionReading(robot.x(), robot.y());
        float rotationReading = robot.orientation();
    #if SIDE == SIDE_POSITIVE
        positionReading *= -1;
        if(rotationReading > 0) {
            rotationReading = -(M_PI - rotationReading);
        } else {
            rotationReading = -(-M_PI - rotationReading);
        }
    #endif
        rob->setRobotPosition( positionReading );
        rob->setOrientation(rotationReading);

        gamemodel->onRobotUpdated(rob);
    }
}

//Used to check if the detection and the camera are on the same side of the field.
template<typename Detection>
static bool isGoodDetection
    (const Detection& detection, const SSL_DetectionFrame& frame, float confidence, bool fourCameraMode)
{
    bool isGoodConf = detection.confidence() > confidence;
    bool isGoodSide = false;

    //For correct side, we look at the X and Y readings and the camera that
    //reported them. This is to help prevent duplicated readings on the edges
    float x = detection.x();
    float y = detection.y();
    float cam = frame.camera_id();
    if(!fourCameraMode) {
        isGoodSide =
        (x >= 0 && cam == 0) ||
        (x  < 0 && cam == 1);
    } else {
        isGoodSide =
        (x >= 0 && y >= 0 && cam == 0) ||
        (x  < 0 && y >= 0 && cam == 1) ||
        (x  < 0 && y  < 0 && cam == 2) ||
        (x >= 0 && y  < 0 && cam == 3);
    }
    isGoodSide |= SIMULATED;    //Simulated overrides anything

    return isGoodConf && isGoodSide;
}

//Predicate for max_element, determining which ball is best on confidence
static bool ballCompareFn(const SSL_DetectionBall&  a, const SSL_DetectionBall&  b) {
    return a.confidence() < b.confidence();
}

//Movement distance between detections within which the ball is said to be stationary
#define NOISE_RADIUS 15

/* Looks at all detected balls in the frame detection, and chooses
 * the best one based on confidence. Sets the GameModel's ballpoint
 * as this best one if that confidence is > CONF_THRESHOLD_BALL,
 * and it is detected on the correct side.
 */
void VisionComm::recieveBall(const SSL_DetectionFrame& frame)
{
    static Point noiseCenterPoint;
    static int seenOutsideRadiusCount = 0;
    static int seenStoppedCount = 0;
    static Point lastDetection;

    //Stop if no balls present
    if(frame.balls_size() <= 0)
        return;

    //Choose the best ball based on confidence
    auto bestDetect = std::max_element(frame.balls().begin(), frame.balls().end(), ballCompareFn);

    //If it is still a good detection...
    if(isGoodDetection(*bestDetect, frame, CONF_THRESHOLD_BALL, fourCameraMode))
    {
        Point newDetection = Point(bestDetect->x(), bestDetect->y());
    #if SIDE == SIDE_POSITIVE
        newDetection *= -1;
    #endif

//        // Kalman Filter Tests
//        KFBall::Matrix P0;
//        P0.resize(6,6);
//        P0(1,1) = 1000;
//        P0(2,2) = 500;
//        P0(3,3) = 100;
//        P0(4,4) = 1000;
//        P0(5,5) = 500;
//        P0(6,6) = 100;

//        if(!kfilter_init)
//        {
//            //Initial estimate
//            KFBall::Vector x(6);
//            x(1) = newDetection.x;
//            x(2) = 0.0;
//            x(3) = newDetection.x;
//            x(4) = newDetection.y;
//            x(5) = 0.0;
//            x(6) = newDetection.x;

//            kfilter.init(x, P0);
//            kfilter_init = true;
//        }

//        // Update Filter
//        KFBall::Vector z(2);
//        z(1) = newDetection.x;
//        z(2) = newDetection.y;

//        KFBall::Vector u(6);
//        kfilter.step(u, z);

//        KFBall::Vector result = kfilter.getX();
//        cout << "Stop: " << result(1) << ", " << result(4) << endl;
//        cout << "Vel: " << result(2) << ", " << result(5) << endl;
//        cout << "Pos: " << result(3) << ", " << result(6) << endl << endl;

        // If the ball is detected outside the noise radius more than X times
        // it is considered to be moving and its position will be updated
        if(gameModel->ballStopped)
        {
            gameModel->setBallPoint(gameModel->getBallPoint());

            if(Measurments::distance(newDetection, noiseCenterPoint) > NOISE_RADIUS)
                ++seenOutsideRadiusCount;

            if(seenOutsideRadiusCount > 2)
            {
                gameModel->ballStopped = false;
                seenOutsideRadiusCount = 0;
            }
        }
        // If the ball is detected close (distance < 1) to its last point
        // 4 times it is considered stopped it's position will not be updated
        else
        {
            gameModel->setBallPoint(newDetection);

            if(Measurments::distance(newDetection,lastDetection) < 1)
                ++seenStoppedCount;
            if(seenStoppedCount >= 4)
            {
                gameModel->ballStopped = true;
                noiseCenterPoint = newDetection;
                seenStoppedCount = 0;
            }

            lastDetection = newDetection;
        }
    }
}

/* Used to parse and recieve a generic Robot team and update GameModel with
 * the information, if we're confident on the Robot detection
 * Initialized to updating blue, but changes to yellow if not updating blue
 */
void VisionComm::recieveRobotTeam(const SSL_DetectionFrame& frame, int whichTeam)
{
    auto* currentTeamDetection = &frame.robots_blue();
    auto* currentTeamVector    = &gamemodel->getMyTeam();
    int*  currentTeamCounts    = blue_rob_readings;

    if(whichTeam == TEAM_YELLOW) {
        currentTeamDetection = &frame.robots_yellow();
        currentTeamCounts    = yell_rob_readings;
    }
    if(whichTeam != TEAM)
        currentTeamVector = &gamemodel->getOponentTeam();
    
    for(const SSL_DetectionRobot& robot : *currentTeamDetection)
    {
        if(isGoodDetection(robot, frame, CONF_THRESHOLD_BOTS, fourCameraMode)) {
            int robotID = robot.robot_id();
            if(gamemodel->find(robotID, *currentTeamVector) or currentTeamCounts[robotID] >= 25) {
                updateInfo(robot, whichTeam);
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

bool VisionComm::receive()
{
    //Receive a new packet if X ms has passed (0 FOR NOW)
    receiveIfMSPassed(0);

    if(packet.has_detection())
    {
        const SSL_DetectionFrame& frame = packet.detection();
        recieveBall(frame);
        recieveRobotTeam(frame, TEAM_BLUE);
        recieveRobotTeam(frame, TEAM_YELLOW);

        /* After we have had a chance to initially recieve all robots,
         * the RoboBulls game is run with the new information here. */
        if (++totalframes > 80)
            gamemodel->notifyObservers();
    }
    
    /* After 50 frames the "seen counts" of each team are set to 0. This prevents
     * ghost robots from appearing over time */
    if(++resetFrames > 50) {
        resetFrames = 0;
        for(int& reading : blue_rob_readings) reading = 0;
        for(int& reading : yell_rob_readings) reading = 0;
    }

    return true;
}


void VisionComm::run()
{
    while(true){
        receive();
    }
}
