#include <iostream>
#include "include/config/simulated.h"
#include "include/config/team.h"
#include "visioncomm.h"
#include "gui/mainwindow.h"
using namespace std;


VisionComm::VisionComm(GameModel *gm)
{
// Use different ports depending on whether it is simulated or the actual vision system
#if SIMULATED
    //Shamsi Vision Address
//    client = new RoboCupSSLClient(10020,"224.5.23.18");

    //James Vision Address
//    client = new RoboCupSSLClient(10020,"224.5.23.2");

    //Narges Vision Address
    client = new RoboCupSSLClient(10020,"224.5.23.8");


    //Ryan Vision Address
//    client = new RoboCupSSLClient(10020,"224.5.23.17");

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
            rob->setTeam(detectedTeamColor == ourTeamColor);
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

                        if(conf > CONF_THRESHOLD_BALL)
                        {
                        #if SIMULATED
                            ballPoint.x = ball.x();
                            ballPoint.y = ball.y();

                            gamemodel->setBallPoint(ballPoint);
                        #else
                            if ((ball.x() >= 0 && detection.camera_id() == 0) ||
                                (ball.x() < 0 && detection.camera_id() == 1))
                            {
                                ballPoint.x = ball.x();
                                ballPoint.y = ball.y();

                                gamemodel->setBallPoint(ballPoint);
                            }
                        #endif
                        }
                    }
                }

                for (int i=0; i < robots_blue_n; i++)
                {
                    float confR = detection.robots_blue(i).confidence();

                    if (confR > CONF_THRESHOLD_BOTS)
                    {
                    #if SIMULATED
                        updateInfo(detection.robots_blue(i), TEAM_BLUE);
                    #else
                        if ((detection.robots_blue(i).x() >= 0 && detection.camera_id() == 0)
                            || (detection.robots_blue(i).x() < 0 && detection.camera_id() == 1))
                        {
                            int robotID = detection.robots_blue(i).robot_id();
                            #if TEAM==TEAM_BLUE
                                if(gamemodel->findMyTeam(robotID)) {
                            #else
                                if(gamemodel->findOpTeam(robotID)) {
                            #endif
                                    /* If the robot is already in the GameModel update it immediately */
                                    updateInfo(detection.robots_blue(i), TEAM_BLUE);
                                }
                            else {
                                /* Otherwise we look at the frame counts, if the robot has been seen
                                 * 25 times in 50 frames, it is added to the gameModel.
                                 */
                                if(blue_rob[robotID] >= 25) {
                                    updateInfo(detection.robots_blue(i), TEAM_BLUE);
                                } else {
                                    if(frames > 50) {
                                        blue_rob[robotID] = 0;
                                    } else {
                                        ++blue_rob[robotID];
                                    }
                                }
                            }
                        }
                    #endif
                    }
                }

                for (int i=0; i < robots_yellow_n; i++)
                {
                    float confR = detection.robots_yellow(i).confidence();

                    if (confR > CONF_THRESHOLD_BOTS)
                    {
                    #if SIMULATED
                        updateInfo(detection.robots_yellow(i), TEAM_YELLOW);
                    #else
                        if ((detection.robots_yellow(i).x() >= 0 && detection.camera_id() == 0)
                            || (detection.robots_yellow(i).x() < 0 && detection.camera_id() == 1))
                        {
                            int robotID = detection.robots_yellow(i).robot_id();
                        #if TEAM==TEAM_BLUE
                            if(gamemodel->findOpTeam(robotID)) {
                        #else
                            if(gamemodel->findMyTeam(robotID)) {
                        #endif
                                /* If the robot is already in the GameModel update it immediately */
                                updateInfo(detection.robots_yellow(i), TEAM_YELLOW);
                            }
                            else {
                                if(yellow_rob[robotID] >= 25) {
                                    updateInfo(detection.robots_yellow(i), TEAM_YELLOW);
                                } else {
                                    if(frames > 50) {
                                        yellow_rob[robotID] = 0;
                                    } else {
                                        ++yellow_rob[robotID];
                                    }
                                }
                            }
                        }
                    #endif
                    }
                }
            }//if_team

            gamemodel->notifyObservers();

        }
    }
//  cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;
//  cout <<gamemodel->toString();
//  cout << gamemodel->getBallPoint().toString() << endl;

    if(++frames > 50) {
        /* After 50 frames the seen counts of each team are set to 0. This prevents
         * ghost robots from appearing over time
         */
        frames = 0;
        for(int& i : blue_rob)
            i = 0;
        for(int& i : yellow_rob)
            i = 0;
    }

    /* Debug Robot/Hit information
    for (int i =0; i<10;i++)
    {
        cout << "id\t" << i << "\thits\t" << blue_rob[i] << endl;
    }
    */

    return true;
}



void VisionComm::run()
{
    while(true){
        receive();
    }
}


