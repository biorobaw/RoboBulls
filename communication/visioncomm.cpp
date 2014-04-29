#include "visioncomm.h"
#include <iostream>

using namespace std;

const int TEAM = 0;

VisionComm::VisionComm(GameModel *gm)
{
    client.open(true);
    gamemodel = gm;
    count=0;

}

VisionComm::~VisionComm(void)
{
    client.close();
    //CloseHandle(hThread); //stop thread
}

void VisionComm::updateInfo(SSL_DetectionRobot robot, string color)
{
    Robot * rob;

    int ourTeamColor = TEAM;

    float id;
    Point robPoint;
//    float conf = 0.0;
//    SSL_DetectionFrame detection = packet.detection();

//    int robots_blue_n =  detection.robots_blue_size();
//    int robots_yellow_n = detection.robots_yellow_size();
//    int size = 0;

    int detectedTeamColor;   //if 0, then it's blue. If 1, then it's yellow team.

//    int robots_blue_n =  detection.robots_blue_size();
//    int robots_yellow_n = detection.robots_yellow_size();

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


    //You can change TEAM to decide if you want to play as blue team or yellow team
//    if (ourTeamColor == 0)
//    {
//        gamemodel->setMyTeam(myTeam);
//        gamemodel->setOponentTeam(opTeam);
//    }
//    else if (ourTeamColor == 1)
//    {
//        gamemodel->setMyTeam(opTeam);
//        gamemodel->setOponentTeam(myTeam);
//    }

    gamemodel->setMyTeam(myTeam);
    gamemodel->setOponentTeam(opTeam);

    cout << "Size of my team: " << gamemodel->getMyTeam().size() << endl;
    cout << "Size of oponent team: " << gamemodel->getOponentTeam().size() << endl;

    cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;

}

//Robot VisionComm::detection()
//{
//    Robot robot;
//    SSL_DetectionFrame detection = packet.detection();

//    int robots_blue_n =  detection.robots_blue_size();
//    int robots_yellow_n = detection.robots_yellow_size();

//    vector<Robot*> myTeam = gamemodel->getMyTeam();
//    vector<Robot*> opTeam = gamemodel->getOponentTeam();

//    for (int i = 0; i < robots_blue_n; i++)
//    {
//        robot = gamemodel->find(detection.robots_blue(i), myTeam);
//    }
//    return robot;
//}


//bool VisionComm::receive()
//{

//    //cout<<("I'm in vissionComm now and receiving something!")<<endl;

//    client.open(true);

//    if (client.receive(packet))
//    {

//        //Rcv packet
//        //use the client to recieve the package and check if it's recieved... look at Wiliam's code
//        //take a look at refcomm

//        if (packet.has_detection())
//        {
//            count++;
////            cout << "Count " << count << endl;
//            //            cout<<"Packet detected!"<<endl;
//            if (count >= DISCARD_RATE)
//            {
//                count = 0;

//                SSL_DetectionFrame detection = packet.detection();

//                int robots_blue_n =  detection.robots_blue_size();
//                int robots_yellow_n = detection.robots_yellow_size();
//                int balls_n = detection.balls_size();
//                vector<Robot*> myTeam = gamemodel->getMyTeam();
//                vector<Robot*> opTeam = gamemodel->getOponentTeam();


//                int detected_id = 0;
//                int detected_id2 = 0;

//                Point robPoint;
//                Point robPoint2;
//                Point ballPoint;

//                float conf = 0.0;
//                float confR = 0.0;

//                //Ball info:
//                for (int i = 0; i < balls_n; i++)
//                {
//                    SSL_DetectionBall ball;
//                    ball = detection.balls(i);

//                    if (ball.confidence()>conf)
//                    {
//                        conf = ball.confidence();

//                        if(conf > CONF_THRESHOLD)
//                        {
//                            ballPoint.x = ball.x();
//                            ballPoint.y = ball.y();

//                            gamemodel->setBallPoint(ballPoint);
//                        }
//                    }
//                }

//                //            cout << "Ball Found: " << ballPoint.getX() << ballPoint.getY()<< endl;
//                //            cout <<"Confidece is" << conf << endl;


//                //Blue robot info:
//                for (int i = 0; i < robots_blue_n; i++)
//                {
//                    SSL_DetectionRobot robot = detection.robots_blue(i);
//                    //                cout<<"Robot(B) : "<<i+1<< "  " << robots_blue_n<<endl;

//                    //                if (robot.confidence() > confR)
//                    //                {
//                    confR = robot.confidence();
//                    //                    cout << "Conf " << confR << endl;
//                    if (confR > CONF_THRESHOLD)
//                    {
//                        //              robot.
//                        if (robot.has_robot_id())
//                        {
//                            detected_id = robot.robot_id();
//                            //                  cout<<"ID=  "<<robot.robot_id()<<endl;

//                            Robot * rob = NULL;
//                            //identify which blue robot id corisponds to which robot object.
//                            for(vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
//                            {
//                                if ((*it)->getID() == detected_id)
//                                {
//                                    // TODO: Compare with current's confidence - low priority
//                                    rob = (*it);
//                                    //                                    cout<<" id = \n" <<(*it)->getID()<<endl;
//                                }
//                            }

//                            if (rob == NULL)
//                            {
//                                rob = new Robot();
//                                rob->setID(detected_id);
//                                myTeam.push_back(rob);
//                            }


//                            // Assumption: rob contains the robot with id == detected_id
//                            rob->setOrientation(robot.orientation());
//                            robPoint.x = robot.x();
//                            robPoint.y = robot.y();
//                            rob->setRobotPosition(robPoint);

//                            // TODO: orientation!

//                        }//if robot has id
//                    }//checking confident
//                    //                 }
//                }//for loop blue robots

//                //Yellow robot info
//                for (int i = 0; i < robots_yellow_n; i++)
//                {
//                    SSL_DetectionRobot robot = detection.robots_yellow(i);

//                    //                if (robot.confidence() > confR)
//                    //                {
//                    confR = robot.confidence();
//                    //                    cout << "Conf " << confR << endl;
//                    if (confR > CONF_THRESHOLD)
//                    {
//                        //              robot.
//                        if (robot.has_robot_id())
//                        {
//                            detected_id2 = robot.robot_id();
//                            //                  cout<<"ID=  "<<robot.robot_id()<<endl;

//                            Robot * rob = NULL;
//                            //identify which blue robot id corisponds to which robot object.
//                            for(vector<Robot*>::iterator it = opTeam.begin(); it != opTeam.end(); it++)
//                            {
//                                if ((*it)->getID() == detected_id2)
//                                {
//                                    // TODO: Compare with current's confidence - low priority
//                                    rob = (*it);
//                                    //                                    cout<<" id = \n" <<(*it)->getID()<<endl;
//                                }
//                            }

//                            if (rob == NULL)
//                            {
//                                rob = new Robot();
//                                rob->setID(detected_id2);
//                                opTeam.push_back(rob);
//                            }

//                            // Assumption: rob contains the robot with id == detected_id
//                            rob->setOrientation(robot.orientation());
//                            robPoint2.x = robot.x();
//                            robPoint2.y = robot.y();
//                            rob->setRobotPosition(robPoint2);
//                        }//if robot has id
//                    }//checking confident
//                }//for loop Yellow robots

//                if (TEAM == 0)
//                {
//                    gamemodel->setMyTeam(myTeam);
//                    gamemodel->setOponentTeam(opTeam);
//                }
//                else
//                {
//                    gamemodel->setMyTeam(opTeam);
//                    gamemodel->setOponentTeam(myTeam);
//                }
//            }//if_team

//        }    //            return false;
//    }
////        cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;

//}

bool VisionComm::receive()
{

    //cout<<("I'm in vissionComm now and receiving something!")<<endl;

    client.open(true);

    if (client.receive(packet))
    {

        //Rcv packet
        //use the client to recieve the package and check if it's recieved... look at Wiliam's code
        //take a look at refcomm

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
                    cout << "confR blue: " << confR << endl;
                    if (confR > CONF_THRESHOLD)
                    {
                        updateInfo(detection.robots_blue(i), "Blue");
                    }

                }


                for (int i=0; i < robots_yellow_n; i++)
                {
                    float confR = detection.robots_yellow(i).confidence();
                    cout << "confR yellow: " << confR << endl;
                    if (confR > CONF_THRESHOLD)
                    {
                        updateInfo(detection.robots_yellow(i), "Yellow");
                    }
                }
            }//if_team

//gamemodel->toString();

        }    //            return false;

    }
//        cout << "Size at end of detection: " << gamemodel->getMyTeam().size()+gamemodel->getOponentTeam().size() << endl;
//    gamemodel->toString();
}



void VisionComm::run()
{
    while(true){
        receive();
    }
}


