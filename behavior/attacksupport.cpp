#include "attacksupport.h"

AttackSupport::AttackSupport()
{
    calcStaticProb();
    state = position;
}

void AttackSupport::perform(Robot * robot)
{
    Point rp = robot->getPosition();
    Point bp = gameModel->getBallPoint();

    // We signal that we are done supporting if:
    // - We are closest member on our team to the ball
    // - The ball is close to us
    finished = Comparisons::distance(bp).minMyTeam()->getID() == robot->getID()
            && Measurements::isClose(rp, bp, ROBOT_RADIUS+BALL_RADIUS+200);

    switch(state)
    {
    case intercept:
    {
        robot->setDribble(true);

//        std::cout << "Intercepting" << std::endl;

        // Evaluate transition to positioning
        Point b_vel = gameModel->getBallVelocity();
        Robot* ball_bot = gameModel->getHasBall();

        bool ball_bot_not_facing_us =
                ball_bot != nullptr && ball_bot->isOnMyTeam()
                && Comparisons::isNotFacingPoint(ball_bot, rp, 15*M_PI/180);

        if(ball_bot_not_facing_us)
            switch2position_count++;
        else
            switch2position_count = 0;

        if(switch2position_count >= 30)
        {
            switch2position_count = 0;
            state = position;
            break;
        }

        // Move into the line that the passing robot
        // is facing if ball velocity is low
        Point intercept_pt;

        if(Measurements::mag(b_vel) < 200)
        {
            if(ball_bot != nullptr && ball_bot->isOnMyTeam())
            {
                Point ball_bot_pos = ball_bot->getPosition();
                float ball_bot_ori = ball_bot->getOrientation();

                Point facing_vector(cos(ball_bot_ori), sin(ball_bot_ori));
                intercept_pt = Measurements::lineSegmentPoint(rp, ball_bot_pos, ball_bot_pos+(facing_vector*10000));
            }
        }
        else
            intercept_pt = Measurements::lineSegmentPoint(rp, bp, bp+(b_vel*10000));

        // Move to intercept only if the intercept point is near
        if(Measurements::isClose(robot, intercept_pt, 200))
        {
            float ang2ball = Measurements::angleBetween(rp, bp);
            setMovementTolerances(1, 0.001);
            setVelocityMultiplier(1.5);
            setMovementTargets(intercept_pt, ang2ball, true, false);
            GenericMovementBehavior::perform(robot);
        }
        else
            state = position;

        break;
    }
    case position:
    {
        robot->setDribble(false);
//        std::cout << "Positioning" << std::endl;

        // Evaluate transition to intercepting by
        // Checking if a teammate with the ball is facing this robot
        Robot* ball_bot = gameModel->getHasBall();

        if(ball_bot != nullptr && ball_bot->isOnMyTeam()
        && Comparisons::isFacingPoint(ball_bot, rp, 10*M_PI/180)
        && !Measurements::isClose(rp, ball_bot, 1000))
            switch2intercept_count = std::min(30, switch2intercept_count+1);
        else
            switch2intercept_count = 0;

        if(switch2intercept_count >= 30)
        {
            switch2intercept_count = 0;
            state = intercept;
            break;
        }

        // Move towards node with highest prob of scoring while facing ball
        calcDynamicProb(robot);

        ProbNode max_node = findMaxNode();
        float ang2ball = Measurements::angleBetween(rp, bp);

        setMovementTolerances(DIST_TOLERANCE, ROT_TOLERANCE);
        setVelocityMultiplier(1.0);
        setMovementTargets(max_node.point, ang2ball, true, false);
        GenericMovementBehavior::perform(robot);
    }
    }


    for(int x = PF_LENGTH_SUPP/2; x < PF_LENGTH_SUPP; ++x)
    {
        for(int y = 0; y < PF_WIDTH_SUPP; ++y)
        {
            ProbNode& curr = prob_field[x][y];
            if(curr.static_val+curr.dynamic_val >= 0.4)
                GuiInterface::getGuiInterface()->drawPoint(curr.point);
        }
    }
}

AttackSupport::ProbNode AttackSupport::findMaxNode()
{
    // Find max probability node in opponent side of field
    ProbNode max_node = prob_field[PF_LENGTH_SUPP/2][0];

    for(int x = PF_LENGTH_SUPP/2; x < PF_LENGTH_SUPP; ++x)
    {
        for(int y = 0; y < PF_WIDTH_SUPP; ++y)
        {
            ProbNode& curr = prob_field[x][y];

            if(max_node.static_val+max_node.dynamic_val <= curr.static_val+curr.dynamic_val)
               max_node = curr;
        }
    }

    return max_node;
}

void AttackSupport::calcStaticProb()
{
    // Calculate the static probility of scoring from each point
    // in the probability field based on fixed factors
    Point opp_goal = gameModel->getOppGoal();
    float w_dist = 2.0, w_ang = 1.0;    // Relative weights
    float dist = 0.0, angle = 0.0;
    float temp_p = 0.0;

    DefenceArea def_area(!OUR_TEAM);

    for (int x = 0; x < PF_LENGTH_SUPP; ++x)
    {
        for (int y = 0; y < PF_WIDTH_SUPP; ++y)
        {
            ProbNode& n = prob_field[x][y];

            n.point = Point(x*PND_SUPP - HALF_FIELD_LENGTH, y*PND_SUPP - HALF_FIELD_WIDTH);

            // Probability of scoring is a decreasing function of distance from
            // the goal. Anything beyond 3000 points gets a probability of zero.
            // It is also a decreasing function of the angle to the goal.
            dist = Measurements::distance(n.point, opp_goal);
            angle = fabs(Measurements::angleBetween(n.point, opp_goal));

            float max_ang_rad = 70*M_PI/180;

            if(dist < 3000 && angle < max_ang_rad) // 80 degrees
            {
                // probability = 1 - (1/3000) * distance
                temp_p = 1.0 - 0.000333 * dist;
                temp_p *= w_dist;
                n.static_val += temp_p;

                // probability = 1 - (1/(85*pi/180)) * angle
                temp_p = 1.0 - angle/max_ang_rad;
                temp_p *= w_ang;
                n.static_val += temp_p;
            }

            // Remove Points from defence area
            if(def_area.contains(n.point))
                n.static_val = 0;

            // Normalize probabilities
            n.static_val /= (w_dist + w_ang);
        }
    }
}

void AttackSupport::calcDynamicProb(Robot * robot)
{
    // Clear previous calculations
    for(int x = 0; x < PF_LENGTH_SUPP; ++x)
        for(int y = 0; y < PF_WIDTH_SUPP; ++y)
            prob_field[x][y].dynamic_val = 0;

    genGoalShadows();
    genDistanceFromTeammates(robot);
    genBallShadows();
    genGoalShotAvoidance();
}


void AttackSupport::genGoalShadows()
{
    // Top end of goal post
    float g1x = gameModel->getOppGoal().x;
    float g1y = gameModel->getOppGoal().y + GOAL_WIDTH/2;

    // Bottom end of goal post
    float g2x = gameModel->getOppGoal().x;
    float g2y = gameModel->getOppGoal().y - GOAL_WIDTH/2;

    float R = ROBOT_RADIUS;

    // Generate clusters of robots
    std::vector<std::vector<Point>> clusters = genClusters();

    // Calculate shadow geometry for each cluster of robots
    for(std::vector<Point> cluster : clusters)
    {
        float top_x = cluster.front().x;
        float top_y = cluster.front().y;

        float bot_x = cluster.back().x;
        float bot_y = cluster.back().y;

        // Gradient of line starting from top end of goal post and tangent to top robot (from MATLAB)
        float m1 = -(R*sqrt(pow(g1x-top_x,2) + pow(g1y-top_y,2) - R*R) + (top_y-g1y)*(g1x-top_x))/(pow(g1x-top_x,2) - R*R);
//        float x1 = (2*top_x + 2*m1*top_y + 2*m1*m1*g1x - 2*m1*g1y)/(2*m1*m1 + 2);
//        float y1 = m1*x1 - m1*g1x + g1y;

        // Gradient of line starting from bottom end of goal post and tangent to bottom robot (from MATLAB)
        float m2 = (R*sqrt(pow(g2x-bot_x,2) + pow(g2y-bot_y,2) - R*R) + (g2y-bot_y)*(g2x-bot_x))/(pow(g2x-bot_x,2) - R*R);
//        float x2 = (2*bot_x + 2*m2*bot_y + 2*m2*m2*g2x - 2*m2*g2y)/(2*m2*m2 + 2);
//        float y2 = m2*x2 - m2*g2x + g2y;

        // Set probabilities
        for(int x = PF_LENGTH_SUPP/2; x < PF_LENGTH_SUPP; ++x)
        {
            for(int y = 0; y < PF_WIDTH_SUPP; ++y)
            {
                ProbNode& n = prob_field[x][y];

                // Reduced probability of scoring in the shadow
                // of a robot considering the goal post as light source
                float extension = 100;   // Distance beyond actual shadow that we consider blocked
                if(n.point.y <= m1*(n.point.x - g1x) + g1y + extension
                && n.point.y >= m2*(n.point.x - g2x) + g2y - extension)
                   n.dynamic_val = -100.0;
            }
        }
    }
}


void AttackSupport::genDistanceFromTeammates(Robot* robot)
{
    // Set probabilities
    for(int x = PF_LENGTH_SUPP/2; x < PF_LENGTH_SUPP; ++x)
    {
        for(int y = 0; y < PF_WIDTH_SUPP; ++y)
        {
            ProbNode& n = prob_field[x][y];

            for(Robot* tmate: gameModel->getMyTeam())
            {
                if(tmate->getID() != robot->getID()
                && (Measurements::distance(tmate->getPosition(), n.point) < 2000))
                        n.dynamic_val = -100.0;
            }
        }
    }
}


void AttackSupport::genBallShadows()
{

    Point bp = gameModel->getBallPoint();

    float R = ROBOT_RADIUS + 50;

    for(Robot* opp: gameModel->getOppTeam())
    {
        float rob_x = opp->getPosition().x;
        float rob_y = opp->getPosition().y;

        // Calculations for line 1 starting from ball and tangent to robot
        float m1 = -(R*sqrt(pow(bp.x-rob_x,2) + pow(bp.y-rob_y,2) - R*R) + (rob_y-bp.y)*(bp.x-rob_x))/(pow(bp.x-rob_x,2) - R*R);
        float x1 = (2*rob_x + 2*m1*rob_y + 2*m1*m1*bp.x - 2*m1*bp.y)/(2*m1*m1 + 2);
        float y1 = m1*x1 - m1*bp.x + bp.y;

        float line1_dir = bp.y < y1? 1 : -1;
        float y1_edge = line1_dir * HALF_FIELD_WIDTH;
        float x1_edge = bp.x + (y1_edge - bp.y)/m1;

        if(x1_edge < -HALF_FIELD_LENGTH)
        {
            x1_edge = -HALF_FIELD_LENGTH;
            y1_edge = m1*(x1_edge - bp.x) + bp.y;
        }
        else if(x1_edge > HALF_FIELD_LENGTH)
        {
            x1_edge = HALF_FIELD_LENGTH;
            y1_edge = m1*(x1_edge - bp.x) + bp.y;
        }

        // Calculations for line 2 starting from ball and tangent to other side of robot
        float m2 = (R*sqrt(pow(bp.x-rob_x,2) + pow(bp.y-rob_y,2) - R*R) + (bp.y-rob_y)*(bp.x-rob_x))/(pow(bp.x-rob_x,2) - R*R);
        float x2 = (2*rob_x + 2*m2*rob_y + 2*m2*m2*bp.x - 2*m2*bp.y)/(2*m2*m2 + 2);
        float y2 = m2*x2 - m2*bp.x + bp.y;

        float line2_dir = bp.y < y2? 1 : -1;
        float y2_edge = line2_dir * HALF_FIELD_WIDTH;
        float x2_edge = bp.x + (y2_edge - bp.y)/m2;

        if(x2_edge < -HALF_FIELD_LENGTH)
        {
            x2_edge = -HALF_FIELD_LENGTH;
            y2_edge = m2*(x2_edge - bp.x) + bp.y;
        }
        else if(x2_edge > HALF_FIELD_LENGTH)
        {
            x2_edge = HALF_FIELD_LENGTH;
            y2_edge = m2*(x2_edge - bp.x) + bp.y;
        }

        // Determine minimum and maximum x-values between both tangent-robot interceptions
        float min_x = 0, max_x = HALF_FIELD_LENGTH;
        max_x = fmin(max_x, fmax(bp.x, fmax(x1_edge, x2_edge)));
        min_x = fmax(min_x, fmin(bp.x, fmin(x1_edge, x2_edge)));

//        GuiInterface::getGuiInterface()->drawLine(Point(bp.x, bp.y), Point(x1_edge, y1_edge));
//        GuiInterface::getGuiInterface()->drawLine(Point(bp.x, bp.y), Point(x2_edge, y2_edge));

        // Shade in the shadows with vertical strokes from from left to right
        for(int x = min_x; x < max_x; x+=PND_SUPP)
        {
            // Determine min and max y-values on tangent lines for current the x-value
            float min_y = m1*x - m1*bp.x + bp.y;
            float min_y_dir = bp.y < min_y? 1 : -1;

            if(min_y_dir == line1_dir)
            {
                if(min_y < -HALF_FIELD_WIDTH) min_y = -HALF_FIELD_WIDTH;
                else if(min_y > HALF_FIELD_WIDTH) min_y = HALF_FIELD_LENGTH;
            }
            else
                min_y = line1_dir * HALF_FIELD_WIDTH;

            float max_y = m2*x - m2*bp.x + bp.y;
            float max_y_dir = bp.y < max_y? 1 : -1;

            if(max_y_dir == line2_dir)
            {
                if(max_y < -HALF_FIELD_WIDTH) max_y = -HALF_FIELD_WIDTH;
                else if(max_y > HALF_FIELD_WIDTH) max_y = HALF_FIELD_LENGTH;
            }
            else
                max_y = line2_dir * HALF_FIELD_WIDTH;

            if(min_y > max_y) std::swap(min_y, max_y);

            // Vertical strokes from min_y to max_y
            for(int y = min_y; y < max_y; y+=PND_SUPP)
            {
                if(abs(x) < HALF_FIELD_LENGTH && abs(y) < HALF_FIELD_WIDTH)
                {
                    ProbNode& node = prob_field[PF_LENGTH_SUPP/2 + x/PND_SUPP][PF_WIDTH_SUPP/2 + y/PND_SUPP];

                    float dist = Measurements::distance(bp, opp->getPosition());

                    // Only cast shadows behind the opponents
                    if(Measurements::distance(bp, node.point) > dist)
                        node.dynamic_val = -100.0;
                }
            }
        }
    }
}


std::vector<std::vector<Point>> AttackSupport::genClusters()
{
    int cluster_tol = 2*ROBOT_RADIUS + 2*BALL_RADIUS + 10;

    std::vector<std::vector<Point>> clusters;

    for(Robot* opp: gameModel->getOppTeam())
    {
        // Check if each opponent belongs to an existing cluster
        bool assigned = false;

        for(std::vector<Point>& cluster: clusters)
        {
            for(Point& p : cluster)
            {
                if(Measurements::distance(opp->getPosition(), p) < cluster_tol)
                {
                    cluster.push_back(opp->getPosition());
                    assigned = true;
                    break;
                }

                if(assigned) break;
            }
        }

        // If the robot does not fit in an existing cluster
        if(!assigned)
        {
            // Add the robot position to a new cluster
            std::vector<Point> cluster;
            cluster.push_back(opp->getPosition());
            clusters.push_back(cluster);
        }
    }

    for(std::vector<Point>& cluster : clusters)
        std::sort(cluster.begin(),cluster.end(), [](const Point& A, const Point& B){return A.y > B.y;});

    return clusters;
}


void AttackSupport::genGoalShotAvoidance()
{
    // Top end of goal post
    float g1x = gameModel->getOppGoal().x;
    float g1y = gameModel->getOppGoal().y + GOAL_WIDTH/2 + ROBOT_RADIUS + 500;

    // Bottom end of goal post
    float g2x = gameModel->getOppGoal().x;
    float g2y = gameModel->getOppGoal().y - GOAL_WIDTH/2 - ROBOT_RADIUS - 500;

    Point bp = gameModel->getBallPoint();

    float min_x = bp.x;
    float max_x = g1x;

    for(int x = min_x; x < max_x; x+=PND_SUPP)
    {
        float min_y = (x - bp.x)*(g2y - bp.y)/(g2x-bp.x) + bp.y;
        float max_y = (x - bp.x)*(g1y - bp.y)/(g1x-bp.x) + bp.y;

        for(int y = min_y; y < max_y; y+=PND_SUPP)
        {
            if(abs(x) < HALF_FIELD_LENGTH && abs(y) < HALF_FIELD_WIDTH)
                prob_field[PF_LENGTH_SUPP/2 + x/PND_SUPP]
                          [PF_WIDTH_SUPP/2  + y/PND_SUPP].dynamic_val = -100.0;
        }
    }
}


bool AttackSupport::isFinished()
{
    return finished;
}

AttackSupport::~AttackSupport()
{
}

float AttackSupport::getScoreProb(const Point& p)
{
    if(Comparisons::isPointInsideField(p))
    {
        ProbNode pn = prob_field[PF_LENGTH_SUPP/2+(int)p.x/PND_SUPP][PF_WIDTH_SUPP/2+(int)p.y/PND_SUPP];
        return fmax(0, pn.dynamic_val + pn.static_val);
    }
    return 0;
}

