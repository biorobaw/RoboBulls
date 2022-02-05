#include "attackmainNN.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

#include <stdlib.h>
#include <stdio.h>

#include <QDebug>


//#include "tensorflow/c/c_api.h"

//#include "tensorflow/core/public/session.h"
//#include "tensorflow/core/platform/env.h"
//#include "tensorflow/cc/ops/image_ops.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::vector<float>> samps
{
{3168.156,2577.349,2529.922,-492.165},  {2769.937,2426.636,2388.003,431.281},   {2897.41,2777.918,2737.355,-472.986},   {3268.939,2400.97,2360.632,-438.262},   {3270.526,2302.523,2268.59,-393.846},   {3536.929,2868.707,2840.562,400.86},    {2949.132,2668.031,2607.894,563.272},   {3290.528,3135.006,3101.448,457.474},
{3671.171,3487.215,3446.999,528.082},   {3230.483,2393.862,2362.003,389.249},   {2736.166,2311.375,2276.244,-401.458},  {2572.556,1943.382,1906.071,378.983},   {3324.646,2763.532,2730.862,-423.679},  {3640.88,3302.903,3268.215,-477.432},   {4828.906,4733.643,4713.543,435.75},    {3308.738,2973.083,2939.672,-444.471},  {3299.784,2910.163,2882.757,398.446},   {3065.208,2584.273,2550.521,-416.305},
{3170.439,2966.62,2939.891,397.338},    {2925.01,2731.902,2673.991,559.519},    {3642.861,3299.906,3272.797,-422.115},  {2670.714,2307.666,2274.432,390.227},   {3158.798,2762.996,2721.957,474.44,},   {3347.33,3089.659,3041.493,543.427},    {3129.673,2704.658,2668.215,-442.499},  {2929.921,2620.466,2587.505,414.322},
{3204.707,2315.551,2278.66,-411.686},   {3032.161,2462.843,2417.334,471.261},   {3238.767,2445.34,2416.733,-372.944},   {3081.35,2732.85,2691.677,-472.593},        {2152.068,1899.571,1857.752,396.392},   {3053.561,2858.584,2826.604,426.396},   {3152.209,2385.345,2355.916,-373.533},  {3159.502,2849.427,2817.307,426.635},
{2304.448,1784.394,1733.627,-422.609},  {2760.888,1836.825,1794.412,-392.442},  {3397.238,3136.72,3083.229,576.815},    {3344.082,2839.312,2810.7,-402.067},        {2903.621,2557.403,2511.28,483.51,},    {2870.324,2351.403,2319.327,387.064},   {3823.418,3427.157,3398.684,440.856},   {3138.003,3003.541,2973.137,-426.276},
{2475.62,2185.156,2148.245,399.935},    {2790.41,2584.027,2543.598,455.307},    {2555.262,2221.939,2187.766,388.19,},   {2828.096,1957.056,1893.892,-493.197},  {2396.002,2190.377,2116.006,-565.923},  {3973.751,3865.334,3836.315,-472.754},  {3081.069,2214.174,2180.432,-385.073},  {3115.34,3008.131,2973.052,458.051},
{3213.585,2916.11,2869.197,-520.964},   {3199.73,2680.399,2644.089,439.697},    {2874.101,2572.904,2524.856,494.909},   {2896.655,2583.833,2540.76,469.823},    {2706.718,2427.227,2385.023,450.66,},   {3479.363,3332.146,3306.206,-414.969},  {2521.014,2348.845,2313.111,-408.152},  {4116.724,3716.616,3695.176,398.64,},
{3916.684,3776.351,3744.159,492.032},   {4023.992,3859.026,3805.632,-639.722},  {3249.888,2258.229,2213.352,447.961},   {3017.535,2409.11,2380.185,-372.202},   {3095.273,2825.317,2787.504,460.688},   {3341.901,2372.956,2328.,459.71},   {3025.805,2679.671,2643.998,-435.788},  {2736.761,2149.797,2109.128,-416.176},
{4140.657,3819.603,3777.711,564.153},   {3697.415,3443.881,3409.677,484.169},   {4017.626,3736.793,3714.862,-404.25,},  {3454.347,2490.639,2457.959,402.141},
{3085.199,2815.702,2777.207,-464.006},  {3163.941,2995.831,2965.356,426.225},   {3410.385,2422.544,2390.64,391.869},    {2426.079,2207.97,2170.746,403.73,},    {2887.056,2238.841,2172.16,-542.341},   {2600.529,2430.207,2380.751,487.777},   {2942.603,2795.294,2766.283,401.681},   {3448.615,3217.297,3169.898,-550.221},
{3107.253,2931.917,2899.248,436.464},   {3025.261,2832.277,2803.039,-405.911},  {3368.411,3085.294,3041.272,-519.331},  {3250.883,2876.755,2839.747,-459.951},  {3366.849,3063.166,3033.443,-425.689},  {3082.619,2853.423,2800.433,547.358},   {3285.242,2682.497,2655.942,-376.517},  {3258.026,2769.435,2739.846,403.748},   {4112.292,3992.507,3970.835,-415.427},  {3939.352,3562.604,3540.899,392.656},
{2517.315,1862.228,1812.491,-427.511},  {3133.1,2857.892,2822.01,451.455},  {3339.714,3211.055,3174.771,-481.351},  {3740.49,3497.798,3455.822,540.266},
{2901.907,2792.202,2750.234,-482.286},  {3771.146,2987.099,2956.378,-427.303},  {3194.473,2485.77,2452.909,-402.85,},   {3286.182,3066.358,3028.297,-481.635},
{2854.426,2652.078,2593.318,555.174},   {3752.269,3075.883,3041.836,456.39,},   {2999.219,2082.659,2031.491,-458.819},  {2368.531,2045.447,2004.638,-406.544},
{3747.571,2828.787,2798.192,414.917},   {2623.791,2268.812,2223.08,-453.232},   {3013.288,2104.401,2061.909,-420.757},  {2973.624,2571.478,2521.346,-505.284},  {2179.458,1880.949,1834.34,416.132},
{3383.767,3139.53,3104.513,467.596},    {4316.053,3636.113,3609.262,-441.072},  {3589.949,3122.279,3093.069,426.09,},   {2405.931,2016.513,1960.981,-469.977},
{2681.21,2496.43,2464.55,-397.69,}, {2710.969,2015.816,1972.256,416.796},   {2495.636,2103.749,2054.834,-451.016},{3348.532,2499.502,2467.38,-399.432}, {3213.546, 2847.354, 2819.091, -400.188},   {3104.243, 2775.31,  2723.537,  533.566},
{3433.785, 3076.634, 3043.447,  450.681},   {4026.809, 3796.957, 3770.829,  444.674}
};

void NoOpDeallocator(void* data, size_t a, void* b) {}


BehaviorRegister<AttackMainNN> AttackMainNN::reg("AttackMainNN");

bool Shot_flag = false;

std::string to_print2 ="";
std::string to_print3 ="";

float AttackMainNN::SCORE_ANGLE_TOLERANCE = 1/180.0*3.14;//ROT_TOLERANCE/2; //7*M_PI/180;
float AttackMainNN::PASS_ANGLE_TOLERANCE  = ROT_TOLERANCE; //7*M_PI/180;


//Number of tensors being input. Number of tensors we get back.
int NumInputs = 1;
int NumOutputs = 1;

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
    //score_skill   = new KickToPointOmniNN(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
    //pass_skill    = new KickToPointOmniNN(robot, &kick_point,PASS_ANGLE_TOLERANCE, -1,true);
    score_skill   = new KickToPointOmni(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
    pass_skill    = new KickToPointOmni(robot, &kick_point,PASS_ANGLE_TOLERANCE, -1,true);
    //wait_skill    = new DribbleToPoint(robot, robot);

    state = scoring;

    //##################################################################################
    // from: https://medium.com/analytics-vidhya/deploying-tensorflow-2-1-as-c-c-executable-1d090845055c
    //********* Read model
    Graph = TF_NewGraph();
    Status = TF_NewStatus();
    SessionOptions = TF_NewSessionOptions();

    const char* saved_model_folder = "C:\\Users\\justi\\Documents\\ThesisRL\\model\\";
    const char* tags = "serve";

    Session = TF_LoadSessionFromSavedModel(SessionOptions, /*run_opts*/NULL, saved_model_folder, &tags, /*num_tags*/1, Graph, /*meta_graph_def*/NULL, Status);

    if(TF_GetCode(Status) == TF_OK)
        {   printf("TF_LoadSessionFromSavedModel OK\n");    }
    else
        {   printf("%s",TF_Message(Status));                }

    //****** Get input tensor
    Input = (TF_Output*) malloc(sizeof(TF_Output) * NumInputs);
    TF_Output t0 = {TF_GraphOperationByName(Graph, "serving_default_batch_normalization_6_input"), 0};

    if(t0.oper == NULL)
        printf("ERROR: Failed TF_GraphOperationByName\n");
    else
        printf("TF_GraphOperationByName is OK\n");

    Input[0] = t0;

    //********* Get Output tensor
    Output = (TF_Output*)malloc(sizeof(TF_Output) * NumOutputs);
    TF_Output t2 = {TF_GraphOperationByName(Graph, "StatefulPartitionedCall"), 0};

    if(t2.oper == NULL)
        printf("ERROR: Failed TF_GraphOperationByName StatefulPartitionedCall\n");
    else
      printf("TF_GraphOperationByName StatefulPartitionedCall is OK\n");

    Output[0] = t2;

    /*prob_field_rows = (Field::FIELD_LENGTH+1)/PND_MAIN;
    prob_field_cols = (Field::FIELD_WIDTH+1)/PND_MAIN;
    prob_field = new ProbNode*[prob_field_rows];
    for(int i=0; i<prob_field_rows; i++) prob_field[i] = new ProbNode[prob_field_cols];*/
    //std::cout << "Attack Main Behavior created"<< std::endl;
}


int num_ins= 4;
//state input: DistanceToGoal  DistanceToOpp         OppX        OppY. (Takes ~ 1 ms avg. (1-5 seconds range on cpu)
float AttackMainNN::PredictShot(float dist_goal, float dist_opp, float oppx, float oppy){
        //********* Allocate data for inputs & outputs
        TF_Tensor** InputValues  = (TF_Tensor**)malloc(sizeof(TF_Tensor*)*NumInputs);
        TF_Tensor** OutputValues = (TF_Tensor**)malloc(sizeof(TF_Tensor*)*NumOutputs);

        int ndims = 2;      int64_t dims[] = {1,num_ins};       int ndata = sizeof(float_t)*num_ins;
        float_t data[] = {dist_goal, dist_opp, oppx, oppy};

        TF_Tensor* float_tensor = TF_NewTensor(TF_FLOAT, dims, ndims, data, ndata, &NoOpDeallocator, 0);

        if (float_tensor == NULL)
            {   std::cout << ("ERROR: Failed TF_NewTensor\n");  throw std::exception();}


        InputValues[0] = float_tensor;

        // Run the Session
        TF_SessionRun(Session, NULL, Input, InputValues, NumInputs, Output, OutputValues, NumOutputs, NULL, 0,NULL , Status);

        if(TF_GetCode(Status) != TF_OK)
            {   printf("%s",TF_Message(Status));    throw std::exception(); }


        void* buff = TF_TensorData(OutputValues[0]);
        float result = *( (float*) buff);

        TF_DeleteTensor(float_tensor);      TF_DeleteTensor(OutputValues[0]);   //cleanup. Maybe need to delete pointer to pointers?"InputValues,OutputValues"
        //std::cout << "Probability of shot Tensor :\n" << result<<std::endl;

        return result;
}

bool write_flag = false;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

bool AttackMainNN::perform()
{
//    std::vector<float> out;
//    for(auto entry : samps){
//        out.push_back(PredictShot(entry[0], entry[1], entry[2],  entry[3]));
//    }
    //qInfo()<<"Out: ";   for(auto res : out) qInfo()<<res;


    if(write_flag){
        //qInfo() <<"Writing";
        std::string input;      std::cin >> input;

        if(input[0] == 'y'){
            file_out2 << to_write2<<std::endl;
            file_out3 << to_write3<<std::endl;
            qInfo() << ++writecounter<<": Writing:"<<to_write3.c_str();

        }
        else
            qInfo()<<"Discarding";
        to_write2 = "";             to_write3 = "";
        write_flag = false;         robot->setUseOverridenControls(true);
        prediction = false;
        nn_prob = -1;
    }



    if(Shot_flag){
        //qInfo() <<"Already shot waiting for result";
        CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
        robot->goToPose(cmd);

        Robot* ball_bot = game_state->getRobotWithBall();

        bool inGoal =  ( ball->x >4500.0 && (abs(ball->y) <Field::GOAL_LENGTH/2) );

       if(inGoal){
                std::string pred_string = (prediction == true ? ",YES," : ",NO,") + std::to_string(nn_prob);

                qInfo() /*<< "BallLoc: " <<*ball */<<"\nGoal was successful(YES) is this correct? Y(es) or N(o)" <<"\nPredicted: " <<pred_string.c_str() <<"Numyes"<<++yes_counter;
                to_write1 +=  "YES";   to_write2 += "YES";    to_write3 += "YES";
                to_write1 +=pred_string;   to_write2 +=pred_string;    to_write3 +=pred_string;

                Shot_flag = false;  write_flag = true;

                robot->setUseOverridenControls(true);

            }
            else if((ball_bot != nullptr && ball_bot->getTeamId() != robot->getTeamId()) || Measurements::mag(ball->getVelocity()) == 0){
                std::string pred_string = (prediction == true ? ",YES," : ",NO,") + std::to_string(nn_prob);

                qInfo() /*<< "BallLoc: " <<*ball*/<< "Goal was unsuccessful(NO) is this correct? Y(es) or N(o)" <<"\nPredicted: " <<pred_string.c_str()<<"Numno"<<++no_counter;
                to_write1 += "NO";     to_write2 += "NO";     to_write3 += "NO";
                to_write1 +=pred_string;   to_write2 +=pred_string;    to_write3 +=pred_string;

                Shot_flag = false;  write_flag = true;

                robot->setUseOverridenControls(true);
            }
        return false;
        }


    switch (state)
    {
    case scoring:
    {
        //std::cout << "AttackMainNN: Score" << std::endl;
        robot->setDribble(true);

        std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        //std::pair<bool, Point> goal_eval = std::make_pair<bool, Point>(true, Point(4500, 0));
        if(goal_eval.first)
        {
            //qInfo() << "Kick point " << goal_eval.second;
            kick_point = goal_eval.second;
            clear_shot_count = std::min(15/*30*/, clear_shot_count+1);
        }
        else
            clear_shot_count--;


        if(clear_shot_count < 0 || has_kicked_to_goal)//if kicked to goal
        {
            clear_shot_count = 0;
            //state = passing;
            state = dribbling;
        }




        //If we kicked the previous cycle or are kicking this cycle(kicking this cycle should take dominance)
        score_skill->perform();
        has_kicked_to_goal =  score_skill->isFinished(); //Returns whether we have kicked
        //Point* shot_to = score_skill->getShotPoint();

        //Now log info
        if(has_kicked_to_goal){
               Shot_flag = true;

               Point goal_point = kick_point;   //Point goal_point = *shot_to;

                float DistanceToPoint = Measurements::distance(robot, goal_point);
                float angle_to_goal = atan2(goal_point.y-robot->y,goal_point.x-robot->x);

                qInfo() << robot->getId() << " Shot from" << *robot << " To: " << goal_eval.second<<"Distance: " <<DistanceToPoint;

                for(Robot* opp : team->getOpponents()){
                    if(opp->x >robot->x - 500){//Only checking robots between robot and the goal on the horizontal axis. This is not ideal but calculation is cheap.
                            float DistanceToOpp = Measurements::distance(robot, opp);
                            Point OppVelocity =  opp->getVelocity();

                            float opp_x_proj = (opp->x  - robot->x)*cos(angle_to_goal) + (opp->y    -   robot->y)*sin(angle_to_goal);
                            float opp_y_proj = -(opp->x -   robot->x)*sin(angle_to_goal) + (opp->y  -   robot->y)*cos(angle_to_goal);

                            float opp_vx_proj = (opp->getVelocity().x)*cos(angle_to_goal) + (opp->getVelocity().y)*sin(angle_to_goal);
                            float opp_vy_proj = -(opp->getVelocity().x)*sin(angle_to_goal) + (opp->getVelocity().y)*cos(angle_to_goal);
                            float v_towards_ball =opp_vy_proj;
                            if(opp_y_proj>0)
                                v_towards_ball *=-1;


                            nn_prob = PredictShot(DistanceToPoint, DistanceToOpp, opp_x_proj, opp_y_proj);
                            prediction = (nn_prob>=0.5 ? true : false);

                            to_write3 = std::to_string(DistanceToPoint) +','+ std::to_string(angle_to_goal)  +',' + std::to_string(DistanceToOpp) +','
                                      + std::to_string(opp_x_proj)      +','+ std::to_string(opp_y_proj)     +','
                                      + std::to_string(opp_vx_proj)     +','+ std::to_string(opp_vy_proj)    +',' +std::to_string(v_towards_ball)+',';

                            qInfo() << "Probability from neural network: " <<nn_prob<<"\nDist_to_goal: " <<DistanceToPoint <<"\tAngle_to_goal: " << angle_to_goal<<"\tRobot angle: "<<robot->getOrientation()
                                                       <<"\tDist_to_opp: "  <<DistanceToOpp <<  "\tOpp_x: " <<opp_x_proj    <<  "\tOpp_y: " << opp_y_proj
                                                       <<"\nOppV_x"         << opp_vx_proj  <<"\tOppV_y: "  << opp_vy_proj  <<  "\tVtowards_ball: " <<v_towards_ball;

                            to_write2 = std::to_string(robot->x)      + ',' + std::to_string(robot->y)  + ','+std::to_string(robot->getVelocity().x)       +',' + std::to_string(robot->getVelocity().y)
                                      +','+std::to_string(robot->getAngularSpeed())
                                      +','+ std::to_string(ball->x)   +','  + std::to_string(ball->y)   +',' + std::to_string(ball->getVelocity().x)       +','  + std::to_string(ball->getVelocity().y)
                                      + ','+std::to_string(opp->x)       +',' + std::to_string(opp->y)       +','
                                      + std::to_string(OppVelocity.x) +','  + std::to_string(OppVelocity.y)+',' +std::to_string(goal_point.x) + ','+ std::to_string(goal_point.y) +',';

                        }
                }
            //Deleting this to avoid double writing since skill is still **Finished**
            delete score_skill;
            //score_skill   = new KickToPointOmniNN(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
            score_skill   = new KickToPointOmni(robot, &kick_point,SCORE_ANGLE_TOLERANCE,-1,true);
            qInfo() <<"Score Skill reset";

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
        //std::cout << "AttackMainNN: Dribble" << std::endl;

        // Evaluate state transition to scoring
        std::pair<bool, Point> goal_eval = calcBestGoalPoint();
        std::pair<bool, Point> pass_eval = calcBestPassPoint();

        //Point targ_point = (goal_eval.first ? goal_eval.second : Point(Field::FIELD_LENGTH/2, 0));
        //float targ_angle = Measurements::angleBetween(*robot, targ_point);
        //CmdGoToPose temp_cmd(/*Point target_pose*/ *robot, /*float target_angle*/targ_angle,/*bool avoid_obstacles */false,/* bool avoid_ball */ false);
        //robot->goToPose(temp_cmd);



        //*****************************

        if(goal_eval.first)
            clear_shot_count++;
        else
            clear_shot_count = std::max(0, clear_shot_count-1);

        if(clear_shot_count > 3/*10*/)
        {
            clear_shot_count = 0;
            state = scoring;
            break;
        }


//        has_kicked_to_goal = false;
//        has_passed = false;
//        break;

        // Evaluate state transition to passing
//        if(pass_eval.first)
//            clear_pass_count++;
//        else
//            clear_pass_count = std::max(0, clear_pass_count-1);

//        if(clear_pass_count > 30)
//        {
//            clear_pass_count = 0;
//            state = passing;
//            break;
//        }

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
            if(Measurements::lineSegmentDistance(obstacle, *ball, target) <= /*Field::BALL_RADIUS+ROBOT_RADIUS+75 */(Field::BALL_RADIUS+ROBOT_RADIUS)*3)//75)//changed constant from 50(prob need to take into account distance from target...)
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

    // Free memory
    TF_DeleteGraph(Graph);
    TF_DeleteSession(Session, Status);
    TF_DeleteSessionOptions(SessionOptions);
    TF_DeleteStatus(Status);

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


