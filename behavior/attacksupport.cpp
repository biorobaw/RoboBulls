#include "attacksupport.h"

AttackSupport::AttackSupport()
{
    calcStaticProb();
}

void AttackSupport::perform(Robot * robot)
{
    calcDynamicProb(robot);

//    for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
//    {
//        for(int y = 0; y < PF_WIDTH; ++y)
//        {
//            ProbNode& curr = prob_field[x][y];
//            if(curr.static_val+curr.dynamic_val >= 0.4)
//                GuiInterface::getGuiInterface()->drawPoint(curr.point);
//        }
//    }

    // Find max probability node in opponent side of field
    ProbNode max_node = prob_field[PF_LENGTH/2][0];

    for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
    {
        for(int y = 0; y < PF_WIDTH; ++y)
        {
            ProbNode& curr = prob_field[x][y];

            if(max_node.static_val+max_node.dynamic_val <= curr.static_val+curr.dynamic_val)
               max_node = curr;
        }
    }

    // Dribble Towards max node while facing ball
    float ang2ball = Measurements::angleBetween(robot->getPosition(), gameModel->getBallPoint());
    setMovementTargets(max_node.point, ang2ball,true,false);
    GenericMovementBehavior::perform(robot);
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

    for (int x = 0; x < PF_LENGTH; ++x)
    {
        for (int y = 0; y < PF_WIDTH; ++y)
        {
            ProbNode& n = prob_field[x][y];

            n.point = Point(x*PND - HALF_FIELD_LENGTH, y*PND - HALF_FIELD_WIDTH);

            // Probability of scoring is a decreasing function of distance from
            // the goal. Anything beyond 3000 points gets a probability of zero.
            // It is also a decreasing function of the angle to the goal.
            dist = Measurements::distance(n.point, opp_goal);
            angle = fabs(Measurements::angleBetween(n.point, opp_goal));

            if(dist < 3000 && angle < 1.484) // 85 degrees
            {
                // probability = 1 - (1/3000) * distance
                temp_p = 1.0 - 0.000333 * dist;
                temp_p *= w_dist;
                n.static_val += temp_p;

                // probability = 1 - (1/(85*pi/180)) * angle
                temp_p = 1.0 - 0.674 * angle;
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
    for(int x = 0; x < PF_LENGTH; ++x)
        for(int y = 0; y < PF_WIDTH; ++y)
            prob_field[x][y].dynamic_val = 0;

    genGoalShadows();
    genDistanceFromTeammates(robot);
    //genBallShadows();
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

    // Cast shadows to clusters of robots from goal post to determine
    // unlikely scoring positions
    for(std::vector<Point> cluster : clusters)
    {
        float top_x = cluster.front().x;
        float top_y = cluster.front().y;

        float bot_x = cluster.back().x;
        float bot_y = cluster.back().y;

        // Gradient of line starting from top end of goal post and tangent to top robot (from MATLAB)
        float m1 = -(R*sqrt(pow(g1x-top_x,2) + pow(g1y-top_y,2) - R*R) + (top_y-g1y)*(g1x-top_x))/(pow(g1x-top_x,2) - R*R);

        // Gradient of line starting from bottom end of goal post and tangent to bottom robot (from MATLAB)
        float m2 = (R*sqrt(pow(g2x-bot_x,2) + pow(g2y-bot_y,2) - R*R) + (g2y-bot_y)*(g2x-bot_x))/(pow(g2x-bot_x,2) - R*R);

//        GuiInterface::getGuiInterface()->drawLine(Point(A1,B1), Point(top_x, top_y), 0.01);
//        GuiInterface::getGuiInterface()->drawLine(Point(A2,B2), Point(bot_x, bot_y), 0.01);

        // Set probabilities
        for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
        {
            for(int y = 0; y < PF_WIDTH; ++y)
            {
                ProbNode& n = prob_field[x][y];

                // Reduced probability of scoring in the shadow
                // of a robot considering the goal post as light source
                float extension = 100;   // Distance beyond actual shadow that we consider blocked
                if(n.point.y <= m1*(n.point.x - g1x) + g1y + extension
                && n.point.y >= m2*(n.point.x - g2x) + g2y - extension)
                   n.dynamic_val = -10.0;
            }
        }
    }
}
void AttackSupport::genDistanceFromTeammates(Robot* robot)
{
    // Set probabilities
    for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
    {
        for(int y = 0; y < PF_WIDTH; ++y)
        {
            ProbNode& n = prob_field[x][y];

            for(Robot* tmate: gameModel->getMyTeam())
            {
                if(tmate->getID() != robot->getID()
                && (Measurements::distance(tmate->getPosition(), n.point) < 1000))
                        n.dynamic_val = -10;
            }
        }
    }
}



void AttackSupport::genBallShadows()
{
    // Cast shadows from ball to robots
    Point bp = gameModel->getBallPoint();

    float R = ROBOT_RADIUS+25;

    for(Robot* opp: gameModel->getOppTeam())
    {
        float rob_x = opp->getPosition().x;
        float rob_y = opp->getPosition().y;

        // Gradient of line 1 starting from ball and tangent to robot
        float m1 = -(R*sqrt(pow(bp.x-rob_x,2) + pow(bp.y-rob_y,2) - R*R) + (rob_y-bp.y)*(bp.x-rob_x))/(pow(bp.x-rob_x,2) - R*R);
        float x1 = (2*rob_x + 2*m1*rob_y + 2*m1*m1*bp.x - 2*m1*bp.y)/(2*m1*m1 + 2);
//        float y1 = m1*x1 - m1*bp.x + bp.y;
//        float ang1 = Measurements::angleBetween(bp, Point(x1, y1));

        // Gradient of line 2 starting from ball and tangent to robot
        float m2 = (R*sqrt(pow(bp.x-rob_x,2) + pow(bp.y-rob_y,2) - R*R) + (bp.y-rob_y)*(bp.x-rob_x))/(pow(bp.x-rob_x,2) - R*R);
        float x2 = (2*rob_x + 2*m2*rob_y + 2*m2*m2*bp.x - 2*m2*bp.y)/(2*m2*m2 + 2);
//        float y2 = m2*x2 - m2*bp.x + bp.y;
//        float ang2 = Measurements::angleBetween(bp, Point(x2, y2));

        // Extending lines from horizontal edges
        float y_incpt1 = bp.y - y1 < 0? HALF_FIELD_WIDTH:-HALF_FIELD_WIDTH;
        float x_incpt1 = (y_incpt1 + m1*bp.x - bp.y)/m1;
        float y_incpt2 = bp.y - y1 < 0? HALF_FIELD_WIDTH:-HALF_FIELD_WIDTH;
        float x_incpt2 = (y_incpt2 + m1*bp.x - bp.y)/m1;

        float x_min = fmin(-HALF_FIELD_LENGTH, fmin(x_incpt1, x_incpt2));
        float x_max = fmax( HALF_FIELD_LENGTH, fmax(x_incpt1, x_incpt2));

        for(int x = x_min; x < x_max; x += PND)
        {
            double t_min = fabs(x1 - bp.x)/fabs(x-bp.x);
            double t_delta = 0;
            for(double t = t_min; t < 1.0; t += t_delta)
            {

            }
        }

        // Extending lines from vertical edges
        x_incpt1 = bp.x - x1 < 0? FIELD_LENGTH:-FIELD_LENGTH;
        y_incpt1 = m1*x_incpt1 - m1*bp.x + bp.y;
        x_incpt2 = bp.x - x1 < 0? FIELD_LENGTH:-FIELD_LENGTH;
        y_incpt2 = m1*x_incpt2 - m1*bp.x + bp.y;

        for(int y = x_min; x < m_max; ++x)
        {

        }

//        GuiInterface::getGuiInterface()->drawLine(Point(bpx, bpy), Point(x1, y1), 0.001);
//        GuiInterface::getGuiInterface()->drawLine(Point(bpx, bpy), Point(x2, y2), 0.001);

//        for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
//        {
//            for(int y = 0; y < PF_WIDTH; ++y)
//            {
//                ProbNode& node = prob_field[x][y];
//                float angle = Measurements::angleBetween(bp, node.point);
//                float dist = Measurements::distance(bp, opp->getPosition());

//                if(Measurements::angleInRange(angle, ang1, ang2)
//                && Measurements::distance(bp, node.point) > dist)
//                    node.dynamic_val -= 10.0;
//            }
//        }
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


bool AttackSupport::isFinished()
{
    return false;
}

AttackSupport::~AttackSupport()
{
}

float AttackSupport::getScoreProb(const Point& p)
{
    if(Comparisons::isPointInsideField(p))
    {
        ProbNode pn = prob_field[PF_LENGTH/2+(int)p.x/PND][PF_WIDTH/2+(int)p.y/PND];
        return fmax(0, pn.dynamic_val + pn.static_val);
    }
    return 0;
}

