#include "indirectkickstrategy.h"
#include "model/gamemodel.h"
#include "utilities/region/rectangle.h"
#include "utilities/comparisons.h"
#include "include/config/team.h"
#include "behavior/goalie.h"
#include "behavior/refstop.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"

#if SIMULATED
    #define R 400
#else
    #define R 700
#endif

void IndirectKickStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();
    Robot *sender;
    Robot *receiver;

    /*
     * finds the closest robot to the ball
     * and makes it the sender.
     * we use the sender in PassBallSender behavior
     * */
    if ((gm->getGameState() == 'I' && OUR_TEAM == TEAM_BLUE) ||
        (gm->getGameState() == 'i' && OUR_TEAM == TEAM_YELLOW))
    {
        //First we have all other robots do whatever
        for(Robot* robot : gameModel->getMyTeam())
            robot->assignBeh<RefStop>();

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
            Rectangle PlayerRegion { robPos.x+R, robPos.y+R, robPos.x-R, robPos.y-R };
            playersCharactristics pch;
            pch.ID = robot->getID();
            pch.surroundingAppNum = PlayerRegion.numOfOpponents();
            pch.distanceToRobot = Measurements::distance(robot, sender);
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
        receiver->assignBeh<AttackSupport>();
        receiverBot = receiver; //Store reciever in class for getNextStrategy

        /* Figuring out a waiter: Robot that is not 5, sender, or reciever that
         * is farthest from this point. This point is about where NGS will send a robot
         * going to the waiting line. It finishes a lot quicker usually, breaking this idea.
         */
        sender->assignBeh<AttackMain>();

        //Goalie is always goalie
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        if(goalie)
            goalie->assignBeh<Goalie>();
    }
    else if ((gm->getGameState() == 'i' && OUR_TEAM == TEAM_BLUE) ||
             (gm->getGameState() == 'I' && OUR_TEAM == TEAM_YELLOW))
    {
        //Assign all simple behaviors
        for(Robot* robot : gameModel->getMyTeam())
            robot->assignBeh<RefStop>();

        //Then assign goalie Goalie
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        if(goalie)
            goalie->assignBeh<Goalie>();
    }
}


char IndirectKickStrategy::getNextStrategy()
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
