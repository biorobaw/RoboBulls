#include <math.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "defendfarfromball.h"
#include "gui/guiinterface.h"

//The distance from the goal where the robot stays idle
#define IDLE_DISTANCE 400

//Percent of goal width to respond to a line. Can be more than 1
#define GOAL_WIDTH_PCT (GOAL_WIDTH * 1.5)

//How close must the ball before we go to kick it away?
int DefendFarFromBall::goalieDist = 900;

DefendFarFromBall::DefendFarFromBall()
    : idlePoint(gameModel->getMyGoal() + Point(IDLE_DISTANCE,0))
    , isKickingBallAway(false)
    , isIdling(false)
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
    if(bVel.x > 0) //|| abs(bVel.x) < 0.005)
        return false;

    //Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    //Set the output to a pair Points representing the line of the ball's trajectory.
    lineEndsOut = {ballPos, Point(goal.x,y)};

    //Is the Y position within the goalie box?
    return (y > -GOAL_WIDTH_PCT) && (y < GOAL_WIDTH_PCT);
}

//TODO: Combine with isBallMovingTowardsGoal
bool DefendFarFromBall::ballOnRobotIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut)
{
    /* Return false automatically if the robot's orientation is facing a direction
    opposite to that of our goal */
    Point myGoalPos = gameModel->getMyGoal();
    float orientation = robot->getOrientation();
    if (orientation > -M_PI/2 && orientation < M_PI/2)
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

bool DefendFarFromBall::isBallUnreachable()
{
    Point ball = gameModel->getBallPoint();
    bool angleTest = abs(Measurments::angleBetween(idlePoint, ball)) > 90*(M_PI/180);
    return angleTest;
}

void DefendFarFromBall::perform(Robot *robot)
{
    Point ball = gameModel->getBallPoint();
    float angleToBall = Measurments::angleBetween(robot, ball);
    Robot* ballBot = gameModel->getHasBall();
    GuiInterface* gui = GuiInterface::getGuiInterface();

    //Segment to hold ballOnRobotIsAimedAtOurGoal and isBallMovingTowardsGoal return
    std::pair<Point,Point> lineSegment;

    //Assume not idling--if we are, it will be set to true below
    isIdling = false;

    if(isKickingBallAway) {
        //We are actively kicking the ball away
        //We will stop kicking if it becomes unreach able or we actually kick it away
        if(kick_skill->perform(robot) || isBallUnreachable() ||
                Measurments::distance(ball,idlePoint) > goalieDist) {
            isKickingBallAway = false;
            delete kick_skill;
            kick_skill = nullptr;
        }
    }
    else if(!isKickingBallAway && Measurments::distance(ball, idlePoint) < goalieDist) {
        //If we're not kicking and the ball is close to the goal, we want to kick it away.
        isKickingBallAway = true;
        kick_skill = new Skill::KickToPointOmni(Point(0,0));
    }
    else if(isBallMovingTowardsGoal(lineSegment))  {
        /* If the ball is moving torwards goal, we move to get into the line of trajectory.
         * But we only move if the nearest point is near the goal */
        Point movePoint = Measurments::linePoint(robot->getRobotPosition(), lineSegment.first, lineSegment.second);
        if(Measurments::distance(movePoint, idlePoint) < goalieDist) {
            gui->drawPath(lineSegment.first, lineSegment.second);
            setVelocityMultiplier(1.5);
            setMovementTargets(movePoint, angleToBall, false, false);
        } else {
            setMovementTargets(idlePoint, angleToBall, false, false);
        }
    }
    else if(ballBot && ballBot->getID() != GOALIE_ID && ballOnRobotIsAimedAtOurGoal(ballBot, lineSegment)) {
        //If there is a robot with the ball facing our goal, we move to get in it's trajectory.
        //But we only move if the nearest point is near the goal
        Point nearestPointOnLine = Measurments::linePoint(robot->getRobotPosition(), lineSegment.first, lineSegment.second);
        if(Measurments::distance(nearestPointOnLine, idlePoint) < goalieDist) {
            gui->drawPath(lineSegment.first, lineSegment.second, 0.1);
            gui->drawPath(robot->getRobotPosition(), nearestPointOnLine, 0.1);
            setMovementTargets(nearestPointOnLine, angleToBall, false, false);
        } else {
            setMovementTargets(idlePoint, angleToBall, false, false);
        }
    }
    else {
        //Otherwise we are just idling at the idle point, facing the center if it is unreachable,
        // or to the ball otherwise
        float facingAngle = isBallUnreachable() ? Measurments::angleBetween(robot,Point(0,0)) : angleToBall;
        setVelocityMultiplier(1);
        setMovementTargets(idlePoint, facingAngle);
        isIdling = true;
    }

    //If we are not kicking, we just use generic move. Otherwise, kick_skill moves the robot
    //and this cannot be called
    if(!isKickingBallAway) {
        auto type = isIdling ? Movement::Type::StayStill : Movement::Type::Default;
        GenericMovementBehavior::perform(robot, type);
    }
}
