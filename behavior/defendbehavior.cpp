#include "utilities/point.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "skill/kicktopointomni.h"

DefendState::DefendState() {

}

DefendState::~DefendState() {

}

/************************************************************/

DefendStateIdle::DefendStateIdle()
    : hasChosenPoint(false)
    , ballComingCounter(0)
{
    std::cout << "DefendStateIdle Created" << std::endl;
}

DefendState* DefendStateIdle::action(Robot* robot)
{
    static const Point defendPoints[] = {
        Point(1500,    0),
        Point(2000,  500),
        Point(2000, -500),
        Point(2500, 1000),
        Point(2500,-1000)
    };

    if(not hasChosenPoint) {
        for(const Point& p : defendPoints)
        {
            Point test(copysign(p.x, gameModel->getMyGoal().x), p.y);
            if(not robotsCloseToPoint(robot, test)) {
                hasChosenPoint = true;
                chosenPoint = test;
                std::cout << "ID " << robot->getID() << "Point " << test.toString() << std::endl;
                break;
            }
        }
    }
    else {
        //If any robots are close to my point (besides me) panic.
        if(robotsCloseToPoint(robot, chosenPoint)) {
            hasChosenPoint = false;
        }

        //Idle at the point, facing the ball
        float ballRobAng = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(chosenPoint, ballRobAng);
        GenericMovementBehavior::perform(robot);

        //If the ball is coming to us, and we are certain, we want to kick the ball.
        Point bp  = gameModel->getBallPoint();
        Point bpr = gameModel->getBallPrediction();
        Point gl  = gameModel->getMyGoal();
        if(abs(bpr.x - gl.x) < 2700 && Measurments::lineDistance(chosenPoint, bp, bpr) < 300) {
           if(++ballComingCounter > 0)
               return new DefendStateKick();
        } else {
            ballComingCounter = 0;
        }
    }

    return this;
}

bool DefendStateIdle::robotsCloseToPoint(Robot* me, const Point &p)
{
    //Returns true if any robots besides me are < ROBOT_SIZE points to p
    return Comparisons::isDistanceToLess(p, ROBOT_SIZE).ignoreID(me).anyMyTeam();
}

/************************************************************/

DefendStateKick::DefendStateKick()
    : ktpo(nullptr)
    , chosenLinePoint(false)
    , kickingBall(false)
    , kickBallTimeout(0)
{
    std::cout << "DefendStateKick Created" << std::endl;
}

DefendStateKick::~DefendStateKick()
{
    delete ktpo;
}

DefendState* DefendStateKick::action(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    Point bpp= gameModel->getBallPrediction();
    Point goal = gameModel->getMyGoal();

    if(not(chosenLinePoint)) {
        //Supposed to happen: When ball is coming torwards robot...
        //If the bp and the prediction are close enough, just say go to the ball.
        if(Measurments::distance(bp, bpp) < 5) {
            linePoint = bp;
            chosenLinePoint = true;
        } else {
            linePoint = Measurments::linePoint(robot->getRobotPosition(), bp, bpp);
            if(abs(goal.x - linePoint.x) < 2000)
                chosenLinePoint = true;
        }

        //Sometimes happens: We go from kick to idle and back, if
        //we're close enough let it go
        if(Measurments::distance(robot, bp) < 200) {
            kickingBall = true;
            chosenLinePoint = true;
            ktpo = new Skill::KickToPointOmni(gameModel->getOpponentGoal());
        }
    }
    else {
        //Inside here we are just in the line of the ball and are waiting
        if(not(kickingBall)) {
            float ballRobAng = Measurments::angleBetween(robot, bp);
            setMovementTargets(linePoint, ballRobAng);
            GenericMovementBehavior::perform(robot);

            //If true, the ball has arrived at us, so we got to kick it
            if(Measurments::distance(robot, bp) < 600 &&
               Comparisons::distanceBall().minMyTeam() == robot) {
                ktpo = new Skill::KickToPointOmni(gameModel->getOpponentGoal());
                kickingBall = true;
            }
        }
        else {
            //Here we are actually kicking the ball away.
            //Go back to idle after kicking
            if(ktpo->perform(robot) || Measurments::distance(robot, bp) > 600)
                return new DefendStateIdle();
        }

        //If true, we have waited for a long time and the ball is not arriving.
        //We will go back to idling.

        if(++kickBallTimeout > 200) {
            std::cout << "DefendStateKick Timeout1 " << robot->getID() << std::endl;
            return new DefendStateIdle();
        }
    }

    return this;
}

/************************************************************/

DefendBehavior::DefendBehavior()
    : state(new DefendStateIdle)
    { }

DefendBehavior::~DefendBehavior()
{
    delete state;
}


void DefendBehavior::perform(Robot* robot)
{
    if(state != nullptr)
    {
        DefendState* nextState = state->action(robot);
        if(nextState != state) {
            delete state;
            state = nextState;
        }
    }
}
