#include "teststrategy.h"
#include "skill/obstacleavoidmove.h"
#include "skill/gotopositionwithorientation.h"
#include "behavior/behavior.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"

class TestBehavior : public Behavior
{
public:
    TestBehavior(const ParameterList& list)
    {
        //if(!list.paramExists<Point>("targetPoint")) {
            //std::cout << ":(" << std::endl;
        //}
        //target = list.getParam<Point>("targetPoint");
        //myMove = new Skill::ObstacleAvoidMove(target);
        GameModel* gm = GameModel::getModel();
        Point bp = gm->getBallPoint();
        target = bp;

        //std::cout << "Target: " << target.toString() << std::endl;
        std::cout << this << std::endl;
    }

    void perform(Robot* robot)
    {
       // myMove->perform(robot);

        //f(Measurments::isClose(robot->getRobotPosition(), target))
           // robot->clearCurrentBeh();
        float x = Measurments::angleBetween(robot->getRobotPosition(), target);
        Skill::GoToPositionWithOrientation go(target, x);
        go.perform(robot);
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
        s.setBehParam<Point>("targetPoint", Point(0,0));
        //s.setSingleAssignment(true);
        s.assignBeh(r0);
    }
}
