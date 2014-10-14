#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/kicktogoal.h"
#include "behavior/defendoneonone.h"
#include "behavior/driveballandkick.h"
#include "behavior/mytestbehavior.h"
#include "behavior/defendclosetoball.h"
#include "behavior/defendfarfromball.h"
#include "behavior/attackmain.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "skill/kick.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"
#include <fstream>


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
            setMovementTargets(target_one,M_PI,false);
            if (Measurments::isClose(rp,target_one,50))
                state = pos_two;
            break;
        case pos_two:
            setMovementTargets(target_two,0,false);
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
        setMovementTargets(Point(-1000,0), M_PI, false);
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
//***************************************************************************************************
    //Shamsi Code
    BehaviorAssignment<ShamsiGoToPose> assignment(true);
    assignment.assignBeh({1});

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
}


