#include "attackmain.h"

AttackMain::AttackMain()
{
    // Calculate the probility of scoring from each point
    // in the probability field based on fixed factors
    Point opp_goal = gameModel->getOppGoal();
    float w_dist = 2.0, w_ang = 1.0;    // Relative weights
    float dist = 0.0, angle = 0.0;
    float temp_p = 0.0;

    DefenceArea def_area(!OUR_TEAM);

    for (int x = 0; x <= PF_LENGTH; ++x)
    {
        for (int y = 0; y <= PF_WIDTH; ++y)
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
                n.base_val += temp_p;

                // probability = 1 - (1/(85*pi/180)) * angle
                temp_p = 1.0 - 0.674 * angle;
                temp_p *= w_ang;
                n.base_val += temp_p;
            }

            // Remove Points from defence area
            if(def_area.contains(n.point))
                n.base_val = 0;

            // Normalize probabilities
            n.base_val /= (w_dist + w_ang);
        }
    }

    kick_skill = new Skill::KickToPointOmni(&kick_point,-1,-1,true);
}


/* Angle tolerances for kicking in degrees (then converted to radians).
 * Passing is lower because it needs to be more precise */
#if SIMULATED
 #define SCORE_ANGLE_TOLERANCE  (30*M_PI/180)
 #define PASS_ANGLE_TOLERANCE   (15*M_PI/180)
#else
 #define SCORE_ANGLE_TOLERANCE  (7*M_PI/180)
 #define PASS_ANGLE_TOLERANCE   (7*M_PI/180)
#endif

void AttackMain::perform(Robot * robot)
{
    // Check if we can score
    std::pair<bool, Point> result = calcBestGoalPoint(robot);

    if(result.first)    // If we can score from where the ball is, kick
    {
        kick_point = result.second;
        kick_skill->perform(robot);
    }
    else    // If we cannot score, move
    {
        // Update dynamic probabilities
        calcActualProb();

        // Find max probability node in opponent side of field
        ProbNode& max_node = prob_field[PF_LENGTH/2][0];

        for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
        {
            for(int y = 0; y < PF_WIDTH; ++y)
            {
                ProbNode& curr = prob_field[x][y];

                if(max_node.base_val+max_node.actual_val <= curr.base_val+curr.actual_val)
                   max_node = curr;

                //if(curr.base_val+curr.actual_val >= 0.4)
                 //   GuiInterface::getGuiInterface()->drawPoint(curr.point);
            }
        }

        // Move Towards Max Node
        setMovementTargets(max_node.point, 0, true, false);
        GenericMovementBehavior::perform(robot);
    }
}


void AttackMain::calcActualProb()
{
    // Clear previous calculations
    for(int x = 0; x < PF_LENGTH; ++x)
        for(int y = 0; y < PF_WIDTH; ++y)
            prob_field[x][y].actual_val = 0;

    // Top end of goal post
    float A1 = gameModel->getOppGoal().x;
    float B1 = gameModel->getOppGoal().y + GOAL_WIDTH/2;

    // Bottom end of goal post
    float A2 = gameModel->getOppGoal().x;
    float B2 = gameModel->getOppGoal().y - GOAL_WIDTH/2;

    float R = ROBOT_RADIUS;

    for(Robot* r: gameModel->getOppTeam())
    {
        float C = r->getPosition().x;
        float D = r->getPosition().y;

        // Gradient of line starting from top end of goal post and tangent to robot (from MATLAB)
        float m1 = -(R*sqrt(pow(A1-C,2) + pow(B1-D,2) - R*R) + (D-B1)*(A1-C))/(pow(A1-C,2) - R*R);

        // Gradient of line starting from bottom end of goal post and tangent to robot (from MATLAB)
        float m2 = (R*sqrt(pow(A2-C,2) + pow(B2-D,2) - R*R) + (B2-D)*(A2-C))/(pow(A2-C,2) - R*R);

        // Set probabilities
        for(int x = std::max(0,PF_LENGTH/2 + (int)(C-1000)/PND); x <= std::min(PF_LENGTH,PF_LENGTH/2 + (int)(C+3*R)/PND); ++x)
        {
            for(int y = std::max(0,PF_WIDTH/2 + (int)(D-1000)/PND); y <= std::min(PF_WIDTH,PF_WIDTH/2 + (int)(D+1000)/PND); ++y)
            {
                ProbNode& n = prob_field[x][y];

                // Reduced probability of scoring in the shadow
                // of a robot considering the goal post as light source
                float extension = 100;   // Distance beyond actual shadow that we consider blocked
                if(n.point.y <= m1*(n.point.x - A1) + B1 + extension
                && n.point.y >= m2*(n.point.x - A2) + B2 - extension
                && n.point.x <= C - R)
                   n.actual_val = -10.0;

                // Reduced probability of scoring close to an opponent
                if(Measurements::distance(r, n.point) < R * 2)
                    n.actual_val = -10.0;
            }
        }
    }
}


std::pair<bool, Point> AttackMain::calcBestGoalPoint(Robot* r)
{
    // Populate a vector with robot positions
    std::vector<Point> obstacles;
    auto myTeam = gameModel->getMyTeam();
    auto oppTeam = gameModel->getOppTeam();

    obstacles.reserve(myTeam.size() + oppTeam.size());

    for(Robot* rob : myTeam)
        if(rob->getID() != r->getID())
            obstacles.push_back(rob->getPosition());
    for(Robot* rob : oppTeam)
        obstacles.push_back(rob->getPosition());

    // For a number of sample points along the opponent goal,
    // check if an obstacle-free path is found for the ball

    // If the robot is in positive y, we start our checks from the
    // positive end of the goal post
    if(r->getPosition().y > 0)
    {
        for(int goal_y = GOAL_WIDTH/2-20; goal_y >= -GOAL_WIDTH/2; goal_y -= 10)
        {
            Point target = gameModel->getOppGoal() + Point(0, goal_y);
            bool clear_shot = true;
            for(const Point& obstacle : obstacles)
            {
                if(Measurements::lineDistance(obstacle, gameModel->getBallPoint(), target) < BALL_RADIUS+ROBOT_RADIUS+20)
                {
                    clear_shot = false;
                    break;
                }
            }

            if(clear_shot)
                return std::pair<bool, Point>(true, target);
        }
    }
    // Otherwise we start checking from the negative end of the goal post
    else
    {
        for(int goal_y = -GOAL_WIDTH/2+20; goal_y <= GOAL_WIDTH/2; goal_y += 10)
        {
            Point target = gameModel->getOppGoal() + Point(0, goal_y);
            bool clear_shot = true;
            for(const Point& obstacle : obstacles)
            {
                if(Measurements::lineDistance(obstacle, gameModel->getBallPoint(), target) < BALL_RADIUS+ROBOT_RADIUS+20)
                {
                    clear_shot = false;
                    break;
                }
            }

            if(clear_shot)
                return std::pair<bool, Point>(true, target);
        }
    }

    return std::pair<bool, Point>(false, Point(0,0));
}

bool AttackMain::isFinished()
{
    return done;
}

AttackMain::~AttackMain()
{
    delete kick_skill;
}
