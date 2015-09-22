#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"
#include "gui/guiinterface.h"

//The distance from the goal where the robot stays idle
#define IDLE_DISTANCE 300

//The distance from the goal where the robot stays to block a possible kick
#define BLOCK_DISTANCE 600

//Percent of goal width to respond to a line. Can be more than 1
#define GOAL_WIDTH_PCT (GOAL_WIDTH * 1.0)

//How close must the ball before we go to kick it away?
int DefendFarFromBall::goalieDist = 900;

DefendFarFromBall::DefendFarFromBall()
    : idlePoint(gameModel->getMyGoal() + Point(IDLE_DISTANCE,0) * GameModel::opSide)
    , isKickingBallAway(false)
    , kick_skill(nullptr)
    { }

DefendFarFromBall::~DefendFarFromBall()
{
    delete kick_skill;
}

bool DefendFarFromBall::isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut)
{
    //Filter out balls not moving towards goal
    Point goal = gameModel->getMyGoal();
    Point bVel = gameModel->getBallVelocity();
    if(signbit(goal.x) != signbit(bVel.x) || abs(bVel.x) < 0.05)
        return false;
    if(isBallBehindGoal())
        return false;

    //Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    //Set the output to a pair Points representing the line of the ball's trajectory.
    lineEndsOut = {ballPos, Point(goal.x,y)};

    //Is the Y position within the goalie box?
    return (y > GOAL_WIDTH_PCT) && (y < GOAL_WIDTH_PCT);
}

//TODO: Combine with isBallMovingTowardsGoal
bool DefendFarFromBall::ballOnRobotIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut)
{
    /* Return false automatically if the robot's orientation is facing a direction
    opposite to that of our goal */
    Point myGoalPos = gameModel->getMyGoal();
    float orientation = robot->getOrientation();
    if ((myGoalPos.x > 0 && !(orientation > - M_PI/2 && orientation < M_PI/2)) ||
        (myGoalPos.x < 0 &&   orientation > - M_PI/2 && orientation < M_PI/2))
        return false;

    /* Essentially get a line that runs through the ball's position and has a slope
     * that is equivalent to where the robot is facing. We use the ball position because
     * the slope of the path will always be the robot's orientation but the ball
     * may be off center from the robot's kicker mechanism */
    float slope = tan( orientation );
    Point ballPos = gameModel->getBallPoint();

    // Extrapolate the line to retrieve the y-coordinate at the x-coordinate of the goal
    float yAtGoalLine = slope * ( myGoalPos.x - ballPos.x ) + ballPos.y;

    Point endPoint = Point( myGoalPos.x, yAtGoalLine );

    // Write to the line segment variable
    lineSegOut = std::make_pair(ballPos, endPoint);

    // If the y coordinate is within the range of the goal then return true
    return yAtGoalLine > -GOAL_WIDTH_PCT && yAtGoalLine < GOAL_WIDTH_PCT;
}

bool DefendFarFromBall::isBallBehindGoal()
{
    Point ball = gameModel->getBallPoint();
    return abs(Measurments::angleBetween(idlePoint, ball)) > 90*(M_PI/180);
}

void DefendFarFromBall::perform(Robot *robot)
{
    Point ball = gameModel->getBallPoint();
    float angleToBall = Measurments::angleBetween(robot, ball);
    Robot* ballBot = gameModel->getHasBall();

    //Segment to hold ballOnRobotIsAimedAtOurGoal return
    std::pair<Point,Point> facingSegment;

    /* If the ball is moving torwards goal, we move to get into the line of trajectory.
     * Otherwise, if the ball is not moving to the goal, we remain stationary */
    std::pair<Point,Point> lineEnds;
    if(isBallMovingTowardsGoal(lineEnds))  {
        Point movePoint = Measurments::linePoint(robot->getRobotPosition(), lineEnds.first, lineEnds.second);
        GuiInterface::getGuiInterface()->drawPath(lineEnds.first, lineEnds.second);
        setMovementTargets(movePoint, angleToBall, false, false);
    }
    else if(ballBot && ballBot->getID() != GOALIE_ID
            && ballOnRobotIsAimedAtOurGoal(ballBot, facingSegment)) {
        //If there is a robot with the ball, we move to get in it's way.
        //If the angle of this bot is too large, we face the center not to follow out-of-view balls
        Point nearestPointOnLine = Measurments::linePoint(robot->getRobotPosition(), facingSegment.first, facingSegment.second);
        GuiInterface::getGuiInterface()->drawPath(facingSegment.first, facingSegment.second, 0.1);
        GuiInterface::getGuiInterface()->drawPath(robot->getRobotPosition(), nearestPointOnLine, 0.1);
        setMovementTargets(nearestPointOnLine, angleToBall, false, false);
    }
    else if(!isKickingBallAway && Measurments::distance(ball, idlePoint) < goalieDist) {
        //If we're not kicking and the ball is close to the goal, we want to kick it away.
        isKickingBallAway = true;
        kick_skill = new Skill::KickToPointOmni(Point(0,0));
    }
    else if(isKickingBallAway) {
        //We are actively kicking the ball away
        if(kick_skill->perform(robot) || Measurments::distance(ball,idlePoint) > goalieDist) {
            isKickingBallAway = false;
            delete kick_skill;
            kick_skill = nullptr;
        }
    }
    else {
        //Otherwise we are just idling at the idle point
        setMovementTargets(idlePoint, angleToBall);
    }

    //If we are not kicking, we just use generic move. Otherwise, kick_skill moves the robot
    //and this cannot be called
    if(!isKickingBallAway) {
        GenericMovementBehavior::perform(robot);
    }
}

