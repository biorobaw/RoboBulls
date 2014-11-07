#include <algorithm>
#include <deque>
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "strategy/attackstrategy.h"

#define ATTACK_STRAT_DEBUG 0

Robot* AttackStrategy::lastDriver = nullptr;
Robot* AttackStrategy::lastReciever = nullptr;


AttackStrategy::~AttackStrategy()
{
    lastReciever = passingRobots.second;
}


void AttackStrategy::assignBeh()
{
    /***************************************/
    /* First we need to find the robots we want */
    GameModel* gm = GameModel::getModel();
    std::vector<Robot*>& myTeam  = gm->getMyTeam();

    if(myTeam.size() > 1)
    {
        std::deque<Robot*> teamDeq(myTeam.begin(), myTeam.end());

        /* We'll sort them closest to the ball first, and make the one making
         * the drive the closest, and the 2nd closest the receiver.
         * the rest are dealt with accordingly.
         */
        auto distanceFn = [&](Robot* rob1, Robot* rob2) {
            return Measurments::distance(rob1->getRobotPosition(), gm->getBallPoint()) <
                   Measurments::distance(rob2->getRobotPosition(), gm->getBallPoint());
        };
        std::sort(teamDeq.begin(), teamDeq.end(), distanceFn);

       if(lastReciever != nullptr) {
            /* Coming from another pass, lastReciever needs to be the new reciever
             * of the ball, not the always second closest to it
             */
            passingRobots = {lastReciever, teamDeq[0]};
            teamDeq.pop_front();
            teamDeq.erase(std::find(teamDeq.begin(), teamDeq.end(), lastReciever));
        } else {
            passingRobots = {teamDeq[0], teamDeq[1]};
            teamDeq.pop_front();
            teamDeq.pop_front();
        }

        /* Then let's add three robots as blockers, or less if there are
         * not enough left in the team. Then remove them from the team list
         */
        int numBlockBots = std::min(2, (int)teamDeq.size());
        blockingRobots.assign(teamDeq.begin(), teamDeq.begin()+numBlockBots);
        teamDeq.erase(teamDeq.begin(), teamDeq.begin()+numBlockBots);

        /* The remaining robots are defenders */
        if(!teamDeq.empty()) {
            defendingRobots.assign(teamDeq.begin(), teamDeq.end());
        }

        /***************************************/
        /* Now we can assign our behaviors */
        BehaviorAssignment<AttackDriver> passerOneMove(true);
        passerOneMove.setBehParam<Robot*>("recieveBot", passingRobots.second);
        passerOneMove.setBehParam<Point>("driveStartPoint", gm->getBallPoint());
        passerOneMove.assignBeh(passingRobots.first);

        BehaviorAssignment<AttackReceiver> passerTwoMove(true);
        passerTwoMove.setBehParam("driveBot", passingRobots.first);
        passerTwoMove.assignBeh(passingRobots.second);

        if(!blockingRobots.empty()) {
            BehaviorAssignment<AttackBlocker> blockerMove(true);
            blockerMove.setBehParam<Robot*>("driverRobot", passingRobots.first);
            for(Robot* robot : blockingRobots)
                blockerMove.assignBeh(robot);
        }

        /* TODO: Here, if the number of defendingRobots is above something,
         * Assign some of them to be midfielders
         */
        if(!defendingRobots.empty()) {
            BehaviorAssignment<AttackDefendGoal> goalDefMove(true);
            for(Robot* defender : defendingRobots)
                goalDefMove.assignBeh(defender);
        }
    }
    else if(myTeam.size() == 1){
        /* If we have one robot the best we can do is have it
         * just kick to the goal by itself
         */
        BehaviorAssignment<KickToGoal> goalKick(true);
        goalKick.assignBeh(myTeam.front());
    }
    else {
        /* There's nothing to do if there are 0 robots
         * on the team. Likely this will never happen
         */
    }
}


bool AttackStrategy::update()
{
    /* We're going to get a pointer to the driver's behavior and update
     * the rest based on that
     */
    Robot* driveBot = passingRobots.first;
    AttackDriver* driveBeh = dynamic_cast<AttackDriver*>(driveBot->getCurrentBeh());

    Robot* recvBot = passingRobots.second;
    AttackReceiver* recvBeh = dynamic_cast<AttackReceiver*>(recvBot->getCurrentBeh());

#if ATTACK_STRAT_DEBUG
    if(driveBeh == nullptr || recvBeh == nullptr) {
        throw std::runtime_error("ERROR: driveBot or recvBot did not have correct behavior!");
    }
#endif

    /* The updating we need to do in this strategy is move the blockers and receiver
     * robots based on if the driver is about to kick or not
     */
    if(driveBeh->isAboutToKick(driveBot)) {
        for(Robot* blocker : blockingRobots) {
            AttackBlocker* blockBeh = dynamic_cast<AttackBlocker*>(blocker->getCurrentBeh());
            blockBeh->moveAwayFromDriver();
        }
        recvBeh->setMoveStatus(false);
    } else {
        recvBeh->setMoveStatus(true);
    }

    /* the strategy is over when the attacking
     * driver robot has kicked
     */
    return driveBeh->hasDoneKick();
}

