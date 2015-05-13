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
#include "utilities/edges.h"
#include "utilities/region.h"

/************************************************************************/

class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
public:
    KickBeh() {
        ktpo = new Skill::KickToPointOmni(Point(-1000 + rand()%2000,
                                                -1000 + rand()%2000));
    }

    void perform(Robot * robot) override
    {
        static float bs = 0.0;
        ktpo->perform(robot);

        bs = gameModel->getBallSpeed();
        if(posedge(bs > 1.5)) {
            prediction = gameModel->getBallPrediction();
        }
        if(posedge(bs < 0.5)) {
            Point bp = gameModel->getBallPoint();

            float dist = Measurments::distance(prediction, bp);

            std::cout << prediction.toString() << " " << bp.toString() << " "
                      << dist << std::endl;

            robot->clearCurrentBeh();
        }
    }

    Point prediction;
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
//    gameModel->findMyTeam(0)->assignBeh<KickBeh>();
//    Region lowerLeftRegion = Region(-3000,0,-2000,0);
//    Region topLeftRegion = Region(-3000,0,2000,0);
//    Region topRightRegion = Region(0,3000,0,2000);
//    Region lowerRightRegion = Region(0,3000,0,-2000);
//    Region goalLeftRegion = Region(-3000,2500,-700,700);
//    Region goalRightRegion = Region(2500, 3000, -700, 700);


    //return false;
}

void TestStrategy::assignBeh()
{
    gameModel->findMyTeam(8)->assignBeh<GoToBeh>();
}
