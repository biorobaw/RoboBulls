#include "goaliebehavior.h"

GoalieBehavior::GoalieBehavior()
    : idlePoint(gameModel->getMyGoal() + Point(ROBOT_RADIUS+50,0))
    , kick_skill(nullptr)
    , def_area(OUR_TEAM)
{
    dribble_skill = new Skill::DribbleBack(idlePoint);
    kick_skill = new Skill::KickToPointOmni(kickPoint);
}

GoalieBehavior::~GoalieBehavior()
{
    delete kick_skill;
    delete dribble_skill;
}

void GoalieBehavior::perform(Robot *robot)
{
    robot->setDrible(false);
    Point bp = gameModel->getBallPoint();
    float angleToBall = Measurements::angleBetween(robot, bp);
    Robot* ballBot = gameModel->getHasBall();

    //Segment to hold ballOnRobotIsAimedAtOurGoal and isBallMovingTowardsGoal return
    std::pair<Point,Point> lineSegment;

    // If there is a robot with the ball facing our goal, we move to get in it's trajectory.
    if(ballBot && ballBot->getID() != GOALIE_ID && botOnBallIsAimedAtOurGoal(ballBot, lineSegment))
    {
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(1.5);
        setMovementTargets(blockPoint, angleToBall, false, false);
        GenericMovementBehavior::perform(robot);
    }

    // If the ball is moving towards goal, we move to get into the line of trajectory.
    else if(isBallMovingTowardsGoal(lineSegment) && !shouldRetrieveBall())
    {
        std::cout << "Moving to goal" << std::endl;
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(1.5);
        setMovementTargets(blockPoint, angleToBall, false, false);
        GenericMovementBehavior::perform(robot);
    }

    // If the ball is stopped just outside our defence area, we dribble it inside
    // the defence area if safe
    else if (shouldRetrieveBall())
        dribble_skill->perform(robot);

    // Kick the ball away if it is inside the defense area
    else if(shouldClearBall())
    {
        kickPoint = Point(0, 0);
        kick_skill->perform(robot);
    }

    // If the the ball is in play and not outside the field, opponents can try to score.
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

        if(def_area.contains(intercept_point,-ROBOT_RADIUS))
        {
            setVelocityMultiplier(1.5);
            setMovementTargets(intercept_point, angleToBall, false, false);
        }
        else
        {
            setVelocityMultiplier(1);
            setMovementTargets(idlePoint, Measurements::angleBetween(robot, Point(0,0)),false,false);
        }
        GenericMovementBehavior::perform(robot);
    }

    // TODO: Insert ball is moving condition to defend against deflections

    // Otherwise we are just idling at the idle point, facing the center
    else
    {
        setVelocityMultiplier(1);
        setMovementTargets(idlePoint, Measurements::angleBetween(robot, Point(0,0)));
        GenericMovementBehavior::perform(robot);
    }
}

bool GoalieBehavior::isBallMovingTowardsGoal(std::pair<Point,Point>& lineSegOut)
{
    // Filter out balls not moving towards goal
    Point goal = gameModel->getMyGoal();
    Point bVel = gameModel->getBallVelocity();
    if(bVel.x > 0)
        return false;

    // Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    // Set the output to a pair of Points representing the line of the ball's trajectory.
    lineSegOut = {ballPos, Point(goal.x+ROBOT_RADIUS,y)};

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

    Point endPoint = Point( myGoalPos.x + ROBOT_RADIUS, yAtGoalLine );

    // Write to the line segment variable
    lineSegOut = std::make_pair(ballPos, endPoint);

    // If the y coordinate is within the range of the goal then return true
    return yAtGoalLine > -GOAL_WIDTH/2 && yAtGoalLine < GOAL_WIDTH/2;
}

bool GoalieBehavior::isBallReachable()
{
    return abs(Measurements::angleBetween(idlePoint, gameModel->getBallPoint()) <= M_PI_2);
}

bool GoalieBehavior::shouldClearBall()
{
    // We only clear the ball when it is inside the defence area
    // and moving slow enough
    return def_area.contains(gameModel->getBallPoint()) && gameModel->getBallSpeed() <= 100;
}

bool GoalieBehavior::shouldRetrieveBall()
{
    // We bring the ball into the defense area if it is
    // - closer than one robot radius outside the defense area
    // - farther than ball diameter inside the defense area
    // - moving slow enough
    // - closer to one of our robots than to an opponent robot
    Point bp = gameModel->getBallPoint();
    Robot* nearest_opp = Comparisons::distance(bp).minOppTeam();
    Robot* nearest_teammate = Comparisons::distance(bp).minMyTeam();

    bool result = def_area.contains(bp, ROBOT_RADIUS*2)
    && !def_area.contains(bp, -2*BALL_RADIUS)
    && (gameModel->getBallSpeed() <= 100)
    && (Measurements::distance(nearest_opp, bp)
       - Measurements::distance(nearest_teammate, bp) > ROBOT_RADIUS);

    return result;
}

