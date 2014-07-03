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

        mySeq.executeOn(robot);
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
//    Robot* r0 = gm->find(0, gm->getMyTeam());
//    Robot* r1 = gm->find(1, gm->getMyTeam());
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

    // Narges code
    BehaviorAssignment<DriveBallAndKick> assignment;
    assignment.setSingleAssignment(true);
    assignment.assignBeh();


    //James code
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.setBehParam<Point>("targetPoint", gm->getBallPoint());
//    assignment.assignBeh({r0, r1});
}

