#include "utilities/point.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "skill/kicktopointomni.h"

DefendState::DefendState() {

}

DefendState::~DefendState() {

}

/************************************************************/

std::array<Point,10> DefendStateIdle::claimed;

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
        for(const Point& p : defendPoints) {
            Point test( copysign(p.x, gameModel->getMyGoal().x), p.y );
            if(std::find(claimed.begin(), claimed.end(), test) == claimed.end()) {
                claimed[robot->getID()] = test;
                hasChosenPoint = true;
                chosenPoint = test;
                std::cout << "ID " << robot->getID() << "Point " << test.toString() << std::endl;
                break;
            }
        }
    }
    else {
        //Idle at the point, facing the ball
        float ballRobAng = Measurments::angleBetween(robot, gameModel->getBallPoint());
        setMovementTargets(chosenPoint, ballRobAng);
        GenericMovementBehavior::perform(robot);

        /* If the ball is coming to us, and we are certain, we want to kick the ball.
         * Conditions checked are:
         * 1) The ball's predicted X is on our side of the field
         * 2) The ball-predicion line distance to robot is small
         * 3) the ball is heading forwards the goal
         */
        Point bp  = gameModel->getBallPoint();
        Point bv  = gameModel->getBallVelocity();
        Point bpr = gameModel->getBallPrediction();
        Point gl  = gameModel->getMyGoal();
        float bs  = gameModel->getBallSpeed();
        float velang = atan2(bv.y, bv.x);


        if( ( abs(bpr.x - gl.x) < 2900 ) &&
            ( Measurments::lineDistance(chosenPoint, bp, bpr) < 400 ) &&
            ( bs > 1.0 ) &&
            ( Measurments::isClose(velang, Measurments::angleBetween(bp, gl), 45*(M_PI/180))))
        {
           if(++ballComingCounter > 0) {
               //We are going to kick, so un-claim the current sitting point
               claimed[robot->getID()] = Point(0,0);
               return new DefendStateKick();
           }
        } else {
            ballComingCounter = 0;
        }
    }

    return this;
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
            //If kick performed, reset timer--we are probably close.
            //We go back to idle if it is moving away
            if(ktpo->perform(robot))
                kickBallTimeout = 0;

            if(ballMovingAway() || Measurments::distance(robot, bp) > 600)
                return new DefendStateIdle();
        }

        //If true, we have waited for a long time and the ball is not arriving.
        //We will go back to idling.
        if(++kickBallTimeout > 400) {
            std::cout << "DefendStateKick Timeout1 " << robot->getID() << std::endl;
            return new DefendStateIdle();
        }
    }

    return this;
}

/* Check to see if kicking is done or not.
 * It happens when the ball has high velocity that is not facing
 * our goal. This means one of us kicked.
 */
bool DefendStateKick::ballMovingAway()
{
    Point bp = gameModel->getBallPoint();
    Point bv = gameModel->getBallVelocity();
    float bs = gameModel->getBallSpeed();
    float ba = atan2(bv.y, bv.x);
    float ballGoalAng = Measurments::angleBetween(bp, gameModel->getMyGoal());
    return (bs > 1) && !(Measurments::isClose(ba, ballGoalAng, 200*(M_PI/180)));
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
