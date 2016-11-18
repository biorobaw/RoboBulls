#include "teststrategy.h"
#include "skill/kicktopointomni.h"
#include "skill/dribbletopoint.h"
#include "behavior/genericmovementbehavior.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "behavior/goalie.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/refstop.h"
#include "behavior/penaltygoalie.h"
#include "behavior/wall.h"
#include "ctime"

/************************************************************************/
//Test Behaviors Section

// Test behavior to repeatedly kick the ball to the center
// Waits 6 seconds between kicks
class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
    std::clock_t start;
    bool wait4recharge;
public:
    KickBeh() {
        ktpo = new Skill::KickToPointOmni(Point(0,0),-1,-1,false);
        wait4recharge = false;
    }
    ~KickBeh() {
        delete ktpo;
    }
    void perform(Robot * robot) override {
        bool finished = false;

        // Perform the kick skill as long as we are not recharging
        if(!wait4recharge)
            finished = ktpo->perform(robot);

        // If finished with kicking, we must recharge for 6 seconds
        // So do nothing
        if(finished)
        {
            wait4recharge = true;
            start = std::clock();
            robot->setLF(0);
            robot->setLB(0);
            robot->setRF(0);
            robot->setRB(0);
        }

        // If 6 seconds have passed since starting recharge,
        // We can kick again
        double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        if(duration >= 6)
            wait4recharge = false;
    }
};

// Test behavior to dribble the ball between two points
class DribbleToPointBeh : public Behavior
{
    Skill::DribbleToPoint* skill;
    Point A = Point(1500, 0);
    Point B = Point(-1500, 0);
    Point* target;
public:
    DribbleToPointBeh()
    {
        target = &A;
        skill = new Skill::DribbleToPoint(target, false, false);
    }

    ~DribbleToPointBeh()
    {
        delete skill;
    }

    void perform(Robot * robot) override
    {
        if(skill->perform(robot))
        {
            if((*target) == A)
                target = &B;
            else
                target = &A;
        }
    }
};

// Test behavior to dribble the ball between two points
class DribbleBackBeh : public Behavior
{
    Skill::DribbleBack* skill;
    Point target;
public:
    DribbleBackBeh()
    {
        target = Point(0,0);
        skill = new Skill::DribbleBack(target);
    }

    ~DribbleBackBeh()
    {
        delete skill;
    }

    void perform(Robot * robot) override
    {
        skill->perform(robot);
    }
};

// Test behavior to rotate to the ball
class RotBeh : public GenericMovementBehavior
{
    void perform(Robot * robot) override
    {
        float ang = Measurements::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(robot->getPosition(), ang);
        GenericMovementBehavior::perform(robot);
    }
};

// Test behavior to dribble when ball is close
class DribbleBeh : public Behavior
{
    void perform(Robot* robot) override
    {
        robot->setDribble(true);
    }
};

// Test behavior to move to a point and print distance and angle errors
class MotionTestBeh : public GenericMovementBehavior
{
public:
    void perform(Robot * robot) override
    {
        Point target = Point(1500,0);
        setMovementTargets(target,0, false, false);

        std::cout << "Distance Error: " << Measurements::distance(robot->getPosition(),target) << std::endl;
        std::cout << "Angle Error in Degrees: " << robot->getOrientation()*180/M_PI << std::endl;
        GenericMovementBehavior::perform(robot,Movement::Type::Default);
    }
};

// Test behavior to move the robot from (-2000,0) to (2000,0)
// while facing the ball
class ShamsiStrafe : public GenericMovementBehavior
{
public:
    int verticaloff = 0;
    ShamsiStrafe(int vertical_offset):verticaloff(vertical_offset){}
    Robot* r1 = gameModel->findMyTeam(1);
    enum {pos_one,pos_two} state = pos_one;
    void perform(Robot *robot) override
    {
//        GameModel * gm = GameModel::getModel();
        Point rp = robot->getPosition();
//        Point bp = gm->getBallPoint();
        Point target_one = Point(r1->getPosition().x-verticaloff, 1200);
        Point target_two = Point(r1->getPosition().x-verticaloff, -1200);
        double ori = Measurements::angleBetween(rp,Point(5,10));
        switch(state)
        {
            case pos_one:
                setMovementTargets(target_one,ori,false,false);
                if (Measurements::isClose(rp,target_one,DIST_TOLERANCE))
                state = pos_two;
                break;
            case pos_two:
                setMovementTargets(target_two,ori,false,false);
                if (Measurements::isClose(rp,target_two,DIST_TOLERANCE))
                state = pos_one;
        }
        GenericMovementBehavior::perform(robot, Movement::Type::Default);
    }
};


// Moves robot to the given position
class GoToBehavior : public GenericMovementBehavior
{
public:
    GoToBehavior(){}

    void perform(Robot *robot) override
    {
        setMovementTargets(gameModel->getBallPoint()+Point(500,0),Measurements::angleBetween(robot->getPosition(),gameModel->getBallPoint()),true, true);
        GenericMovementBehavior::perform(robot);
    }
};


class Receiver : public GenericMovementBehavior
{
private:
    Skill::KickToPointOmni* ktpi;
    Skill::DribbleBack* dribble_skill;
public:
    Receiver()
    {

        Point* key = new Point(-3000,0);
        dribble_skill = new Skill::DribbleBack(key);
    }

    void perform(Robot *robot) override
    {
        setMovementTargets(Point(-2000,0), 0, true, false);
        GenericMovementBehavior::perform(robot);
        dribble_skill->perform(robot);
    }
};


class Passer : public GenericMovementBehavior
{
private:
    Skill::KickToPointOmni* ktpo;
public:
    Passer()
    {
        ktpo = new Skill::KickToPointOmni(Point(-4500,0));
    }

    void perform(Robot *robot) override
    {
        setMovementTargets(Point(0,0), 0, false, false);
        GenericMovementBehavior::perform(robot);
        ktpo->perform(robot);
    }
};

class Pusher : public GenericMovementBehavior
{
private:
public:
    Robot* r1 = gameModel->findMyTeam(1);
    Pusher()
    {

    }

    void perform(Robot *robot) override
    {
        setMovementTargets(Point(r1->getPosition().x-1000,r1->getPosition().y+300), 0, false, false);
        setVelocityMultiplier(5);
        GenericMovementBehavior::perform(robot);
    }
};


bool TestStrategy::update()
{
    //Change IDs and behaviors to be assigned here.
    //All robots must exists before any action is taken.
    Robot* r1 = gameModel->findMyTeam(1);
    Robot* r2 = gameModel->findMyTeam(2);
    Robot* r3 = gameModel->findMyTeam(3);
    Robot* r4 = gameModel->findMyTeam(4);
    Robot* r5 = gameModel->findMyTeam(5);

    if(r1)
        r1->assignBeh<Passer>();
    if(r2)
        r2->assignBeh<ShamsiStrafe>(1000);
    if(r3)
        r3->assignBeh<ShamsiStrafe>(1200);
    if(r4)
        r4->assignBeh<ShamsiStrafe>(800);
    if(r5)
        r5->assignBeh<Passer>();

    return false;
}

void TestStrategy::assignBeh()
{

}
