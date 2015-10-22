#include "teststrategy.h"
#include "skill/kicktopointomni.h"
#include "behavior/genericmovementbehavior.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "behavior/defendfarfromball.h"
#include "behavior/rotateonpoint.h"

/************************************************************************/
//Test Behaviors Section

//Test behavior to kick the ball to the center
class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
public:
    KickBeh() {
        ktpo = new Skill::KickToPointOmni(Point(0,0));
    }
    ~KickBeh() {
        delete ktpo;
    }
    void perform(Robot * robot) override {
        ktpo->perform(robot);
    }
};

//Test behavior to rotate to the ball
class RotBeh : public GenericMovementBehavior
{
    void perform(Robot * robot) override
    {
        float ang = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(robot->getRobotPosition(), ang);
        GenericMovementBehavior::perform(robot);
    }
};

//Test behavior to dribble when ball is close
class DribbleBeh : public Behavior
{
    void perform(Robot* robot) override
    {
        Point bp = gameModel->getBallPoint();
        bool b = Measurments::distance(robot,bp) < 500;
        robot->setDrible(b);
    }
};

//Test behavior to move to a point only
class GoToBeh : public GenericMovementBehavior
{
    void perform(Robot * robot) override
    {
        setMovementTargets(Point(0,0), 0);
        GenericMovementBehavior::perform(robot);
    }
};

//Test behavior to move the robot from (-2000,0) to (2000,0)
//while facing the ball
class ShamsiStrafe : public GenericMovementBehavior
{
public:
    enum {pos_one,pos_two} state = pos_one;
    void perform(Robot *robot) override
    {
        GameModel * gm = GameModel::getModel();
        Point rp = robot->getRobotPosition();
        Point bp = gm->getBallPoint();
        Point target_one = Point(-2000,0);
        Point target_two = Point(2000,0);
        double ori = Measurments::angleBetween(rp,bp);
        switch(state)
        {
            case pos_one:
            setMovementTargets(target_one,ori);
            if (Measurments::isClose(rp,target_one,100))
            state = pos_two;
            break;
            case pos_two:
            setMovementTargets(target_two,ori);
            if (Measurments::isClose(rp,target_two,100))
            state = pos_one;
        }
        GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
    }
};

/************************************************************************/

bool TestStrategy::update()
{
    //Change IDs and behaviors to be assigned here.
    //All robots must exists before any action is taken.
    Robot* r0 = gameModel->findMyTeam(3);
    Robot* r1 = gameModel->findMyTeam(5);
    if(r0 and r1) {
        r0->assignBeh<GoToBeh>();
        r1->assignBeh<GoToBeh>();
    }
    return false;
}

void TestStrategy::assignBeh()
{

}
