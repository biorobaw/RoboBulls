#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "skill/kicktopointomni.h"
#include "skill/kick.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "behavior/defendfarfromball.h"
#include "behavior/rotateonpoint.h"

/************************************************************************/

class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
public:
    KickBeh() {
        ktpo = new Skill::KickToPointOmni(Point(0,0));
    }

    void perform(Robot * robot) override
    {
        ktpo->perform(robot);
    }
};

class RotBeh : public GenericMovementBehavior
{
    void perform(Robot * robot) override
    {
        float ang = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(robot->getRobotPosition(), ang);
        GenericMovementBehavior::perform(robot);
    }
};

class GoToBeh : public GenericMovementBehavior
{
    void perform(Robot * robot) override
    {
        setMovementTargets(Point(0,0), 0);
        GenericMovementBehavior::perform(robot);
    }
};
class ShamsiStrafe : public GenericMovementBehavior
{
public:
    //The robot moves between (-2000,0) and (2000,0)
    //While constantly facing the ball
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


bool TestStrategy::update()
{
    return false;
}

void TestStrategy::assignBeh()
{
    //Point bp = gameModel->getBallPoint();
    //gameModel->findMyTeam(8)->assignBeh<RotateOnPoint>(bp, Measurments::angleBetween(bp, gameModel->getOpponentGoal()),400);
    gameModel->findMyTeam(8)->assignBeh<KickBeh>();

//    gameModel->findMyTeam(1)->assignBeh<DefendBehavior>();
//    gameModel->findMyTeam(2)->assignBeh<DefendBehavior>();
//    gameModel->findMyTeam(3)->assignBeh<DefendBehavior>();
//    gameModel->findMyTeam(5)->assignBeh<DefendFarFromBall>();
}
