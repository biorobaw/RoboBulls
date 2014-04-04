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

        //      int balls_n = detection.balls_size();
        //      SSL_DetectionBall ball;

        //      //Ball info:
        //      for (int i = 0; i < balls_n; i++) {
        //          ball = detection.balls(i);
        //      }

         // if (refclient.getCommand() == 's'){
              ///////strategy///////
              // set detection for strat
        //          strat.setDetection(&detection);
        //          // set team
        //          strat.setTeam(sets.team);
        //          // set ball
        //          strat.setBall(ball.x(),ball.y());
        //          //set robot objects
        //          strat.setRobots(NXTrobot);
        //          // compute strat
        //          strat.test();
              /////////////////////
        //  }

        //  if (sets.team == 'b'){
              int robots_blue_n =  detection.robots_blue_size();
              vector<Robot*> myTeam = gamemodel->getMyTeam();
              //int robot_id = 0;
              int detected_id = 0;
              //int index = -1;
              Point robPoint;

              //Blue robot info:
              for (int i = 0; i < robots_blue_n; i++) {
                  SSL_DetectionRobot robot = detection.robots_blue(i);
                  cout<<"Robot(B) : "<<i+1<< "  " << robots_blue_n<<endl;

        //              robot.
                  if (robot.has_robot_id())
                  {
                      detected_id = robot.robot_id();
                      cout<<"ID=  "<<robot.robot_id()<<endl;

                      Robot * rob = NULL;
                      //identify which blue robot id corisponds to which robot object.
                      //for (int j = 0; j<TOTAL_ROBOTS; j++)
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
                      // TODO: set position
                      //rob->setRobotPosition();
                      robPoint.setX(robot.x());
                      robPoint.setY(robot.y());
                      rob->setRobotPosition(robPoint);


        //                  Point ball_pos(ball.x(),ball.y());
        //                  Point bot(robot.x(),robot.y());

        //                  //send information to robot object
        //                  NXTrobot[index].set(bot,ball_pos,robot.orientation(),i);
        //                  //send the entire detection object
        //                  NXTrobot[index].setDetection(&detection);
        //                  //update settings
        //                  NXTrobot[index].setSettings(&sets);

                      //robot objects compute values

        //                  control.penalty(sets,refclient, index, comm, NXTrobot);
        //                  if (refclient.getCommand() == ' ')
        //                  {
        //                      control.shoot(sets, index, NXTrobot);
        //                  }

        //                  control.kickOff(sets, refclient, index, comm, NXTrobot);

        //              //fill in communication array
        //                  control.sendCommand(index, comm, NXTrobot);
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

