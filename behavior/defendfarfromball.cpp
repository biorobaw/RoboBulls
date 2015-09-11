#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"
#include "gui/guiinterface.h"

//The distance from the goal where the robot stays idle
#define IDLE_DISTANCE 300

//The distance from the goal where the robot stays to block a possible kick
#define BLOCK_DISTANCE 600

//Currently unused? Other Strategies broken
int DefendFarFromBall::goalieDist = 900;

DefendFarFromBall::DefendFarFromBall()
    : idlePoint(gameModel->getMyGoal() + Point(IDLE_DISTANCE,0) * GameModel::opSide)
{

}

bool DefendFarFromBall::isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut)
{
    //Filter out balls not moving towards goal
    Point goal = gameModel->getMyGoal();
    Point bVel = gameModel->getBallVelocity();
    if(signbit(goal.x) != signbit(bVel.x) || abs(bVel.x) < 0.1)
        return false;

    //Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    //Set the output to a pair Points representing the line of the ball's trajectory.
    lineEndsOut = {ballPos, Point(goal.x,y)};

    //Is the Y position within the goalie box?
    return (y > -GOAL_WIDTH*0.75) && (y < GOAL_WIDTH*0.75);
}

void DefendFarFromBall::perform(Robot *robot)
{
    float angleToBall = Measurments::angleBetween(robot, gameModel->getBallPoint());
    Robot* ballBot = gameModel->getHasBall();

    /* If the ball is moving torwards goal, we move to get into the line of trajectory.
     * Otherwise, if the ball is not moving to the goal, we remain stationary */
    std::pair<Point,Point> lineEnds;
    if(isBallMovingTowardsGoal(lineEnds))  {
        Point movePoint = Measurments::linePoint(robot->getRobotPosition(), lineEnds.first, lineEnds.second);
        GuiInterface::getGuiInterface()->drawPath(lineEnds.first, lineEnds.second);
        setMovementTargets(movePoint, angleToBall);
    }
    else if (ballBot != NULL && !ballBot->isOnMyTeam()) {
        //If there is a robot with the ball, we move to get in it's way.
        //There's some logic here not to follow following out-of-view balls
        Point myGoal = gameModel->getMyGoal();
        double  blockAng = Measurments::angleBetween(idlePoint, ballBot);
        double centerAng = Measurments::angleBetween(idlePoint, Point(0,0));
        double direction = blockAng;
        if(abs(blockAng) > 120*(M_PI/180))
            direction = centerAng;
        Point blockPoint(cos(direction)*BLOCK_DISTANCE + myGoal.x, sin(direction)*BLOCK_DISTANCE + myGoal.y);
        setMovementTargets(blockPoint, angleToBall);
    } else {
        //Otherwise we are just idling at the idle point
        setMovementTargets(idlePoint, angleToBall);
    }

    GenericMovementBehavior::perform(robot);
}

