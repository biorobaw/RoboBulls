#include "test_strategy.h"
#include "strategy/skills/kicktopointomni.h"
#include "strategy/skills/dribbletopoint.h"
#include "strategy/behaviors/genericmovementbehavior.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "strategy/behaviors/defendbehavior.h"
#include "strategy/behaviors/goalie.h"
#include "strategy/behaviors/attackmain.h"
#include "strategy/behaviors/attacksupport.h"
#include "strategy/behaviors/refstop.h"
#include "strategy/behaviors/penaltygoalie.h"
#include "strategy/behaviors/wall.h"
#include "ctime"
#include "model/ball.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
#include "model/team.h"

using std::endl, std::cout;
/************************************************************************/
//Test Behaviors Section

// Test behavior to repeatedly kick the ball to the center
// Waits 6 seconds between kicks
class KickBeh : public Behavior
{
    KickToPointOmni* ktpo;
    std::clock_t start;
    bool wait4recharge;
public:
    KickBeh(Robot* robot) : Behavior(robot) {
        ktpo = new KickToPointOmni(robot,Point(0,0),-1,-1,false);
        wait4recharge = false;
    }
    ~KickBeh() {
        delete ktpo;
    }
    bool perform() override {
        bool finished = false;

        // Perform the kick skill as long as we are not recharging
        if(!wait4recharge)
            finished = ktpo->perform();

        // If finished with kicking, we must recharge for 6 seconds
        // So do nothing
        if(finished)
        {
            wait4recharge = true;
            start = std::clock();
            robot->setTargetVelocity(Point(0,0),0);
        }

        // If 6 seconds have passed since starting recharge,
        // We can kick again
        double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        if(duration >= 6)
            wait4recharge = false;
    }

    string getName() override{
        return "Kick";
    }
};

// Test behavior to dribble the ball between two points
class DribbleToPointBeh : public Behavior
{
    DribbleToPoint* skill;
    Point A = Point(1500, 0);
    Point B = Point(-1500, 0);
    Point* target;
public:
    DribbleToPointBeh(Robot* robot) : Behavior(robot)
    {
        target = &A;
        skill = new DribbleToPoint(robot,target, false, false);
    }

    ~DribbleToPointBeh()
    {
        delete skill;
    }

    bool perform() override
    {
        if(skill->perform())
        {
            if((*target) == A)
                target = &B;
            else
                target = &A;
        }
    }

    string getName() override {
        return "Dribble to point";
    }
};

// Test behavior to dribble the ball between two points
class DribbleBackBeh : public Behavior
{
    DribbleBack* skill;
    Point target;
public:
    DribbleBackBeh(Robot* robot) : Behavior(robot)
    {
        target = Point(0,0);
        skill = new DribbleBack(robot,target);
    }

    ~DribbleBackBeh()
    {
        delete skill;
    }

    bool perform() override
    {
        skill->perform();
    }
    string getName() override {
        return "Dribble Back";
    }
};

// Test behavior to rotate to the ball
class RotBeh : public GenericMovementBehavior
{
    RotBeh(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot) {

    }

    bool perform() override
    {
        float ang = Measurements::angleBetween(robot, *game_state->getBall());
        cmd.setTarget(robot->getPosition(),ang);
        GenericMovementBehavior::perform();
    }

    string getName() override {
        return "Rot";
    }

};

// Test behavior to dribble when ball is close
class DribbleBeh : public Behavior
{
    DribbleBeh(Robot* robot) : Behavior(robot) {

    }

    bool perform() override
    {
        robot->setDribble(true);
    }

    string getName() override {
        return "Dribble";
    }
};

// Test behavior to move to a point and print distance and angle errors
class MotionTestBeh : public GenericMovementBehavior
{
public:

    MotionTestBeh(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot) {

    }

    bool perform() override
    {
        Point target = Point(1500,0);
        cmd.setTarget(target,0);
        cmd.avoidBall = cmd.avoidObstacles = false;

        std::cout << "Distance Error: " << Measurements::distance(robot->getPosition(),target) << std::endl;
        std::cout << "Angle Error in Degrees: " << robot->getOrientation()*180/M_PI << std::endl;
        GenericMovementBehavior::perform();
    }

    string getName() override {
        return "Motion Test";
    }
};

// Test behavior to move the robot from (-2000,0) to (2000,0)
// while facing the ball
class Strafe : public GenericMovementBehavior
{
public:
    Point A = Point(1200,0), B = Point(-1200,0);
    enum {pos_one,pos_two} state = pos_one;
    Strafe(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot){}
    Strafe(Robot* robot, Point A, Point B) :
        Behavior(robot), GenericMovementBehavior(robot), A(A), B(B){}

    bool perform() override
    {
        Point rp = robot->getPosition();
        double ori = Measurements::angleBetween(rp, *game_state->getBall());
        switch(state)
        {
            case pos_one:
                cmd.setTarget(A,ori);
                cmd.avoidBall = cmd.avoidObstacles = true;
                if (Measurements::isClose(rp,A,DIST_TOLERANCE))
                state = pos_two;
                break;
            case pos_two:
                cmd.setTarget(B,ori);
                cmd.avoidBall = cmd.avoidObstacles = true;
                if (Measurements::isClose(rp,B,DIST_TOLERANCE))
                state = pos_one;
        }
        GenericMovementBehavior::perform();
    }

    string getName() override {
        return "Strafe";
    }
};


// Moves robot to the given position
class GoToBehavior : public GenericMovementBehavior
{
public:
    GoToBehavior(Robot* robot, double angle):
        Behavior(robot), GenericMovementBehavior(robot)
    {
        offset += Point(cos(angle*M_PI/180), sin(angle*M_PI/180)) * 500;
    }

    Point offset;

    bool perform() override  {
        cout<< "/*performing*/: r"<< robot->getID() << endl;
        Point b = *game_state->getBall();
        cmd.setTarget( b + offset, 0);
                     // Measurements::angleBetween(robot->getPosition(),robot->getTeam()->getGameState()->getBall()->getPosition()));
        cmd.avoidBall = cmd.avoidObstacles = false;
        GenericMovementBehavior::perform();
    }

    string getName() override {
        return "GoTo";
    }
};


TestStrategy::TestStrategy(RobotTeam* _team) : Strategy(_team) {

}


#include "robot/navigation/commands/CmdGoToPose.h"
void TestStrategy::assignBehaviors()
{
    std::cout << "Assigning test strategy behaviors " << std::endl;
//    //team->getRobot(0)->assignBeh<GenericMovementBehavior>(Point(-650, 300), 0);

    auto r = team->getRobot(6);
    if(r){
        std::cout << "Go to pose" << endl;
        CmdGoToPose cmd = CmdGoToPose(Point(-650,-1500),0,true,true);
        r->goToPose(cmd);
    }
//    r->assignBeh<GoToBehavior>(0);

//    auto robots = team->getRobots();
//    int num_robots = robots.size();
//    int i = 0;
//    for(auto r : robots){
//        if(i++ ==0) r->assignBeh<Strafe>();
//        else r->assignBeh<GoToBehavior>(i*360.0/num_robots);

//    }

}

int TestStrategy::getStatus()
{
    return STATUS_RUNNING;
}


QString TestStrategy::getName(){
    return "Test";
}
