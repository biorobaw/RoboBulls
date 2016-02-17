#include "indiectkickstrategy.h"
#include "behavior/simplebehaviors.h"
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
        sender = Comparisons::distanceBall().ignoreID(GOALIE_ID).minMyTeam();

        struct playersCharactristics {
            int ID;
            int surroundingAppNum;  //number of opponent players surrounding our team players
            double distanceToRobot; // how far away is the reciever player from sender player
        };
        std::vector<playersCharactristics> myTeamInfo;

        //We populate the vector myTeamInfo with playersCharactristics then choose best later
        for(Robot* robot : gameModel->getMyTeam())
        {
            if(robot->getID() == GOALIE_ID || robot == sender)
                continue;
            Point robPos = robot->getPosition();
            Region PlayerRegion { robPos.x+R, robPos.x-R, robPos.y+R, robPos.y-R };
            playersCharactristics pch;
            pch.ID = robot->getID();
            pch.surroundingAppNum = PlayerRegion.numOfOpponents();
            pch.distanceToRobot = Measurments::distance(robot, sender);
            myTeamInfo.push_back(pch);
        }

        //Best: The least number of opponents and closest distance to kicker
        auto bestRobotRegion = std::min_element(myTeamInfo.begin(), myTeamInfo.end(),
          [](playersCharactristics& a, playersCharactristics& b) {
                return a.surroundingAppNum < b.surroundingAppNum ||
                       a.distanceToRobot < b.distanceToRobot;
          });

        //Assigning passBallReceiver behavior to the receiver robot
        int receiverID = bestRobotRegion->ID;
        receiver = gameModel->findMyTeam(receiverID);
        receiver->assignBeh<PassBallReceiver>(sender);
        receiverBot = receiver; //Store reciever in class for getNextStrategy

        /* Figuring out a waiter: Robot that is not 5, sender, or reciever that
         * is farthest from this point. This point is about where NGS will send a robot
         * going to the waiting line. It finishes a lot quicker usually, breaking this idea.
         */
        Robot* waiter = Comparisons::idNot(GOALIE_ID).ignoreIDs({sender, receiver}).anyMyTeam();
        sender->assignBeh<PassBallSender>(waiter);

        //Goalie is always goalie
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        if(goalie)
            goalie->assignBeh<DefendFarFromBall>();
    }
    else if ((gm->getGameState() == 'i' && TEAM == TEAM_BLUE) ||
             (gm->getGameState() == 'I' && TEAM == TEAM_YELLOW))
    {
        //Assign all simple behaviors
        for(Robot* robot : gameModel->getMyTeam())
            robot->assignBeh<SimpleBehaviors>();

        //Then assign goalie DefendFarFromBall
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        if(goalie)
            goalie->assignBeh<DefendFarFromBall>();
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
