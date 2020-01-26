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
        GenericMovementBehavior::perform(robot,Move::MoveType::Default);
    }
};

// Test behavior to move the robot from (-2000,0) to (2000,0)
// while facing the ball
class Strafe : public GenericMovementBehavior
{
public:
    Point A = Point(1200,0), B = Point(-1200,0);
    enum {pos_one,pos_two} state = pos_one;
    Strafe(){}
    Strafe(Point A, Point B) : A(A), B(B){}

    void perform(Robot *robot) override
    {
        Point rp = robot->getPosition();
        double ori = Measurements::angleBetween(rp, gameModel->getBallPoint());
        switch(state)
        {
            case pos_one:
                setMovementTargets(A,ori,true,true);
                if (Measurements::isClose(rp,A,DIST_TOLERANCE))
                state = pos_two;
                break;
            case pos_two:
                setMovementTargets(B,ori,true,true);
                if (Measurements::isClose(rp,B,DIST_TOLERANCE))
                state = pos_one;
        }
        GenericMovementBehavior::perform(robot, Move::MoveType::Default);
    }
};


// Moves robot to the given position
class GoToBehavior : public GenericMovementBehavior
{
public:
    GoToBehavior(double angle) {
        offset += Point(cos(angle*M_PI/180), sin(angle*M_PI/180)) * 500;
    }

    Point offset;

    void perform(Robot *robot) override  {
        setMovementTargets(gameModel->getBallPoint() + offset,
                           Measurements::angleBetween(robot->getPosition(),gameModel->getBallPoint()),
                           true, true);
        GenericMovementBehavior::perform(robot);
    }
};


TestStrategy::TestStrategy(Team* _team) : Strategy(_team) {

}

bool TestStrategy::update()
{
    //Change IDs and behaviors to be assigned here.
    //All robots must exists before any action is taken.
    Robot* r[5];
    for(int i=0; i<5; i++) r[i] = team->getRobot(i);

    if(r[0]) r[0]->assignBeh<Strafe>();
    for(int i=1; i<5; i++) r[i]->assignBeh<GoToBehavior>((i+1)*360/5);

    return false;
}

void TestStrategy::assignBeh()
{
    std::cout << "adfasdfas " << std::endl;
    Team::getTeam(TEAM_BLUE)->getRobot(0)->assignBeh<GenericMovementBehavior>(Point(-650, 300), 0);
     std::cout << "adfasdfas " << std::endl;
}

