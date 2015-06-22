#include "indiectkickstrategy.h"
#include "behavior/simplebehaviors.h"
#include "behavior/behaviorassignment.h"
#include "model/gamemodel.h"
#include "behavior/passballreceiver.h"
#include "behavior/passballsender.h"
#include "utilities/region.h"
#include "utilities/comparisons.h"
#include "include/config/team.h"
#include "behavior/defendfarfromball.h"

#if SIMULATED
    #define R 400
#else
    #define R 700
#endif

void IndiectKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();
    vector<Robot*>& myTeam = gm->getMyTeam();

    Robot *sender;
    Robot *receiver;

    /*
     * finds the closest robot to the ball
     * and makes it the sender.
     * we use the sender in PassBallSender behavior
     * */
    if ((gm->getGameState() == 'I' && TEAM == TEAM_BLUE) ||
        (gm->getGameState() == 'i' && TEAM == TEAM_YELLOW))
    {
        //First we have all other robots do whatever
        for(Robot* robot : gameModel->getMyTeam())
            robot->assignBeh<SimpleBehaviors>();

        //Sender is always closet to ball (this used to be 20 lines of code)
        sender = Comparisons::distanceBall().ignoreID(5).minMyTeam();

        Region PlayerRegion;
        struct playersCharactristics{
            int ID;
            int surroundingAppNum;  //number of opponent players surrounding our team players
            double distanceToRobot; // how far away is the reciever player from sender player
        };
        vector <playersCharactristics> myTeamInfo;

        for (unsigned it = 0 ; it < myTeam.size(); it++)
        {
            if (sender->getID() != myTeam[it]->getID())
            {
                PlayerRegion = Region(myTeam[it]->getRobotPosition().x + R,
                                       myTeam[it]->getRobotPosition().x - R,
                                       myTeam[it]->getRobotPosition().y + R,
                                       myTeam[it]->getRobotPosition().y - R);
                playersCharactristics pch;
                pch.ID = myTeam[it]->getID();
                pch.surroundingAppNum = PlayerRegion.numOfOpponents();
                pch.distanceToRobot = Measurments::distance(myTeam[it]->getRobotPosition(), sender->getRobotPosition());
                myTeamInfo.push_back(pch);
            }
        }

        int k = 0;
        int lessSurroundings = 0;
        double distance;
        for (unsigned j = 0; j < myTeamInfo.size(); j++)
        {
            if (j == 0 && myTeamInfo[j].ID != 5)
            {
                lessSurroundings = myTeamInfo[j].surroundingAppNum;
                distance = myTeamInfo[j].distanceToRobot;
                k = 0;
            }
            else if (j == 0 && myTeamInfo[j].ID == 5)
            {
                lessSurroundings = myTeamInfo[1].surroundingAppNum;
                distance = myTeamInfo[1].distanceToRobot;
                k = 1;
            }
            else
            {
                if (lessSurroundings > myTeamInfo[j].surroundingAppNum
                        && myTeamInfo[j].ID != 5)
                {
                    lessSurroundings = myTeamInfo[j].surroundingAppNum;
                    distance = myTeamInfo[j].distanceToRobot;
                    k = j;
                }
                else if (lessSurroundings == myTeamInfo[j].surroundingAppNum &&
                                 myTeamInfo[j].distanceToRobot < distance &&
                                 myTeamInfo[j].ID != 5)
                {
                    lessSurroundings = myTeamInfo[j].surroundingAppNum;
                    distance = myTeamInfo[j].distanceToRobot;
                    k = j;
                }
            }
        }

        //Assigning passBallReceiver behavior to the receiver robot
        int receiverID = myTeamInfo[k].ID;
        receiver = gameModel->findMyTeam(receiverID);
        receiver->assignBeh<PassBallReceiver>(sender);
        receiverBot = receiver; //Store reciever in class for getNextStrategy

        /* Figuring out a waiter: Robot that is not 5, sender, or reciever that
         * is farthest from this point. This point is about where NGS will send a robot
         * going to the waiting line. It finishes a lot quicker usually, breaking this idea.
         */
        Robot* waiter = Comparisons::idNot(5).ignoreIDs({sender, receiver}).anyMyTeam();
        sender->assignBeh<PassBallSender>(waiter);

        //Goalie is always goalie
        Robot* goalie = gameModel->findMyTeam(5);
        if(goalie)
            goalie->assignBeh<DefendFarFromBall>();
    }
    else if ((gm->getGameState() == 'i' && TEAM == TEAM_BLUE) ||
             (gm->getGameState() == 'I' && TEAM == TEAM_YELLOW))
    {
        BehaviorAssignment<DefendFarFromBall> golieAssignment;
        golieAssignment.setSingleAssignment(true);
        for (Robot* rob: myTeam)
        {
            if (rob->getID() == 5)
                golieAssignment.assignBeh(rob);
        }
        BehaviorAssignment<SimpleBehaviors> simpleAssignment;
        simpleAssignment.setSingleAssignment(true);
        for (unsigned i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != 5)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
}


char IndiectKickStrategy::getNextStrategy()
{
    /* Here we check to see if the robot has kicked (PassBallReceiver's
     * "isFinished" ans return to NGS if so */
#if 0
    if(receiverBot != NULL) {
        return receiverBot->getCurrentBeh()->isFinished() ? ' ' : '\0';
    }
#endif
    return '\0';
}
