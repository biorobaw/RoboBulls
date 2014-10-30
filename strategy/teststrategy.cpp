#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"
#include "behavior/driveballandkick.h"
#include "behavior/mytestbehavior.h"
#include "behavior/defendclosetoball.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "skill/kick.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"
#include "model/gamemodel.h"
#include "behavior/passballsender.h"
#include "behavior/passballreceiver.h"
#include "behavior/simplebehaviors.h"


class TestBehavior : public Behavior
{
public:
    TestBehavior(const ParameterList& list)
    {
        UNUSED_PARAM(list);
        targetPoint = GameModel::getModel()->getPenaltyPoint();
        db = new Skill::KickToPoint(targetPoint);
    }

    void perform(Robot * robot)
    {
        db->perform(robot);
    }

private:
    Skill::Skill* db;
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

class ShamsiStrafe : public GenericMovementBehavior
{
public:
    ShamsiStrafe(const ParameterList& list)
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
            if (Measurments::isClose(rp,target_one,50))
                state = pos_two;
            break;
        case pos_two:
            setMovementTargets(target_two,ori,false);
            if (Measurments::isClose(rp,target_two,50))
                state = pos_one;
        }

        GenericMovementBehavior::perform(robot);
    }
private:
    enum {pos_one,pos_two} state = pos_one;
};

class ShamsiGoToPose : public GenericMovementBehavior
{
public:
    ShamsiGoToPose(const ParameterList& list)
    {
        UNUSED_PARAM(list);
    }

    void perform(Robot *robot) override
    {
        setMovementTargets(Point(2000,0), M_PI, false);
        GenericMovementBehavior::perform(robot);
    }
};

class ShamsiKickToPoint : public GenericMovementBehavior
{
public:
    Skill::KickToPoint * kkkk;
    ShamsiKickToPoint(const ParameterList& list)
    {
        kkkk = new Skill::KickToPoint(Point(0,0));
        UNUSED_PARAM(list);
    }

    void perform(Robot *robot) override
    {
        kkkk->perform(robot);
    }
};


TestStrategy::TestStrategy()
{

}

void TestStrategy::assignBeh()
{
    GameModel *gm = GameModel::getModel();
//*************************************************************
//////  Shamsi Code
//    BehaviorAssignment<TestBehavior> assignment(true);
//    assignment.assignBeh();


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

//    BehaviorAssignment<ShamsiKickToPoint> assignment(true);
//    assignment.assignBeh({1});


////     Narges code testing DriveBallAndKick
//    BehaviorAssignment<DriveBallAndKick> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh(gm->getMyTeam().at(0));

//**************************************************************
//     Narges code testing DriveBallAndKick
    /*
    Point ballPos = gm->getBallPoint();
    vector<Robot*> myTeam = gm->getMyTeam();
    BehaviorAssignment<TestBehavior> assignment(true);
    assignment.setSingleAssignment(true);

    BehaviorAssignment<SimpleBehaviors> simpleAssignment;
    simpleAssignment.setSingleAssignment(true);

    BehaviorAssignment<DefendFarFromBall> golieAssignment;
    golieAssignment.setSingleAssignment(true);

    for (Robot* rob: myTeam)
    {
        if (rob->getID() == 5)
            golieAssignment.assignBeh(rob);
    }


    Robot *closestRobot;
    int closestRobotID;

    //Finds the closest robot to the penalty point and its ID
    if (myTeam.size() > 1)
    {
        if (myTeam.at(0)->getID() != 5)
            closestRobot = myTeam.at(0);
        else
            closestRobot = myTeam.at(1);

        for (unsigned i = 1; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != 5)
            {
                Point iPos = myTeam.at(i)->getRobotPosition();
                Point closestPos = closestRobot->getRobotPosition();
                if (Measurments::distance(iPos, ballPos) < Measurments::distance(closestPos, ballPos))
                    closestRobot = myTeam.at(i);
            }
        }
        closestRobotID = closestRobot->getID();
    }
//    cout << closestRobotID << endl;

    assignment.assignBeh(closestRobot);  // assigns penalty behavior to closest robot to the penalty point

    if (myTeam.size() > 1)  // assigns simple behavior to the rest of robots
    {
        for (unsigned i = 0; i < myTeam.size(); i++)
        {
            if (myTeam.at(i)->getID() != closestRobotID && myTeam.at(i)->getID() != 5)
                simpleAssignment.assignBeh(myTeam.at(i));
        }
    }
*/
//***********************************************************************************


////    Narges code testing defendCloseToBall
//    BehaviorAssignment<DefendCloseToBall> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh();


////    Narges code testing defendfarfromball
//    BehaviorAssignment<DefendFarFromBall> assignment;
//    assignment.setSingleAssignment(true);
////    for (Robot* rob: gm->getMyTeam())
////    {
////        if (rob->getID() == 5)
//            assignment.assignBeh();
////    }

////    Narges testing myTestBehavior
//    BehaviorAssignment<myTestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh(gm->getMyTeam().at(0));


////     Narges testing passSender
//    BehaviorAssignment<PassBallSender> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh(gm->getMyTeam().at(0));

////     Narges testing passReceiver
//    BehaviorAssignment<PassBallReceiver> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.assignBeh(gm->getMyTeam().at(0));

////    BehaviorAssignment<DriveBallAndKick> assignment;
//    GameModel * gm = GameModel::getModel();
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    assignment.setBehParam<Point>("targetPoint", gm->getBallPoint());
//    assignment.assignBeh({0, 1});

//    BehaviorAssignment<ShamsiKickToPoint> assignment(true);
//    assignment.assignBeh({1});

//************************************************************
////    James code
//    BehaviorAssignment<TestBehavior> assignment;
//    assignment.setSingleAssignment(true);
//    Point p = gm->getBallPoint() - Point (200,200);
//    assignment.setBehParam<Point>("targetPoint",p );
//    assignment.assignBeh({r0, r1});


////    james code
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

}




