#include "visioncomm.h"
#include <iostream>
#include "include/globals.h"

using namespace std;


VisionComm::VisionComm(GameModel *gm)
{
// Use different ports depending on whether it is simulated or the actual vision system
#if SIMULATED
    client = new RoboCupSSLClient(10020,"224.5.23.3");
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

void VisionComm::updateInfo(SSL_DetectionRobot robot, string color)
{
    Robot * rob;

    int ourTeamColor = TEAM;

    float id = 0;
    Point robPoint;

    int detectedTeamColor = 0;   //if 0, then it's blue. If 1, then it's yellow team.

    vector<Robot*> myTeam = gamemodel->getMyTeam();
    vector<Robot*> opTeam = gamemodel->getOponentTeam();

    if (color.compare("Blue") == 0)
        detectedTeamColor = 0;
    else if (color.compare("Yellow") == 0)
        detectedTeamColor = 1;

    if (robot.has_robot_id())
    {
        id = robot.robot_id();

        if (detectedTeamColor == ourTeamColor)
        {
            rob = gamemodel->find(id, myTeam);
        }
        else
        {
            rob = gamemodel->find(id, opTeam);
        }
    }



    if (rob == NULL)
    {
        rob = new Robot();
        rob->setID(id);
        if (detectedTeamColor == ourTeamColor)
            myTeam.push_back(rob);
        else
            opTeam.push_back(rob);
    }


    // Assumption: rob contains the robot with id == detected_id
    rob->setOrientation(robot.orientation());
    robPoint.x = robot.x();
    robPoint.y = robot.y();
    rob->setRobotPosition(robPoint);

    gamemodel->setMyTeam(myTeam);
    gamemodel->setOponentTeam(opTeam);

    cout<<"Ball Position:\t"<<gamemodel->getBallPoint().x << " , " << gamemodel->getBallPoint().y<<endl;

//    cout << gamemodel->toString() << endl;
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
                        updateInfo(detection.robots_blue(i), "Blue");
                    }

                }


                for (int i=0; i < robots_yellow_n; i++)
                {
                    float confR = detection.robots_yellow(i).confidence();

                    if (confR > CONF_THRESHOLD)
                    {
                        updateInfo(detection.robots_yellow(i), "Yellow");
                    }
                }
            }//if_team
        }
    }
//        cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;
//    cout <<gamemodel->toString().str();

    return true;
}



void VisionComm::run()
{
    while(true){
        receive();
    }
}


