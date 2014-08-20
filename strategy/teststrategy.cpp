#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"
#include "behavior/driveballandkick.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/obstacleavoidmove.h"
#include "skill/basic_movement.h"
#include "utilities/skillsequence.h"
#include "communication/robcomm.h"
#include "behavior/mytestbehavior.h"
#include "behavior/defendclosetoball.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "skill/three_omni_motion/gotoposebasic.h"
#include "communication/nxtrobcomm.h"


class TestBehavior : public Behavior
{
public:
    TestBehavior(const ParameterList& list)
    {
        Point pt = GameModel::getModel()->getPenaltyPoint();

        this->targetPoint = list.getParam<Point>("targetPoint");

       // mySeq.addSkill(new Skill::GoToPositionWithOrientation(targetPoint, 0));
        mySeq.addSkill(new Skill::ObstacleAvoidMove(targetPoint));
        mySeq.addSkill(new Skill::ObstacleAvoidMove(pt));
        mySeq.addSkill(new Skill::Stop());
    }

    void perform(Robot * robot)
    {
       // auto result = controller.
           //closed_loop_control(robot, targetPoint.x, targetPoint.y, 0);

        //RobComm* com = RobComm::getRobComm();
        //com->sendVels(result.left, result.right, robot->getID());

        if(mySeq.executeOn(robot))
            robot->clearCurrentBeh();
    }

private:
    SkillSequence mySeq;
    //ClosedLoopControl controller;
    Point targetPoint;
};



TestStrategy::TestStrategy()
{
}

void TestStrategy::assignBeh()
{
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


//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setBehParam<Point>("targetPoint", gm->getBallPoint());
//    assignment.assignBeh(r0);

//    // Narges code testing DriveBallAndKick
//    BehaviorAssignment<DriveBallAndKick> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

//    Narges code testing defendCloseToBall
//    BehaviorAssignment<DefendCloseToBall> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

    //Narges code testing defendfarfromball
//    BehaviorAssignment<DefendFarFromBall> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();

    //testing test behavior
//        BehaviorAssignment<AttackMain> assignment;
//        assignment.setSingleAssignment(true);
//        assignment.assignBeh();



    //James code
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    Point p = gm->getBallPoint() - Point (200,200);
//    assignment.setBehParam<Point>("targetPoint",p );
//    assignment.assignBeh({r0, r1});
}

