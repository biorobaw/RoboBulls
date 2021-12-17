#include "attackmainNN.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

#include <QDebug>


float AttackMainNN::SCORE_ANGLE_TOLERANCE = ROT_TOLERANCE; //7*M_PI/180;
float AttackMainNN::PASS_ANGLE_TOLERANCE  = ROT_TOLERANCE; //7*M_PI/180;

AttackMainNN::AttackMainNN(Robot* robot) : Behavior(robot)
{   if(robot->getId() == 2 )
     qInfo() << "RObot 2 now attacl";

    prob_field_rows = (Field::FIELD_LENGTH+1)/PND_MAIN;
    prob_field_cols = (Field::FIELD_WIDTH+1)/PND_MAIN;
    prob_field = new ProbNode*[prob_field_rows];
    for(int i=0; i<prob_field_rows; i++) prob_field[i] = new ProbNode[prob_field_cols];
    calcStaticProb();
    dribble_skill = new DribbleToPoint (robot, &kick_point);
    score_skill   = new KickToPointOmni(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
    pass_skill    = new KickToPointOmni(robot, &kick_point,PASS_ANGLE_TOLERANCE, -1,true);
    state = scoring;

    /*prob_field_rows = (Field::FIELD_LENGTH+1)/PND_MAIN;
    prob_field_cols = (Field::FIELD_WIDTH+1)/PND_MAIN;
    prob_field = new ProbNode*[prob_field_rows];
    for(int i=0; i<prob_field_rows; i++) prob_field[i] = new ProbNode[prob_field_cols];*/
    //std::cout << "Attack Main Behavior created"<< std::endl;
}

bool AttackMainNN::perform()
{ //if(team->getID() == 0 ) qInfo() << "Attack Main  Team Blue!!!!";
//    auto clusters = genClusters();
//    for(std::vector<Point> cluster : clusters)
//    {
//       for(Point p: cluster)
//           std::cout << p.x << ", " << p.y << "\t";
//       std::cout << std::endl;
//    }

//    calcActualProb();
//    for(int x = PF_LENGTH/2; x < PF_LENGTH; ++x)
//    {
//        for(int y = 0; y < PF_WIDTH; ++y)
//        {
//            ProbNode& curr = prob_field[x][y];
//            if(curr.base_val+curr.actual_val >= 0.4)
//                GuiInterface::getGuiInterface()->drawPoint(curr.point);
//        }
//    }
    qInfo() <<" NNNNNN";
    switch (state)
    {
    case scoring:
    {   //state = passing;
        //break;
        //std::cout << "AttackMain: Score" << std::endl;
        robot->setDribble(true);

        std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        if(goal_eval.first)
        {
            //qInfo() << "Kick point " << goal_eval.second;
            kick_point = goal_eval.second;
            clear_shot_count = std::min(30, clear_shot_count+1);
        }
        else
            clear_shot_count--;


        if(clear_shot_count < 0 || has_kicked_to_goal)//if kicked to goal
        {
            clear_shot_count = 0;
            state = passing;
        }

        //If kicked previous cycle, will return false.
        has_kicked_to_goal = score_skill->perform();

        if(has_kicked_to_goal)
            qInfo() << "Shoot to score to point " << goal_eval.second;


        has_passed = false;

        break;
    }
    case passing:
    {
        //std::cout << "AttackMain: Pass" << std::endl;
        robot->setDribble(true);

        std::pair<bool, Point> pass_eval = calcBestPassPoint();

        if(pass_eval.first)
        {
            kick_point = pass_eval.second;
            clear_pass_count = std::min(30, clear_pass_count+1);;
        }
        else
            clear_pass_count--;


        if(clear_pass_count < 0 || has_passed)//if we passed
        {
            clear_pass_count = 0;
            state = dribbling;
            //has_passed = false;
            //break;
        }
        //If kicked previous cycle, will return false.
        has_passed = pass_skill->perform();
        GuiInterface::getGuiInterface()->drawLine(*robot, kick_point);
        if(has_passed)
            qInfo() <<"Passing to "<< kick_point;

        //has_passed = false;

        break;
    }
    case dribbling:
    {
        //std::cout << "AttackMain: Dribble" << std::endl;

        // Evaluate state transition to scoring
        std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        std::pair<bool, Point> pass_eval = calcBestPassPoint();

        if(goal_eval.first)
            clear_shot_count++;
        else
            clear_shot_count = std::max(0, clear_shot_count-1);

        if(clear_shot_count > 30)
        {
            clear_shot_count = 0;
            state = scoring;
            break;
        }

        // Evaluate state transition to passing
        if(pass_eval.first)
            clear_pass_count++;
        else
            clear_pass_count = std::max(0, clear_pass_count-1);

        if(clear_pass_count > 30)
        {
            clear_pass_count = 0;
            state = passing;
            break;
        }

        // Update dynamic probabilities
        calcDynamicProb();

        // Find max probability node in opponent side of field
        ProbNode max_node = prob_field[PF_LENGTH_MAIN/2][0];

        //Perhaps we need to add distance to max node as a part of the dynamic
        //probabilities heuristic - Justin

        for(int x = PF_LENGTH_MAIN/2; x < PF_LENGTH_MAIN; ++x)
        {//string test = "";
            for(int y = 0; y < PF_WIDTH_MAIN; ++y)
            {
                ProbNode& curr = prob_field[x][y];
                //test+="N[" +std::to_string(x) +"]["+std::to_string(y)+"]=" +std::to_string(curr.static_val)+"  ";
                //qInfo()<<" N" << curr.static_val;
                if(max_node.static_val+max_node.dynamic_val <= curr.static_val+curr.dynamic_val)
                   max_node = curr;
            }
            //test+="/n";
            //qInfo() << QString(test.c_str());
        }

        // Dribble Towards Max Node
        //qInfo()<< "Max node points: "<<max_node.point;
        kick_point = max_node.point;
        dribble_skill->perform();

        has_kicked_to_goal = false;
        has_passed = false;
    }
    }
    return isFinished();
}


void AttackMainNN::calcStaticProb()
{
    // Calculate the static probility of scoring from each point
    // in the probability field based on fixed factors
    Point opp_goal = Field::getGoalPosition(OPPONENT_SIDE);
    float w_dist = 2.0, w_ang = 1.0;    // Relative weights
    float dist = 0.0, angle = 0.0;
    float temp_p = 0.0;

    DefenceArea def_area(OPPONENT_SIDE);

    for (int x = 0; x < PF_LENGTH_MAIN; ++x)
    {
        for (int y = 0; y < PF_WIDTH_MAIN; ++y)
        {
            ProbNode& n = prob_field[x][y];
            n.static_val =0;
            n.dynamic_val =0;

            n.point = Point(x*PND_MAIN - Field::HALF_FIELD_LENGTH, y*PND_MAIN - Field::HALF_FIELD_WIDTH);
            //qInfo() << n.point;
            // Probability of scoring is a decreasing function of distance from
            // the goal. Anything beyond 3000 points gets a probability of zero.
            // It is also a decreasing function of the angle to the goal.
            dist = Measurements::distance(n.point, opp_goal);
            angle = fabs(Measurements::angleBetween(n.point, opp_goal));
            //qInfo() <<"distL " <<dist <<" angle: " <<angle;
            if(dist < 3000 && angle < 1.484) // 85 degrees
            {
                // probability = 1 - (1/3000) * distance
                temp_p = 1.0 - 0.000333 * dist;
                temp_p *= w_dist;
                n.static_val += temp_p;
                //qInfo() << "Weighted dist: " << temp_p;

                // probability = 1 - (1/(85*pi/180)) * angle
                temp_p = 1.0 - 0.674 * angle;
                temp_p *= w_ang;
                n.static_val += temp_p;
                //qInfo() << "Weighted angle: " << temp_p;

            }

            // Remove Points from defence area
            if(def_area.contains(n.point))
                n.static_val = 0;

            // Normalize probabilities
            n.static_val /= (w_dist + w_ang);
            //qInfo() << x<<"," << y<< " "<<n.static_val;
        }
    }
}

void AttackMainNN::calcDynamicProb()
{
    // Clear previous calculations
    for(int x = 0; x < PF_LENGTH_MAIN; ++x)
        for(int y = 0; y < PF_WIDTH_MAIN; ++y)
            prob_field[x][y].dynamic_val = 0;

    auto gp = Field::getGoalPosition(OPPONENT_SIDE);
    // Top end of goal post
    float A1 = gp.x;
    float B1 = gp.y + Field::GOAL_LENGTH/2;

    // Bottom end of goal post
    float A2 = gp.x;
    float B2 = gp.y - Field::GOAL_LENGTH/2;

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

        // Gradient of line starting from top end of goal post and tangent to top robot (found using MATLAB)
        float m1 = -(R*sqrt(pow(A1-top_x,2) + pow(B1-top_y,2) - R*R) + (top_y-B1)*(A1-top_x))/(pow(A1-top_x,2) - R*R);

        // Gradient of line starting from bottom end of goal post and tangent to bottom robot (found using MATLAB)
        float m2 = (R*sqrt(pow(A2-bot_x,2) + pow(B2-bot_y,2) - R*R) + (B2-bot_y)*(A2-bot_x))/(pow(A2-bot_x,2) - R*R);

//        GuiInterface::getGuiInterface()->drawLine(Point(A1,B1), Point(top_x, top_y));
//        GuiInterface::getGuiInterface()->drawLine(Point(A2,B2), Point(bot_x, bot_y));

        // Set probabilities
        for(int x = PF_LENGTH_MAIN/2; x < PF_LENGTH_MAIN; ++x)
        {
            for(int y = 0; y < PF_WIDTH_MAIN; ++y)
            {
                ProbNode& n = prob_field[x][y];

                // Reduced probability of scoring in the shadow
                // of a robot considering the goal post as light source
                float extension = 100;   // Distance beyond actual shadow that we consider blocked
                if(n.point.y <= m1*(n.point.x - A1) + B1 + extension
                && n.point.y >= m2*(n.point.x - A2) + B2 - extension)
                   n.dynamic_val = -10.0;
            }
        }
    }
}

std::vector<std::vector<Point>> AttackMainNN::genClusters()
{
    int cluster_tol = 2*ROBOT_RADIUS + 2*Field::BALL_RADIUS + 10;

    std::vector<std::vector<Point>> clusters;

    for(Robot* opp: team->getOpponents())
    {
        // Check if each opponent belongs(should go in a) to an existing cluster
        bool assigned = false;

        for(std::vector<Point>& cluster: clusters)
        {
            for(Point& p : cluster)
            {
                if(Measurements::distance(*opp, p) < cluster_tol)
                {
                    cluster.push_back(*opp);
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
            cluster.push_back(*opp);
            clusters.push_back(cluster);
        }
    }

    for(std::vector<Point>& cluster : clusters) //Looks like it is sorting based on vertical location of clusters-Justin
        std::sort(cluster.begin(),cluster.end(), [](const Point& A, const Point& B){return A.y > B.y;});

    return clusters;
}

std::pair<bool, Point> AttackMainNN::calcBestGoalPoint()
{
    // Populate a vector with robot positions
    std::vector<Point> obstacles;
    auto myTeam = team->getRobots();
    auto oppTeam = team->getOpponents();

    obstacles.reserve(myTeam.size() + oppTeam.size());

    for(Robot* rob : myTeam)
        if(rob->getId() != robot->getId())
            obstacles.push_back(*rob);
    for(Robot* rob : oppTeam)
        obstacles.push_back(*rob);

    // Store clusters of targets
    std::vector<std::vector<Point>> target_clusters;

    std::vector<Point> empty_cluster;
    target_clusters.push_back(empty_cluster);

    // Sample a number of points along opp goal and generate clusters of clear shot points
    for(int goal_y = -Field::GOAL_LENGTH/2+Field::BALL_RADIUS+10; goal_y <= Field::GOAL_LENGTH/2-Field::BALL_RADIUS-10; goal_y += 10)
    {
        auto gp = Field::getGoalPosition(OPPONENT_SIDE);
        Point target = gp + Point(0, goal_y);
        bool clear_shot = true;

        for(const Point& obstacle : obstacles)
        {
            // If there is an obstacle in the way
            if(Measurements::lineSegmentDistance(obstacle, *ball, target) <= Field::BALL_RADIUS+ROBOT_RADIUS+75)//changed constant from 50(prob need to take into account distance from target...)
            {
                clear_shot = false;
                break;
            }
        }

        if(clear_shot)
            target_clusters.back().push_back(target);
        else if(!target_clusters.back().empty())
            target_clusters.push_back(empty_cluster);
    }

    // Find largest cluster and set the midpoint as the final target
    auto max_cluster = std::max_element(target_clusters.begin(), target_clusters.end(),
                                     [](const std::vector<Point>& A, const std::vector<Point>& B)
                                     { return A.size() < B.size(); });

    if(max_cluster->empty())
        // No target found
        return std::pair<bool, Point>(false, Point(0,0));
    else
    {
        // Find average
        Point final_target(0,0);
        for(Point p : *max_cluster)
            final_target += p;
        final_target *= 1.0/(max_cluster->size());

        return std::pair<bool, Point>(true, final_target);
    }
}

std::pair<bool, Point> AttackMainNN::calcBestPassPoint()
{
    std::vector<Robot*> obstacles;
    for(auto* r2 : team->getOpponents()) obstacles.push_back(r2);
    Point bp = *ball;
    Robot* best_supp = nullptr;
    float best_prob = 0;

    for(Robot* teammate : team->getRobots())
    {
        if(teammate != robot && teammate->isInField())
        {
            Point tp = *teammate;
            bool path_clear = Measurements::robotInPath(obstacles, bp, tp, 2*ROBOT_RADIUS+Field::BALL_RADIUS+20) == nullptr;//added 2x

            float t_prob = getScoreProb(tp);
            bool has_score_potential =  t_prob > 0.4;

            bool far_enough = !Measurements::isClose(robot, teammate, 500);
            //qInfo() <<"Path clear: "<<path_clear <<" has_score_potential: "<<has_score_potential <<" Far enough: "<<far_enough ;
            if(path_clear && has_score_potential && far_enough)
            {
                if (best_supp == nullptr
                || (best_supp != nullptr && t_prob > best_prob))
                {
                    best_supp = teammate;
                    best_prob = t_prob;
                }
            }

        }
    }

    if(best_supp == nullptr)
        return std::pair<bool, Point>(false, Point(0,0));
    else
        return std::pair<bool, Point>(true, *best_supp);
}

bool AttackMainNN::isFinished()
{
    //return has_passed;
    return has_passed || has_kicked_to_goal;
}

bool AttackMainNN::hasPassed()
{
    return has_passed;
}

bool AttackMainNN::hasKickedToGoal()
{
    return has_kicked_to_goal;
}

AttackMainNN::~AttackMainNN()
{
    delete score_skill;
    delete pass_skill;
    delete dribble_skill;
    /*for(int i=0; i < prob_field_rows; i++)
        for (int j = 0 ; j < prob_field_cols; j++)
             delete prob_field[i][j].point;*/

    for(int i=0; i < prob_field_rows; i++)
    {
        delete[] prob_field[i];
    }

    delete[] prob_field;
}

float AttackMainNN::getScoreProb(const Point& p)
{
    if(Comparisons::isPointInsideField(p))
    {

        int rows = PF_LENGTH_MAIN/2+(int)p.x/PND_MAIN > PF_LENGTH_MAIN-1 ? PF_LENGTH_MAIN-1 : PF_LENGTH_MAIN/2+(int)p.x/PND_MAIN;
        rows = rows < 0 ? 0 : rows;
        int cols = PF_WIDTH_MAIN/2+(int)p.y/PND_MAIN > PF_WIDTH_MAIN-1 ? PF_WIDTH_MAIN-1 : PF_WIDTH_MAIN/2+(int)p.y/PND_MAIN;
        cols = cols < 0 ? 0 : cols;
        ProbNode pn = prob_field[rows][cols];
        return fmax(0, pn.dynamic_val + pn.static_val);
    }
    return 0;
}

string AttackMainNN::getName() {
    return "Attack Main";
}