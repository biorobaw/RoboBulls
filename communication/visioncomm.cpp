#include <cmath>
#include <deque>
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
    packetCount=0;
}

VisionComm::~VisionComm(void)
{
    client->close();
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


template<typename Detection>
static bool isGoodDetection
    (const Detection& detection, const SSL_DetectionFrame& frame, float confidence)
{
    bool isGoodConf = detection.confidence() > confidence;
    bool isGoodSide = (detection.x() >= 0 && frame.camera_id() == 0) ||
                      (detection.x()  < 0 && frame.camera_id() == 1) || SIMULATED;
    return isGoodConf && isGoodSide;
}


/* Looks at all detected balls in the frame detection, and chooses
 * the best one based on confidence. Sets the GameModel's ballpoint
 * as this best one if the confidence is > CONF_THRESHOLD_BALL
 * and it is detected on the correct side.
 */
static bool ballCompareFn(const SSL_DetectionBall&  a, const SSL_DetectionBall&  b) {
    return a.confidence() < b.confidence();
}

#define NOISE_RADIUS   15

void VisionComm::recieveBall(const SSL_DetectionFrame& frame)
{
    static Point noiseCenterPoint;
    static int seenOutsideRadiusCount = 0;
    static int seenStoppedCount = 0;
    static Point lastDetection;

    if(frame.balls_size() > 0)
    {
        auto bestDetect = std::max_element(frame.balls().begin(), frame.balls().end(), ballCompareFn);

        if(isGoodDetection(*bestDetect, frame, CONF_THRESHOLD_BALL))
        {
            Point newDetection = Point(bestDetect->x(), bestDetect->y());
        #if SIDE == SIDE_POSITIVE
            newDetection *= -1;
        #endif

            // If the ball is detected outside the noise radius more than 5 times
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
}

/* Used to parse and recieve a generic Robot team and update GameModel with
 * the information, if we're confident on the Robot detection
 * Initialized to updating blue, but changes to yellow if not updating blue
 */
void VisionComm::recieveRobotTeam(const SSL_DetectionFrame& frame, int whichTeam)
{
    auto* currentTeamDetection = &frame.robots_blue();
    auto* currentTeamVector    = &gamemodel->getMyTeam();
    int*  currentTeamCounts    = blue_rob;

    if(whichTeam == TEAM_YELLOW) {
        currentTeamDetection = &frame.robots_yellow();
        currentTeamCounts    = yellow_rob;
    }
    if(whichTeam != TEAM)
        currentTeamVector = &gamemodel->getOponentTeam();
    
    for(const SSL_DetectionRobot& robot : *currentTeamDetection)
    {
        if(isGoodDetection(robot, frame, CONF_THRESHOLD_BOTS)) {
            int robotID = robot.robot_id();
            if(gamemodel->find(robotID, *currentTeamVector) or currentTeamCounts[robotID] >= 25) {
                updateInfo(robot, whichTeam);
            } else {
                ++currentTeamCounts[robotID];
            }
        }
    }
}


bool VisionComm::receive()
{
    if (client->receive(packet) and packet.has_detection() and ++packetCount>=DISCARD_RATE)
    {
        packetCount = 0;
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
    if(++frames > 50) {
        frames = 0;
        for(int& i : blue_rob) i = 0;
        for(int& i : yellow_rob) i = 0;
    }

    return true;
}


void VisionComm::run()
{
    while(true){
        receive();
    }
}
