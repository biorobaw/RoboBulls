#include "teststrategy.h"
#include "skill/obstacleavoidmove.h"
#include "behavior/behavior.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"

class TestBehavior : public Behavior
{
public:
    TestBehavior(const ParameterList& list)
    {
        target = list.getParam<Point>("targetPoint");
        myMove = new Skill::ObstacleAvoidMove(target);
    }

    void perform(Robot* robot)
    {
        myMove->perform(robot);

        if(Measurments::isClose(robot->getRobotPosition(), target))
            robot->clearCurrentBeh();
    }
private:
    Skill::ObstacleAvoidMove* myMove;
    Point target;
};


TestStrategy::TestStrategy()
{
}

void TestStrategy::assignBeh()
{
    GameModel * gm = GameModel::getModel();

    Robot* r0 = gm->find(0, gm->getMyTeam());

    if(r0) {
        BehaviorAssignment<TestBehavior> s;
        s.setBehParam<Point>("targetPoint", gm->getBallPoint());
        s.setSingleAssignment(true);
        s.assignBeh(r0);
    }
}
