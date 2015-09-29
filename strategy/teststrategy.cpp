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
    return false;
}

//IDs of field robots
#define HALL_ROBOT_ID   3
#define CASTLE_ROBOT_ID 2
#define SCON_ROBOT_ID   4

class KickBehavior : public Behavior
{
public:
    //Constructor with a point to kick to
    KickBehavior(Point kickTarget)
    {
        //Initially create the skill the kicking target to a point
        k = new Skill::KickToPointOmni(kickTarget);
    }

    ~KickBehavior()
    {
        delete k;
    }

    void perform(Robot* robot) override
    {
        //Work on performing the kick each iteration
        bool kickFinished = k->perform(robot);

        //KTPO returns true after each state is finished and the  kick is done
        if(kickFinished) {
            std::cout << "KickToPointOmni has finished kicking" << std::endl;
        }
    }

private:
    Skill::KickToPointOmni* k;
};

void TestStrategy::assignBeh()
{
    gameModel->findMyTeam(0)->assignBeh<GoToBeh>();
}
