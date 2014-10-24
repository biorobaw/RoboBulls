#include "indiectkickstrategy.h"
#include "behavior/simplebehaviors.h"
#include "behavior/behaviorassignment.h"
#include "model/gamemodel.h"
#include "behavior/passballreceiver.h"
#include "behavior/passballsender.h"
#include "utilities/region.h"

#if SIMULATED
    #define R 400
#else
    #define R 700
#endif

IndiectKickStrategy::IndiectKickStrategy()
{
}

void IndiectKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();
    vector <Robot*> myTeam;

    myTeam = gm->getMyTeam();

    Robot *sender;
    Robot *receiver;
    int senderID;
    Point ballPos = gm->getBallPoint();

    /*
     * finds the closest robot to the ball
     * and makes it the sender.
     * we use the sender in PassBallSender behavior
     * */
    if (myTeam.size() == 1)
        sender = myTeam.at(0);
    else if (myTeam.size() > 1)
    {
        for (int i = 0; i < myTeam.size(); i++)
        {
            if (i == 0 && myTeam.at(0)->getID() != 5)
                sender = myTeam.at(i);
            if (i == 0 && myTeam.at(0)->getID() == 5)
                sender = myTeam.at(1);
            else
            {
                if (myTeam.at(i)->getID() != 5)
                {
                    Point iPos = myTeam.at(i)->getRobotPosition();
                    Point closestPos = sender->getRobotPosition();
                    if (Measurments::distance(iPos, ballPos) < Measurments::distance(closestPos, ballPos))
                        sender = myTeam.at(i);
                }
            }
        }
        senderID = sender->getID();
    }


    //assigns the sender behavior to closest robot to the ball
    BehaviorAssignment<PassBallSender> senderAssignment;
    senderAssignment.setSingleAssignment(true);
    senderAssignment.assignBeh(sender);

    // creates the passBallRecever behaviorAssignment
    BehaviorAssignment<PassBallReceiver> receiverAssignment;
    receiverAssignment.setSingleAssignment(true);


    /*
     * determining which robot the sender is
     * sending the ball to. we call this robot
     * the receiver
     * */
    Region *PlayerRegion;
    struct playersCharactristics{
        int ID;
        int surroundingAppNum;  //number of opponent players surrounding our team players
        double distanceToRobot; // how far away is the recieve player from sender player
    };
    vector <playersCharactristics> myTeamInfo;

    for (int it = 0 ; it < myTeam.size(); it++)
    {
        if (sender->getID() != myTeam[it]->getID())
        {
            PlayerRegion = new Region(myTeam[it]->getRobotPosition().x + R,
                                      myTeam[it]->getRobotPosition().x - R,
                                      myTeam[it]->getRobotPosition().y + R,
                                      myTeam[it]->getRobotPosition().y - R);
            playersCharactristics pch;
            pch.ID = myTeam[it]->getID();
            pch.surroundingAppNum = PlayerRegion->numOfOpponents();
            pch.distanceToRobot = Measurments::distance(myTeam[it]->getRobotPosition(), sender->getRobotPosition());
            myTeamInfo.push_back(pch);
        }
    }

    int i;
    int lessSurroundings = 0;
    double distance;
    for (int j = 0; j < myTeamInfo.size(); j++)
    {
        if (j == 0 && myTeamInfo[j].ID != 5)
        {
            lessSurroundings = myTeamInfo[j].surroundingAppNum;
            distance = myTeamInfo[j].distanceToRobot;
            i = 0;
        }
        else if (j == 0 && myTeamInfo[j].ID == 5)
        {
            lessSurroundings = myTeamInfo[1].surroundingAppNum;
            distance = myTeamInfo[1].distanceToRobot;
            i = 1;
        }
        else
        {
            if (lessSurroundings > myTeamInfo[j].surroundingAppNum
                    && myTeamInfo[j].ID != 5)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = j;
            }
            else if (lessSurroundings == myTeamInfo[j].surroundingAppNum &&
                             myTeamInfo[j].distanceToRobot < distance &&
                             myTeamInfo[j].ID != 5)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                i = j;
            }
        }
    }
    int receiverID = myTeamInfo[i].ID;

    for (Robot *rob: myTeam)
    {
        if (rob->getID() == receiverID)
            receiver = rob;
    }

    //Assigning passBallReceiver behavior to the receiver robot
    receiverAssignment.assignBeh(receiver);
}
