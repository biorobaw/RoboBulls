#include "include/config/simulated.h"
#include "utilities/comparisons.h"
#include "behavior/defendbehavior.h"
#include "skill/kicktopointomni.h"

/* Check to see if kicking is done or not.
 * It happens when the ball has high velocity that is not facing
 * our goal. This means one of us kicked.
 */
static bool ballIsMovingAway()
{
    Point bp = gameModel->getBallPoint();
    Point bv = gameModel->getBallVelocity();
    float bs = gameModel->getBallSpeed();
    float ba = atan2(bv.y, bv.x);
    float ballGoalAng = Measurments::angleBetween(bp, gameModel->getMyGoal());
    return (bs > 0.25) && !(Measurments::isClose(ba, ballGoalAng, 90*(M_PI/180)));
}

/* Return true when we think the ball is stopped.
 */
static bool ballIsStopped()
{
    return gameModel->getBallSpeed() < 0.25;
}

/************************************************************/

Point DefendState::claimed[10];
int   DefendState::whoIsKicking = -1;

void DefendState::clearClaimedPoints()
{
    for(Point& p : claimed)
        p = Point(0,0);
}

DefendState::~DefendState() {

}

/************************************************************/

#if SIMULATED
 #define LINE_DISTANCE 400
 #define GOALIE_DIST   800
#else
 #define LINE_DISTANCE 1200
 #define GOALIE_DIST   300
#endif

DefendStateIdle::DefendStateIdle()
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

    if(claimed[robot->getID()] == Point(0,0)) {
        for(const Point& p : defendPoints) {
            Point test(p.x * GameModel::mySide, p.y);
            if(std::find(claimed, claimed+10, test) == claimed+10) {
                //Here we ensure there is no other robot there
                Robot* closest = Comparisons::distance(test).minMyTeam();
                if(closest->getID() != robot->getID() 
                    && Measurments::distance(test, closest) < ROBOT_RADIUS) {
                    continue;
                }
                claimed[robot->getID()] = test;
                break;
            }
        }
    }
    else {
        //Idle at the point, facing the ball
        Point& chosenPoint = claimed[robot->getID()];
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
           return new DefendStateKick();
        }

        /* And here, we check if the ball is sitting near us. If it is, kick it back.
         * I'm tired of this not being here.
         * \"If the ball is close AND nobody is kicking AND I'm closest to ball...
         * \"AND the ball near the goal AND the ball is not moving away...\" Kick it.
         */
        if(Measurments::distance(bp, robot) < 4*LINE_DISTANCE
            && Measurments::distance(bp, gl) < 2500
            && whoIsKicking == -1
            && not(ballIsMovingAway())
            && Comparisons::distanceBall().minMyTeam() == robot)
        {
            whoIsKicking = robot->getID();
            return new DefendStateIdleKick();
        }
    }

    return this;
}

/************************************************************/

DefendStateIdleKick::DefendStateIdleKick()
{
    std::cout << "DefendStateIdleKick Created" << std::endl;
    ktpo = new Skill::KickToPointOmni(gameModel->getOpponentGoal());
}

DefendStateIdleKick::~DefendStateIdleKick()
{
    delete ktpo;
}

DefendState* DefendStateIdleKick::action(Robot* robot)
{
    if(ktpo->perform(robot) || ballIsMovingAway()) {
        whoIsKicking = -1;
        return new DefendStateIdle();
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
    setMovementTolerances(DIST_TOLERANCE/10, ROT_TOLERANCE);
}

DefendStateKick::~DefendStateKick()
{
    delete ktpo;
    whoIsKicking = -1;
}

DefendState* DefendStateKick::action(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    Point goal = gameModel->getMyGoal();

    if(not(chosenLinePoint)) {
        //The conditions to go to this state validate the ball is RIGHT NOW
        //Heading to us. We go to the closet point on the ball's path.
        if(!tryGetValidLinePoint(robot)) {
            return new DefendStateIdle();
        }
    }
    else {
        //Inside here we are just in the line of the ball and are waiting
        if(not(kickingBall)) {
            tryGetValidLinePoint(robot);
            float ballRobAng = Measurments::angleBetween(robot, bp);
            setMovementTargets(linePoint, ballRobAng);
            GenericMovementBehavior::perform(robot);

            //If the ball is close, and we are the closet to the ball,
            //and nobody else is kicking, we kick.
            if(Measurments::distance(robot, bp) < 600
                && whoIsKicking == -1
                && Comparisons::distanceBall().minMyTeam() == robot) 
            {
                whoIsKicking = robot->getID();
                ktpo = new Skill::KickToPointOmni(gameModel->getOpponentGoal());
                kickingBall = true;
            }

            //If the ball is coming and it stops, go back to idle.
            if(ballIsStopped()) {
                return new DefendStateIdle();
            }
        }
        else {
            //Here we are actually kicking the ball away.
            //Timeout is reset if a kick is made.
            if(ktpo->perform(robot))
                kickBallTimeout = 0;

            //Second condition to return to idle. Stop if it moves too far away
            if(Measurments::distance(robot, bp) > LINE_DISTANCE*3) {
                return new DefendStateIdle();
            }
        }

        //Tiemout conditions. If we have waited too long, or the ball is
        //is too close to the goal, we go back to idle.
        if(++kickBallTimeout > 400
                || Measurments::distance(goal, bp) < GOALIE_DIST //Goalie action distance
                || ballIsMovingAway()) {
            std::cout << "DefendStateKick Timeout " << robot->getID() << std::endl;
            return new DefendStateIdle();
        }
    }

    return this;
}

/* Tries to get a valid line point along the ball's projected path.
 * If it does find one, sets this->chosenLinePoint to it and
 * returns true.
 */
bool DefendStateKick::tryGetValidLinePoint(Robot* r)
{
    Point bp = gameModel->getBallPoint();
    Point bpp= gameModel->getBallPrediction();
    Point goal = gameModel->getMyGoal();
    Point p = Measurments::linePoint(r->getRobotPosition(), bp, bpp);
    if(abs(goal.x - p.x) < 2500 && Measurments::distance(r, p) < LINE_DISTANCE*2) {
        linePoint = p;
        chosenLinePoint = true;
        return true;
    }
    return false;
}

/************************************************************/

int DefendBehavior::currentUsers = 0;

DefendBehavior::DefendBehavior() 
    : state(new DefendStateIdle())
{ 
        currentUsers += 1;
}

DefendBehavior::~DefendBehavior()
{
    //If 0 (Nobody on DefendBehavior), unclaim all points
    currentUsers -= 1;
    if(currentUsers == 0) {
        DefendState::clearClaimedPoints();
    }
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
