#include "goalie.h"

Goalie::Goalie()
    : idlePoint(gameModel->getMyGoal() + Point(ROBOT_RADIUS+50,0))
    , kick_skill(nullptr)
    , def_area(GameModel::OUR_TEAM)
{
    dribble_skill = new Skill::DribbleBack(idlePoint);
    kick_skill = new Skill::KickToPointOmni(&kickPoint);
}

Goalie::~Goalie()
{
    delete kick_skill;
    delete dribble_skill;
}

void Goalie::perform(Robot *robot)
{
    robot->setDribble(false);
    Point bp = gameModel->getBallPoint();
    float angleToBall = Measurements::angleBetween(robot, bp);
    Robot* ballBot = gameModel->getHasBall();

    //Segment to hold ballOnRobotIsAimedAtOurGoal and isBallMovingTowardsGoal return
    std::pair<Point,Point> lineSegment;

    if(def_area.contains(bp, -ROBOT_RADIUS) || !def_area.contains(bp, ROBOT_RADIUS*2))
        retrieving_ball = false;

    // If there is a robot with the ball facing our goal, we move to get in it's trajectory.
    if(ballBot && !ballBot->isGoalie() && botOnBallIsAimedAtOurGoal(ballBot, lineSegment))
    {
//        std::cout << "Ball Bot" << std::endl;
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(1.5);
        setMovementTargets(blockPoint, angleToBall, false, false);
        GenericMovementBehavior::perform(robot);
    }

    // If the ball is moving towards goal, we move to get into the line of trajectory.
    else if(isBallMovingTowardsGoal(lineSegment) && !retrieving_ball)
    {
//        std::cout << "Ball Moving Towards Goal" << std::endl;
        Point blockPoint = Measurements::lineSegmentPoint(robot->getPosition(), lineSegment.first, lineSegment.second);
        setVelocityMultiplier(1.5);
        setMovementTargets(blockPoint, angleToBall, false, false);
        GenericMovementBehavior::perform(robot);
    }

    // If the ball is stopped just outside our defence area, we dribble it inside
    // the defence area if safe
    else if ((shouldRetrieveBall() || retrieving_ball) && !clearing_ball)
    {
//        std::cout << "Retrieving" << std::endl;
        retrieving_ball = true;
        dribble_skill->perform(robot);
    }

    // Kick the ball away if it is inside the defense area
    else if(shouldClearBall())
    {
//        std::cout << "Clearing" << std::endl;

        // Find reachable teammates
        std::vector<Robot*> candidates;

        std::vector<Robot*> obstacles;
        for(Robot* r : gameModel->getOppTeam().getRobots()) obstacles.push_back(r);
        for(Robot* r : gameModel->getMyTeam().getRobots()) obstacles.push_back(r);


        for(Robot* tmate: gameModel->getMyTeam().getRobots())
        {
            if (tmate->getID() != robot->getID()
            && tmate->getPosition().x > -1500)   // Ignore teammates too close to the goal
            {
                auto remove_it = std::remove_if(obstacles.begin(), obstacles.end(), [&](Robot* r)
                {
                    return r->isOnMyTeam() && (r->getID() == tmate->getID() || r->getID() == robot->getID());
                });

                obstacles.erase(remove_it, obstacles.end());

                Robot* obst = Measurements::robotInPath(obstacles, bp, tmate->getPosition(), ROBOT_RADIUS + BALL_RADIUS + 20);

                obstacles.push_back(tmate);
                obstacles.push_back(robot);

                if (obst == nullptr)
                    candidates.push_back(tmate);
            }
        }

        // Find farthest robot from among the reachable teammates
        Robot* farthest = nullptr;
        if(!candidates.empty())
        {
            farthest = candidates.front();
            for(Robot* r: candidates)
            {

                if(r->getPosition().x > farthest->getPosition().x)
                    farthest = r;
            }
        }

        if(farthest != nullptr)
            kickPoint = farthest->getPosition();
        else
            kickPoint = Point(3000, 0);

//        GuiInterface::getGuiInterface()->drawLine(bp, kickPoint);

        clearing_ball = kick_skill->perform(robot);
    }

    // If the the ball is in play and not outside the field, opponents can try to score.
    // So, move to block the shortest path from the ball to the goal
    else if (isBallReachable())
    {
//        std::cout << "Stray Ball" << std::endl;

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
//        std::cout << "Ball Out of Bounds" << std::endl;
        setVelocityMultiplier(1);
        setMovementTargets(idlePoint, Measurements::angleBetween(robot, Point(0,0)));
        GenericMovementBehavior::perform(robot);
    }
}

bool Goalie::isBallMovingTowardsGoal(std::pair<Point,Point>& lineSegOut)
{
    // Filter out balls not moving towards goal
    Point goal = gameModel->getMyGoal();
    std::cout << "goal point:" << std::endl;
    Point bVel = gameModel->getBallVelocity();
    if(bVel.x > -10)
        return false;

    // Calculate y position at goal point
    Point ballPos = gameModel->getBallPoint();
    float y = (bVel.y / bVel.x) * (goal.x - ballPos.x) + ballPos.y;

    // Set the output to a pair of Points representing the line of the ball's trajectory.
    lineSegOut = {ballPos, Point(goal.x+ROBOT_RADIUS,y)};

    // Is the Y position within the goalie box?
    return (y > -GOAL_WIDTH/2) && (y < GOAL_WIDTH/2);
}

bool Goalie::botOnBallIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut)
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

bool Goalie::isBallReachable()
{
    return abs(Measurements::angleBetween(idlePoint, gameModel->getBallPoint()) <= M_PI_2);
}

bool Goalie::shouldClearBall()
{
    // We only clear the ball when it is inside the defence area
    // and moving slow enough
    return def_area.contains(gameModel->getBallPoint()) && gameModel->getBallSpeed() <= 100;
}

bool Goalie::shouldRetrieveBall()
{
    // We bring the ball into the defense area if it is
    // - closer than 2 robot radius outside the defense area
    Point bp = gameModel->getBallPoint();
    bool b0 = def_area.contains(bp, ROBOT_RADIUS*2);

    // - farther than ball diameter inside the defense area
    bool b1 = !def_area.contains(bp, -ROBOT_RADIUS*0.5);

    // - moving slow enough
    bool b2 = (gameModel->getBallSpeed() <= 100);

    // - closer to one of our robots than to an opponent robot
    Robot* nearest_opp = Comparisons::distance(bp).minOppTeam();
    Robot* nearest_teammate = Comparisons::distance(bp).minMyTeam();

    bool b3;
    if(nearest_opp == nullptr
    || Measurements::distance(nearest_opp, bp)
     - Measurements::distance(nearest_teammate, bp) > ROBOT_RADIUS)
        b3 = true;
    else
        b3 = false;

    return b0 && b1 && b2 && b3;
}

