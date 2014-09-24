#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/positionforkickoff.h"
#include "behavior/defendoneonone.h"
#include "behavior/driveballandkick.h"
#include "behavior/mytestbehavior.h"
#include "behavior/defendclosetoball.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"


class TestBehavior : public Behavior
{
public:
    TestBehavior(const ParameterList& list)
    {
        UNUSED_PARAM(list);
        targetPoint = GameModel::getModel()->getPenaltyPoint();
        db = new Skill::DriveBall(targetPoint, 0);
    }

    void perform(Robot * robot)
    {
        db->perform(robot);
    }

private:
    Skill::DriveBall* db;
    Point targetPoint;
};

class TestBehavior2 : public GenericMovementBehavior
{
public:
    TestBehavior2(const ParameterList& list)
    {
        UNUSED_PARAM(list);
    }

    void perform(Robot *robot) override
    {
        setMovementTargets(GameModel::getModel()->getBallPoint(), 0, true);
        GenericMovementBehavior::perform(robot);
    }
};

class ShamsiStrafeBehavior : public GenericMovementBehavior
{
public:
    ShamsiStrafeBehavior(const ParameterList& list)
    {
        UNUSED_PARAM(list);
    }

    void perform(Robot *robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point rp = gm->getMyTeam().at(0)->getRobotPosition();
        Point bp = gm->getBallPoint();
        Point target_one = Point(-2000,0);
        Point target_two = Point(2000,0);
        double ori = Measurments::angleBetween(rp,bp);

        switch(state)
        {
        case pos_one:
            setMovementTargets(target_one,ori,false);
            if (Measurments::isClose(rp,target_one))
                state = pos_two;
            break;
        case pos_two:
            setMovementTargets(target_two,ori,false);
            if (Measurments::isClose(rp,target_two))
                state = pos_one;
        }

        GenericMovementBehavior::perform(robot,Movement::Type::facePoint);
    }
private:
    enum {pos_one,pos_two} state = pos_one;
};

TestStrategy::TestStrategy()
{
}

void TestStrategy::assignBeh()
{
//***************************************************************************************************
    //Shamsi Code
    BehaviorAssignment<ShamsiStrafeBehavior> assignment(true);
    assignment.assignBeh();

    //Martin code
//    cout << "running test strategy!" << endl;
//    GameModel * gm = GameModel::getModel();

//    float ballToOpGoalDist = Measurments::distance(gm->getBallPoint(), gm->getOpponentGoal());
//    float ballToMyGoalDist = Measurments::distance(gm->getBallPoint(), gm->getMyGoal());

//    if (ballToOpGoalDist > ballToMyGoalDist){
//        BehaviorAssignment<KickToGoal> assignment;
//        assignment.setSingleAssignment(true);
//        assignment.assignBeh();
//    } else {
//        BehaviorAssignment<DefendOneOnOne> assignment;
//        assignment.setSingleAssignment(true);
//        assignment.assignBeh();
//    }


    //test behavior
//    GameModel* gm = GameModel::getModel();
//    Robot* r0 = gm->find(0, gm->getMyTeam());
//    if(!r0) return;


    //james code

//    GameModel* gm = GameModel::getModel();
//#if SIMULATED
//    Robot* r0 = gm->find(0, gm->getMyTeam());
//    Robot* r1 = gm->find(1, gm->getMyTeam());
//#else
//    Robot* r0 = gm->find(3, gm->getMyTeam());
//    Robot* r1 = gm->find(8, gm->getMyTeam());
//#endif
//    if(!r0 || !r1) return;


//    //if(r0 != NULL) {
//     //   BehaviorAssignment<TestBehavior> ass;
//        //ass.setBehParam<Point>("targetPoint", point())
//     //   ass.assignBeh(r0);
//   // }

//    //BehaviorAssignment<DriveBallAndKick> assignment;
//    GameModel * gm = GameModel::getModel();
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.setBehParam<Point>("targetPoint", gm->getBallPoint());
//    assignment.assignBeh({0, 1});

    // Narges code testing DriveBallAndKick
//    BehaviorAssignment<TestBehavior> assignment(true);
//    assignment.assignBeh({3});


////    //Narges code testing defendCloseToBall
//    BehaviorAssignment<DefendCloseToBall> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

////    Narges code testing defendfarfromball
//    BehaviorAssignment<DefendFarFromBall> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

////    Narges testing myTestBehavior
//    BehaviorAssignment<myTestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

    //testing test behavior
//        BehaviorAssignment<AttackMain> assignment;
//        assignment.setSingleAssignment(true);
//        assignment.assignBeh();



//    //James code
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    Point p = gm->getBallPoint() - Point (200,200);
//    assignment.setBehParam<Point>("targetPoint",p );
//    assignment.assignBeh({r0, r1});
}

