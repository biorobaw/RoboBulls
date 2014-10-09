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
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"


class TestBehavior : public GenericMovementBehavior
{
public:
    TestBehavior(const ParameterList& list)
        : GenericMovementBehavior(list)
    {
        UNUSED_PARAM(list);
        setVelocityMultiplier(0.75);
    }

    void perform(Robot * robot)
    {
        setMovementTargets(GameModel::getModel()->getBallPoint());
        GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);
    }

private:
    Point targetPoint;
};



TestStrategy::TestStrategy()
{
}

void TestStrategy::assignBeh()
{
//***************************************************************************************************
    //Shamsi Code
    //Three Omni Wheel Test
//        GameModel * gm = GameModel::getModel();
//        RobComm * comm = RobComm::getRobComm();
//        GoToPoseBasic instance;
//        threeWheelVels control = instance.calcWheelVels(gm->getMyTeam().at(0),0,0,0);
//        comm->sendVelsThreeOmni(control.L,control.R,control.B,1);
//        cout << "Sending Velocities" << endl;

    //GoToPosition Test
//        GameModel * gm = GameModel::getModel();
//        Skill::GoToPosition skill = Skill::GoToPosition(Point(-1000,0),0);
//        skill.perform(gm->getMyTeam().at(1));

    //Four Omni Wheel Test
//    GameModel * gm = GameModel::getModel();
//    Skill::GoToPosition skill = Skill::GoToPosition(Point(0,0),0);
//    skill.perform(gm->getMyTeam().at(0));
//    cout << "Sending Velocities" << endl;
//***************************************************************************************************


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
    BehaviorAssignment<DriveBallAndKick> assignment;
    assignment.setSingleAssignment(true);
    assignment.assignBeh();


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

