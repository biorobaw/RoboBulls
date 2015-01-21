#include "include/config/simulated.h"
#include "include/config/team.h"
#include "visioncomm.h"
using namespace std;


VisionComm::VisionComm(GameModel *gm)
{
// Use different ports depending on whether it is simulated or the actual vision system
#if SIMULATED
    //Shamsi Vision Address
    //client = new RoboCupSSLClient(10020,"224.5.23.5");

    //James Vision Address
    client = new RoboCupSSLClient(10020,"224.5.23.2");

    //Narges Vision Address
    //client = new RoboCupSSLClient(10020,"224.5.23.8");

    //Ryan Vision Address
    //client = new RoboCupSSLClient(10020,"224.5.23.17");
#else
    client = new RoboCupSSLClient();
#endif
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
        rob->setRobotPosition( Point(robot.x(), robot.y()) );
        rob->setOrientation(robot.orientation());

        gamemodel->onRobotUpdated(rob);
    }
}


/* Looks at all detected balls in the frame detection, and chooses
 * the best one. Sets the GameModel's ballpoint as this best one.
 * We use max_element with the compare function below. `bind` is used
 * to turn the three-param function to a two-param function. We need this because
 * the compare function needs the `frame` to verify the correct camera.
 */
static bool ballCompareFn(const SSL_DetectionBall&  a, 
                          const SSL_DetectionBall&  b, 
                          const SSL_DetectionFrame& frame) {
    if(a.confidence() < b.confidence())
        return true;
    if((a.x()  < 0 && frame.camera_id() == 0) ||
       (a.x() >= 0 && frame.camera_id() == 1))
        return true;
    return false;
}

void VisionComm::recieveBall(const SSL_DetectionFrame& frame) 
{
    using namespace std::placeholders;
    auto bestBall = std::max_element(frame.balls().begin(), frame.balls().end(),
                        std::bind(ballCompareFn, _1, _2, frame));
    gamemodel->setBallPoint( Point(bestBall->x(), bestBall->y()) );
}


/* Used to parse and recieve a generic Robot team and update GameModel with
 * the information, if we're confident on the Robot detection
 * Initialized to updating blue, but changes to yellow if not updating blue
 */
void VisionComm::recieveRobotTeam(const SSL_DetectionFrame& frame, int whichTeam)
{
    auto* currentTeamDetection   = &frame.robots_blue();
    auto* currentTeamVector      = &gamemodel->getMyTeam();
    int*  currentTeamFrameCounts = blue_rob;

    if(whichTeam == TEAM_YELLOW) {
        currentTeamDetection   = &frame.robots_yellow();
        currentTeamFrameCounts = yellow_rob;
    }
    if(whichTeam != TEAM)
        currentTeamVector = &gamemodel->getOponentTeam();
    
    for(const SSL_DetectionRobot& robot : *currentTeamDetection)
    {
        if(robot.confidence() < CONF_THRESHOLD_BOTS)
            continue;
    #if SIMULATED == 0
        if((robot.x() >= 0 && frame.camera_id() == 0) ||
           (robot.x()  < 0 && frame.camera_id() == 1))
    #endif
        {
            /* If the robot is already in the game, Or if the robot has been seen 
             * for at least 25 of the past 50 frames, update/add the robot. Otherwise, 
             * the robot has not been added, so we increment its count of appearances 
             * in this set of 50 frames.
             */
            int robotID = robot.robot_id();
            if(gamemodel->find(robotID, *currentTeamVector) or currentTeamFrameCounts[robotID] >= 25) {
                updateInfo(robot, whichTeam);
            } else {
                ++currentTeamFrameCounts[robotID];
            }
        }
    }
}


bool VisionComm::receive()
{
    if (client->receive(packet) and packet.has_detection()) 
    {
        if (++packetCount >= DISCARD_RATE)
        {
            packetCount = 0;
            const SSL_DetectionFrame& detection = packet.detection();
            recieveBall(detection);
            recieveRobotTeam(detection, TEAM_BLUE);
            recieveRobotTeam(detection, TEAM_YELLOW);
        }
        
        //Runs the Robobulls game once
        gamemodel->notifyObservers();
    }
    
    /* After 50 frames the "seen counts" of each team are set to 0. This prevents
     * ghost robots from appearing over time
     */
    if(++frames > 50) {
        frames = 0;
        for(int& i : blue_rob)
            i = 0;
        for(int& i : yellow_rob)
            i = 0;
    }

    return true;
}


void VisionComm::run()
{
    while(true){
        receive();
    }
}
