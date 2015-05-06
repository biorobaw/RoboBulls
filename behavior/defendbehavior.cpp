#include "include/config/simulated.h"
#include "utilities/point.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "skill/kicktopointomni.h"

DefendState::DefendState() {

}

DefendState::~DefendState() {

}

#if SIMULATED
 #define LINE_DISTANCE 400
 #define GOALIE_DIST   800
#else
 #define LINE_DISTANCE 1200
 #define GOALIE_DIST   300
#endif

/************************************************************/

std::array<Point,10> DefendStateIdle::claimed;

DefendStateIdle::DefendStateIdle()
    : hasChosenPoint(false)
{
    std::cout << "DefendStateIdle Created" << std::endl;
}

DefendState* DefendStateIdle::action(Robot* robot)
{
    //Static points to position robots at
    static const Point defendPoints[] = {
        Point(1500,    0),
        Point(2000,  500),
        Point(2000, -500),
        Point(2500, 1000),
        Point(2500,-1000)
    };

    if(not hasChosenPoint) {
        for(const Point& p : defendPoints) {
            Point test(p.x*GameModel::mySide, p.y );
            if(std::find(claimed.begin(), claimed.end(), test) == claimed.end()) {
                claimed[robot->getID()] = test;
                hasChosenPoint = true;
                chosenPoint = test;
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
         * 2) The ball-to-predicion line distance to robot is small
         * 3) The balls speed is significant
         * 4) the ball is heading forwards the goal
         */
        Point bp  = gameModel->getBallPoint();
        Point bv  = gameModel->getBallVelocity();
        Point bpr = gameModel->getBallPrediction();
        Point gl  = gameModel->getMyGoal();
        float bs  = gameModel->getBallSpeed();
        float velang = atan2(bv.y, bv.x);

        if( ( abs(bpr.x - gl.x) < 2900 ) &&
            ( Measurments::lineDistance(chosenPoint, bp, bpr) < LINE_DISTANCE ) &&
            ( bs > 0.2 ) &&
            ( Measurments::isClose(velang, Measurments::angleBetween(bp, gl), 90*(M_PI/180))))
        {
           //We are going to kick, so un-claim the current sitting point
           claimed[robot->getID()] = Point(0,0);
           return new DefendStateKick();
        }
    }

    return this;
}

/************************************************************/

//Current ID of who is kicking to ball, to only keep one robot going after it
int DefendStateKick::whoIsKicking = 0;

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
        //The conditions to go to this state validate the ball is RIGHT NOW
        //Heading to us. We go to the closet point on the ball's path.
        linePoint = Measurments::linePoint(robot->getRobotPosition(), bp, bpp);
        if(abs(goal.x - linePoint.x) < 2500)
            chosenLinePoint = true;
    }
    else {
        //Inside here we are just in the line of the ball and are waiting
        if(not(kickingBall)) {
            float ballRobAng = Measurments::angleBetween(robot, bp);
            setMovementTargets(linePoint, ballRobAng);
            GenericMovementBehavior::perform(robot);

            //If true, the ball has arrived at us, so we got to kick it
            if(Measurments::distance(robot, bp) < 600 &&
               Comparisons::distanceBall().minMyTeam() == robot &&
               whoIsKicking == -1) {
                whoIsKicking = robot->getID();
                ktpo = new Skill::KickToPointOmni(gameModel->getOpponentGoal());
                kickingBall = true;
            }
        }
        else {
            //Here we are actually kicking the ball away.
            //Timeout is reset if a kick is made.
            if(ktpo->perform(robot))
                kickBallTimeout = 0;

            //Second condition to return to idle. if we kicked, return.
            //Also stop if it moves too far awaytoo fara way, or
            if(ballMovingAway() || Measurments::distance(robot, bp) > LINE_DISTANCE*1.5) {
                whoIsKicking = -1;
                return new DefendStateIdle();
            }
        }

        //Tiemout conditions. If we have waited too long, or the ball is
        //is too close to the goal, we go back to idle.
        if(++kickBallTimeout > 400
                || Measurments::distance(goal, bp) < GOALIE_DIST //Goalie action distance
                || ballMovingAway()) {
            std::cout << "DefendStateKick Timeout1 " << robot->getID() << std::endl;
            whoIsKicking = -1;
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
    return (bs > 0.25) && !(Measurments::isClose(ba, ballGoalAng, 90*(M_PI/180)));
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
