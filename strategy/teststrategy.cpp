#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"


TestStrategy::TestStrategy()
{
}

void TestStrategy::assignBeh()
{
    GameModel * gm = GameModel::getModel();

    float ballToOpGoalDist = Measurments::distance(gm->getBallPoint(), gm->getOpponentGoal());
    float ballToMyGoalDist = Measurments::distance(gm->getBallPoint(), gm->getMyGoal());

    if (ballToOpGoalDist > ballToMyGoalDist){
        BehaviorAssignment<KickToGoal> assignment;
        assignment.setSingleAssignment(true);
        assignment.assignBeh();
    } else {
        BehaviorAssignment<DefendOneOnOne> assignment;
        assignment.setSingleAssignment(true);
        assignment.assignBeh();
    }
}
