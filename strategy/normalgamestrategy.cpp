#include <vector>
#include "behavior/genericmovementbehavior.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/stopbehavior.h"
#include "model/gamemodel.h"
#include "strategy/normalgamestrategy.h"

bool NormalGameStrategy::isOnAttack = true;

/*************************************************/

void NormalGameStrategy::assignBeh()
{
    GameModel* gm = GameModel::getModel();
    Point ballPoint = gm->getBallPoint();
    std::vector<Robot*> myTeam = gm->getMyTeam();

    for(Robot* robot : myTeam)
        robot->clearCurrentBeh();

    if(isOnAttack) {
        /* Assign behaviors to robots to attack */
        std::deque<Robot*> teamDeq(myTeam.begin(), myTeam.end());

        /* Here we are sorting the robots by distance to the ball, and now
         * we are assigning the closest one to kick to the goal
         */
        auto distanceFn = [&](Robot* rob1, Robot* rob2) {
            return Measurments::distance(rob1->getRobotPosition(), ballPoint) <
                   Measurments::distance(rob2->getRobotPosition(), ballPoint);
        };
        std::sort(teamDeq.begin(), teamDeq.end(), distanceFn);

        BehaviorAssignment<KickToGoal> goalKick(true);
        goalKick.assignBeh(teamDeq.front());
        teamDeq.pop_front();
    }
    else {
        /* Assign behaviors to robots to defend */
    }
}


bool NormalGameStrategy::update()
{
    return false;
}
