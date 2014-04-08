#include "visioncomm.h"
#include <iostream>

using namespace std;

VisionComm::VisionComm(GameModel *gm)
{
    client.open(true);
    gamemodel = gm;

}

VisionComm::~VisionComm(void)
{
    client.close();
  //CloseHandle(hThread); //stop thread
}


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
            cout<<"Packet detected!"<<endl;

            SSL_DetectionFrame detection = packet.detection();

            int robots_blue_n =  detection.robots_blue_size();
            int balls_n = detection.balls_size();
            vector<Robot*> myTeam = gamemodel->getMyTeam();

            int detected_id = 0;

            Point robPoint;
            Point ballPoint;

            Ball * gameBall = NULL;

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

                        ballPoint.setX(ball.x());
                        ballPoint.setY(ball.y());

                        gamemodel->setBallPoint(ballPoint);
                    }
                }



//                if (gameBall = NULL)
//                    gameBall = new Ball();

//                gameBall->setBallPosition(ballPoint);
            }

            cout << "Ball Found: " << ballPoint.getX() << ballPoint.getY()<< endl;
            cout <<"Confidece is" << conf << endl;


            //Blue robot info:
            for (int i = 0; i < robots_blue_n; i++)
            {
              SSL_DetectionRobot robot = detection.robots_blue(i);
              cout<<"Robot(B) : "<<i+1<< "  " << robots_blue_n<<endl;

            //              robot.
              if (robot.has_robot_id())
              {
                  detected_id = robot.robot_id();
                  cout<<"ID=  "<<robot.robot_id()<<endl;

                  Robot * rob = NULL;
                  //identify which blue robot id corisponds to which robot object.
                  for(vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
                  {
                      if ((*it)->getID() == detected_id)
                      {
                          rob = (*it);
                          cout<<" id = \n" <<(*it)->getID()<<endl;
                      }
                  }

                  if (rob == NULL){
                      rob = new Robot();
                      rob->setID(detected_id);
                      myTeam.push_back(rob);
                  }


                  // Assumption: rob contains the robot with id == detected_id
                  rob->setOrientation(robot.orientation());
                  robPoint.setX(robot.x());
                  robPoint.setY(robot.y());
                  rob->setRobotPosition(robPoint);


              }//if robot has id
        //  }//for blue robots
            }//if_team

            return false;
        }
    }
}

void VisionComm::run()
{
  while(true){
      receive();
  }
}

