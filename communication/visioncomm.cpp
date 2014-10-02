#include <iostream>
#include "include/config/simulated.h"
#include "include/config/team.h"
#include "visioncomm.h"
using namespace std;


VisionComm::VisionComm(GameModel *gm)
{
// Use different ports depending on whether it is simulated or the actual vision system
#if SIMULATED
    //client = new RoboCupSSLClient(10020,"224.5.23.9");
    client = new RoboCupSSLClient(10020,"224.5.23.21");
#else
    client = new RoboCupSSLClient();
#endif
    client->open(true);
    gamemodel = gm;
    count=0;
}

VisionComm::~VisionComm(void)
{
    client->close();
    //CloseHandle(hThread); //stop thread
}

/* This function processes a DetectionRobot from the vision system and fills
 * out the information in the GameModel
 */
void VisionComm::updateInfo(const SSL_DetectionRobot& robot, int detectedTeamColor)
{
    Robot *rob;
    int   ourTeamColor = TEAM;     //if 0, then it's blue. If 1, then it's yellow team.
    float id = 0;
    Point robPoint;
    vector<Robot*>* currentTeam;
    GameModel* gm = GameModel::getModel();
    
    if (detectedTeamColor == ourTeamColor) {
        currentTeam = &gamemodel->getMyTeam();
    } else {
        currentTeam = &gamemodel->getOponentTeam();
    }

    if (robot.has_robot_id())
    {
        id  = robot.robot_id();
        rob = gm->find(id, *currentTeam);

        if (rob == NULL)
        {
            rob = new Robot();
            rob->setID(id);
            currentTeam->push_back(rob);
        }
		
		// Assumption: rob contains the robot with id == detected_id
		robPoint.x = robot.x();
        robPoint.y = robot.y();
        rob->setRobotPosition(robPoint);
		rob->setOrientation(robot.orientation());
        
        gm->setRobotUpdated(rob, detectedTeamColor);
    }
}


bool VisionComm::receive()
{
//    client.open(true);

    if (client->receive(packet))
    {

        //Rcv packet
        //use the client to recieve the package and check if it's recieved... look at Wiliam's code
        //take a look at refcommSIMULATED

        if (packet.has_detection())
        {
            count++;

            if (count >= DISCARD_RATE)
            {
                count = 0;

                SSL_DetectionFrame detection = packet.detection();

                int balls_n = detection.balls_size();
                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n = detection.robots_yellow_size();


                Point ballPoint;

                float conf = 0.0;

                //Ball info:
                for (int i = 0; i < balls_n; i++)
                {
                    SSL_DetectionBall ball;
                    ball = detection.balls(i);

                    if (ball.confidence()>conf)
                    {
                        conf = ball.confidence();

                        if(conf > CONF_THRESHOLD)
                        {
                            ballPoint.x = ball.x();
                            ballPoint.y = ball.y();

                            gamemodel->setBallPoint(ballPoint);
                        }
                    }
                }


                for (int i=0; i < robots_blue_n; i++)
                {
                    float confR = detection.robots_blue(i).confidence();
                    if (confR > CONF_THRESHOLD)
                    {
                        updateInfo(detection.robots_blue(i), TEAM_BLUE);
                    }
                }


                for (int i=0; i < robots_yellow_n; i++)
                {
                    float confR = detection.robots_yellow(i).confidence();

                    if (confR > CONF_THRESHOLD)
                    {
                        updateInfo(detection.robots_yellow(i), TEAM_YELLOW);
                    }
                }
            }//if_team
            gamemodel->notifyObservers();
        }
    }
//        cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;
//    cout <<gamemodel->toString();

    return true;
}



void VisionComm::run()
{
    while(true){
        receive();
    }
}


