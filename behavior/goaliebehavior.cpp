#include "goaliebehavior.h"

//The distance from the goal where the robot stays idle
#define IDLE_DISTANCE ROBOT_RADIUS*2.5

GoalieBehavior::GoalieBehavior()
    : idlePoint(gameModel->getMyGoal() + Point(IDLE_DISTANCE,0))
    , isKickingBallAway(false)
    , isIdling(false)
    , kick_skill(nullptr)
    , def_area(OUR_TEAM)
    { }

GoalieBehavior::~GoalieBehavior()
{
    delete kick_skill;
}

void GoalieBehavior::perform(Robot *robot)
{
    Point ball = gameModel->getBallPoint();
    float angleToBall = Measurements::angleBetween(robot, ball);
    Robot* ballBot = gameModel->getHasBall();

    //Segment to hold ballOnRobotIsAimedAtOurGoal and isBallMovingTowardsGoal return
    std::pair<Point,Point> lineSegment;

    //Assume not idling--if we are, it will be set to true below
    isIdling = false;

    // We are actively kicking the ball away
    // We will stop kicking if it becomes unreachable or we actually kick it away
    if(isKickingBallAway)
    {
        if(kick_skill->perform(robot) || !isBallReachable() ||
           !def_area.contains(gameModel->getBallPoint()))
        {
            isKickingBallAway = false;
            delete kick_skill;
            kick_skill = nullptr;
        }
    }

    // If we're not kicking and the ball is in the defence area, we want to kick it away.
    else if(shouldClearBall())
    {
        std::cout << "Should Clear Ball" << std::endl;
        isKickingBallAway = true;
        kick_skill = new Skill::KickToPointOmni(Point(0,0));
    }

    // If there is a robot with the ball facing our goal, we move to get in it's trajectory.
    else if(ballBot && ballBot->getID() != GOALIE_ID && botOnBallIsAimedAtOurGoal(ballBot, lineSegment))
    {
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(3);
        setMovementTargets(blockPoint, angleToBall, false, false);
    }

    // If the ball is moving towards goal, we move to get into the line of trajectory.
    else if(isBallMovingTowardsGoal(lineSegment))
    {
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(3);
        setMovementTargets(blockPoint, angleToBall, false, false);
    }

    // If the the ball is free and not outside the field, opponents can try to score.
    // So, move to block the shortest path from the ball to the goal
    else if (isBallReachable())
    {
        // This is the point along the goal-post closest to the ball
        Point goal_point = Measurements::lineSegmentPoint(gameModel->getBallPoint(),
                                                   Point(gameModel->getMyGoal().x, GOAL_WIDTH/2),
                                                   Point(gameModel->getMyGoal().x, -GOAL_WIDTH/2));

        // This is the point along the line segment ball_point->goal_point closest to the robot
        Point intercept_point = Measurements::lineSegmentPoint(robot->getPosition(),
                                                        gameModel->getBallPoint(),
                                                        goal_point);

        if(def_area.contains(intercept_point))
        {
            setVelocityMultiplier(3);
            setMovementTargets(intercept_point, angleToBall, false, false);
        }
        else
        {
            setVelocityMultiplier(1);
            setMovementTargets(idlePoint, Measurements::angleBetween(robot, Point(0,0)));
        }
    }

    // TODO: Insert ball is moving condition to defend against deflections

    // Otherwise we are just idling at the idle point, facing the center
    else
    {
        setVelocityMultiplier(1);
        setMovementTargets(idlePoint, Measurements::angleBetween(robot, Point(0,0)));
        isIdling = true;
    }

    // If we are not kicking, we just use generic move. Otherwise, kick_skill moves the robot
    // and this cannot be called
    if(!isKickingBallAway) {
        auto type = isIdling ? Movement::Type::StayStill : Movement::Type::Default;
        GenericMovementBehavior::perform(robot, type);
    }
}

bool GoalieBehavior::isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut)
{
    // Filter out balls not moving towards goal
    Point goal = gameModel->getMyGoal();
    Point bVel = gameModel->getBallVelocity();
    if(bVel.x > 0)
        return false;

    // Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    // Set the output to a pair Points representing the line of the ball's trajectory.
    lineEndsOut = {ballPos, Point(goal.x,y)};

    // Is the Y position within the goalie box?
    return (y > -GOAL_WIDTH/2) && (y < GOAL_WIDTH/2);
}

bool GoalieBehavior::botOnBallIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut)
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
    return yAtGoalLine > -GOAL_WIDTH/2 && yAtGoalLine < GOAL_WIDTH/2;
}

bool GoalieBehavior::isBallReachable()
{
    return abs(Measurements::angleBetween(idlePoint, gameModel->getBallPoint())) < 90*(M_PI/180);
}

bool GoalieBehavior::shouldClearBall()
{
    // We only clear the ball when it is inside the defence area
    // and not moving
    return def_area.contains(gameModel->getBallPoint()) && gameModel->getBallSpeed() == 0;
}

