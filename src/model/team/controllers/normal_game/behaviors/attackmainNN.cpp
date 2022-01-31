#include "attackmainNN.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

#include <QDebug>



BehaviorRegister<AttackMainNN> AttackMainNN::reg("AttackMainNN");

bool Shot_flag = false;


float AttackMainNN::SCORE_ANGLE_TOLERANCE = 1/180.0*3.14;//ROT_TOLERANCE/2; //7*M_PI/180;
float AttackMainNN::PASS_ANGLE_TOLERANCE  = ROT_TOLERANCE; //7*M_PI/180;

AttackMainNN::AttackMainNN(Robot* robot) : Behavior(robot)
{   if(robot->getId() == 2 )
     qInfo() << "RObot 2 now attacl";

    qInfo()<<"Score tolerance "<<SCORE_ANGLE_TOLERANCE;

    string filename("C:\\Users\\justi\\Downloads\\CanShoot.csv");
    file_out.open(filename, std::ios_base::app);
    string filename2("C:\\Users\\justi\\Downloads\\CanShootRawInfo.csv");
    file_out2.open(filename2, std::ios_base::app);

    string filename3("C:\\Users\\justi\\Downloads\\CanShoot2.csv");
    file_out3.open(filename3, std::ios_base::app);

    prob_field_rows = (Field::FIELD_LENGTH+1)/PND_MAIN;
    prob_field_cols = (Field::FIELD_WIDTH+1)/PND_MAIN;
    prob_field = new ProbNode*[prob_field_rows];
    for(int i=0; i<prob_field_rows; i++) prob_field[i] = new ProbNode[prob_field_cols];
    calcStaticProb();
    dribble_skill = new DribbleToPoint (robot, &kick_point);
    score_skill   = new KickToPointOmniNN(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
    pass_skill    = new KickToPointOmniNN(robot, &kick_point,PASS_ANGLE_TOLERANCE, -1,true);
    //wait_skill    = new DribbleToPoint(robot, robot);

    state = scoring;

    /*prob_field_rows = (Field::FIELD_LENGTH+1)/PND_MAIN;
    prob_field_cols = (Field::FIELD_WIDTH+1)/PND_MAIN;
    prob_field = new ProbNode*[prob_field_rows];
    for(int i=0; i<prob_field_rows; i++) prob_field[i] = new ProbNode[prob_field_cols];*/
    //std::cout << "Attack Main Behavior created"<< std::endl;
}

bool write_flag = false;
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
    //if(Measurements::distance(kick_point, *ball)<Field::BALL_RADIUS*3 + ROBOT_RADIUS +20)
    //state = scoring;

    if(write_flag){
        std::string input;

        std::cin >> input;
        if(input[0] == 'y'){
            file_out2 << to_write2<<std::endl;
            file_out3 << to_write3<<std::endl;
            qInfo() <<"Writing:"<<to_write3.c_str();

        }
        else qInfo()<<"Discarding";
        to_write2 = ""; to_write3 = "";
        write_flag = false;
        robot->setUseOverridenControls(true);

    }
   //
   bool inGoal =  ( ball->x >4500.0 && (abs(ball->y) <Field::DEF_AREA_LENGTH/2) );
    if(Shot_flag){
        CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
        robot->goToPose(cmd);

        Robot* ball_bot = game_state->getRobotWithBall();
        if(ball_bot != nullptr && ball_bot->getTeamId() != robot->getTeamId())
        {   qInfo()<< "NO is this correct? Y(es) or N(o)";
            to_write1 += "NO,";
            to_write2 += "NO,";
            to_write3 += "NO,";

            Shot_flag = false;
            write_flag = true;
            robot->setUseOverridenControls(true);
            return false;
        }
        else if(inGoal){
                qInfo()<< "YES is this correct? Y(es) or N(o)";
                to_write1 +=  "YES,";
                to_write2 += "YES,";
                to_write3 += "YES,";
                Shot_flag = false;
                write_flag = true;
                robot->setUseOverridenControls(true);

            }
            else if(Measurements::mag(ball->getVelocity()) == 0){
                qInfo()<< "NO is this correct? Y(es) or N(o)";
                to_write1 += "NO,";
                to_write2 += "NO,";
                to_write3 += "NO,";
                Shot_flag = false;
                write_flag = true;
                robot->setUseOverridenControls(true);
            }
        return false;
        }
//    else qInfo()<<Measurements::distance(kick_point, *ball);

    switch (state)
    {
    case scoring:
    {   //state = passing;
        //break;
        std::cout << "AttackMainNN: Score" << std::endl;
        robot->setDribble(true);

        //std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        std::pair<bool, Point> goal_eval = std::make_pair<bool, Point>(true, Point(4500, 0));
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
        has_kicked_to_goal = score_skill->perform() || score_skill->isFinished();

        if(has_kicked_to_goal){
           Shot_flag = true;

            qInfo() << "Shoot to score to point " << goal_eval.second;

            float DistanceToPoint = Measurements::distance(robot, goal_eval.second);
            Point goal_point = goal_eval.second;
            float angle_to_goal = atan2(goal_point.y-robot->y,goal_point.x-robot->x);

            qInfo() << robot->getId() << " Shot from" << *robot << " To: " << goal_eval.second<<
                       "Distance: " <<DistanceToPoint;
            for(Robot* opp : team->getOpponents()){
                if(opp->x >robot->x - 500)//Only checking robots between robot and the goal on the horizontal axis. This is not ideal but calculation is cheap.
                {
                    float DistanceToOpp = Measurements::distance(robot, opp);
                    Point OppVelocity =  opp->getVelocity();

//                    float OppDistToIntersect= Measurements::lineDistance(*opp, *robot, goal_eval.second); //Technically Py in terms of the line segment
//                    float OppDistanceOnLine= Measurements::distance(*robot, Measurements::lineSegmentPoint(*opp, *robot, goal_eval.second));  //Technically Px in terms of the line segment
//                    Point UnitVectorBallToGoal = Measurements::unitVector(goal_eval.second - *robot);

//                    //Project velocity onto the unit vector in direction from ball to kickpoint to determine movement to or from kickpoint
//                    Point projection= UnitVectorBallToGoal*(OppVelocity.dot(UnitVectorBallToGoal));
//                    //This is the rejection, or the movement orthogonal to the unit vector direction from ball to kickpoint.
//                    Point rejection = (OppVelocity - projection);


//                    //If opp moving towards goal, Vx is positive. moving towards robot, negative. For some reason comparing the angle doesnt work even if they're the same?
//                    float Vx = Measurements::mag(projection) * ((projection.x/UnitVectorBallToGoal.x>0 && projection.y/UnitVectorBallToGoal.y>0)? 1 : -1);
//                    //If opp moving towards line between robot and kick_point, it is positive, else negative.
//                    float Vy = Measurements::mag(rejection) *(Measurements::lineDistance(*opp+Measurements::unitVector(OppVelocity),*robot,goal_eval.second)<OppDistToIntersect ? 1 : -1);
//                    if(projection.angle()!= UnitVectorBallToGoal.angle())
//                        qInfo() << "Angle inconsistent: " <<projection.angle() <<" Unit vec: " <<UnitVectorBallToGoal.angle();
//                    if(!(projection.x/UnitVectorBallToGoal.x>0 && projection.y/UnitVectorBallToGoal.y>0))
//                        qInfo() << "Messed up: " <<projection <<" Unit vec: " <<UnitVectorBallToGoal;
//                    Point VxVy = OppVelocity.rotate(-UnitVectorBallToGoal.angle());

//                    qInfo() <<"Opponent"<< opp->getId() << " at: " << *opp
//                           << "Distance from robot to opp" << DistanceToOpp
//                           <<"Oppdist to intersect(Py)"
//                           << OppDistToIntersect <<" Opp distance on line(Px)"
//                           <<OppDistanceOnLine
//                           <<"Velocity" << opp->getVelocity()
//                          <<"Line to project on" << ((goal_eval.second - *robot).normalized())
//                         <<"Vx: " << Vx
//                           <<"Vy: " << Vy
//                          <<"Projection" <<projection
//                         <<"Rejection " <<rejection
//                       <<"mag of vx vy " << (Measurements::mag(Point(Vx, Vy)))
//                      <<"ahhh" <<VxVy
//                          ;
//                    //<<"Should be velocity: " <<(projection+rejection)


//                    //DistanceToPoint,DistanceToOpp,OppDistToIntersect,OppDistanceOnLine,Vx,Vy,
//                    file_out<< std::to_string(DistanceToPoint) <<',' << std::to_string(DistanceToOpp)<<','
//                            <<std::to_string(OppDistToIntersect) <<','<< std::to_string(OppDistanceOnLine)<<','<< Vx <<','<<Vy<<',';


                    float opp_x_proj = (opp->x  - robot->x)*cos(angle_to_goal) + (opp->y    -   robot->y)*sin(angle_to_goal);
                    float opp_y_proj = -(opp->x -   robot->x)*sin(angle_to_goal) + (opp->y  -   robot->y)*cos(angle_to_goal);

                    float opp_vx_proj = (opp->getVelocity().x)*cos(angle_to_goal) + (opp->getVelocity().y)*sin(angle_to_goal);
                    float opp_vy_proj = -(opp->getVelocity().x)*sin(angle_to_goal) + (opp->getVelocity().y)*cos(angle_to_goal);
                    float v_towards_ball =opp_vy_proj;
                    if(opp_y_proj>0)
                        v_towards_ball *=-1;


                    to_write3 = std::to_string(DistanceToPoint) +','+std::to_string(angle_to_goal)+','+std::to_string(DistanceToOpp)+','+std::to_string(opp_x_proj)+','+std::to_string(opp_y_proj)+','
                                            +std::to_string(opp_vx_proj)+','+std::to_string(opp_vy_proj)+','+std::to_string(v_towards_ball)+',';
                    qInfo() <<"Dist_to_goal: " <<DistanceToPoint <<"\tAngle_to_goal: " << angle_to_goal<<"\tRobot angle: "<<robot->getOrientation() <<"\tDist_to_opp: "<<DistanceToOpp<<"\tOpp_x: "<<opp_x_proj<<"\tOpp_y: "<< opp_y_proj
                           <<"\nOppV_x"<< opp_vx_proj<<"\tOppV_y: "<< opp_vy_proj<<"\tVtowards_ball: "<<v_towards_ball;

                    to_write2 = std::to_string(robot->x) + ',' + std::to_string(robot->y) + ','+std::to_string(opp->x) +','+ std::to_string(opp->y)+','
                            + std::to_string(OppVelocity.x) +','+std::to_string(OppVelocity.y)+',' +std::to_string(goal_eval.second.x) + ','+std::to_string(goal_eval.second.y) +','+',';

                    /*file_out << "Opponent" + std::to_string(rob->getId())+ " at: " + (*rob).toString().c_str()
                                                                           << "Distance from robot to opp" << std::to_string(Measurements::distance(robot, rob))
                                                                           <<std::to_string(Measurements::lineDistance(*rob, *robot, goal_eval.second))
                                                                           <<"Velocity" << rob->getVelocity().toString().c_str()
                                                                          <<"Line to project on" << ((goal_eval.second - *robot).normalized()).toString().c_str()<<std::endl;*/
                }
            }
        delete score_skill;
        score_skill   = new KickToPointOmniNN(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);


        }


        has_passed = false;

        break;
    }
    case passing:
    {
        std::cout << "AttackMainNN: Pass" << std::endl;
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
        std::cout << "AttackMainNN: Dribble" << std::endl;

        // Evaluate state transition to scoring
        std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        std::pair<bool, Point> pass_eval = calcBestPassPoint();

        if(goal_eval.first)
            clear_shot_count++;
        else
            clear_shot_count = std::max(0, clear_shot_count-1);

        if(clear_shot_count > 10)
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
            if(Measurements::lineSegmentDistance(obstacle, *ball, target) <= (Field::BALL_RADIUS+ROBOT_RADIUS)*3)//75)//changed constant from 50(prob need to take into account distance from target...)
            {
                //qInfo() <<"Not clear" << target <<"Obstacle"<< obstacle;
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
