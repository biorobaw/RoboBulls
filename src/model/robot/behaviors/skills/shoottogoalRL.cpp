#include "shoottogoalRL.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "utilities/debug.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/game_state.h"
#include "model/team/team.h"
#include "utilities/measurements.h"
#include <iostream>

#include <QDebug>
/*
 *Utilizing a similar state action space as pruposed in: https://zhuyifengzju.github.io/files/2018Robocup.pdf
 *By Carnegie Mellon University
 *
 */

//float BEHIND_RAD_AVOID = ROBOT_RADIUS+Field::BALL_RADIUS + DIST_TOLERANCE; //was 50
//float BEHIND_RAD = ROBOT_RADIUS+Field::BALL_RADIUS;
//float FORWARD_WAIT_COUNT = 5;
//float RECREATE_DIST_TOL = 25;
//float STRICTEST_ANG_TOL = ROT_TOLERANCE;  //40 * (M_PI/180);
//float KICK_LOCK_ANGLE =  ROT_TOLERANCE;  //12 * (M_PI/180);
//float KICKLOCK_COUNT = 15;
//#endif
string getTimeStamp();


#include <chrono>

/************************************************************************/


ShootToGoalRL::ShootToGoalRL(Robot* robot, float targetTolerance)
    : Behavior(robot)
{
    //It is important to note the model may have been trained on a different size field and may not work.
    half_model_field_length = 4.5 * 1000;
    half_model_goal_width = .5 * 1000;
    gp_top = Point(half_model_field_length,  half_model_goal_width /*Field::GOAL_LENGTH/2*/);
    gp_bottom = Point(half_model_field_length, -half_model_goal_width/*(Field::GOAL_LENGTH/2)*/);
    gp_mid = Point(half_model_field_length, 0);


    max_v = 2.5;/*rSoccer is in meters, we set in mm*/
    max_w = 10;/*rSoccer is in radians, we set in ??*/
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    time_start = std::chrono::high_resolution_clock::now();
    time_last = time_start;

    prev_ball = *ball;
    prev_angle = robot->getOrientation();

    //Load TorchScript model of our trained actor(pi/policy) to predict actions.
    try {
      // Deserialize the ScriptModule from a file using torch::jit::load().
      actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\Feb13_Shoot100.pt");
      //actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\March5Shoot100.pt");


      //March5Shoot100
      //actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\Feb_6100.pt");

    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return;
    }

}

std::vector<float> ShootToGoalRL::getState(){

    Point ball_proj = ( (*ball-*robot).rotate(-(robot->getOrientation())) / 1000.f); //translational
    Point ball_v_proj = (ball->getVelocity().rotate(-(robot->getOrientation())) / 1000.f); //translational


//            float ball_x = (ball->x - robot->x) / 1000.f;
//            float ball_y  = (ball->y- robot->y) / 1000.f;
//            float rob_ang = robot->getOrientation();
//            ball_x = ball_x*cos(rob_ang) + ball_y*sin(rob_ang);
//            ball_y = -ball_x*sin(rob_ang) + ball_y*cos(rob_ang);


//            float ball_v_x = ball->getVelocity().x / 1000.f;
//            float ball_v_y = ball->getVelocity().y / 1000.f;

//            auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-time_start).count()/1000.f;
//            //**NOT** a good way to do this. Kalman filter is destroying velocity detection, so to circumvent it, doing this.
//            //Really should be implemented a better way to get a more true velocity for the ball.
//            float ball_v_x = ( (ball->x -prev_ball.x )/cycle_time )/ 1000.f;
//            float ball_v_y = ( (ball->y -prev_ball.y )/cycle_time) / 1000.f;
//            prev_ball = *ball;
//            ball_v_x = ball_v_x*cos(rob_ang) + ball_v_y*sin(rob_ang);
//            ball_v_y = -ball_v_x*sin(rob_ang) + ball_v_y*cos(rob_ang);
//float v_theta = Measurements::angleDiff(robot->getOrientation(), prev_angle) / cycle_time;
//            prev_angle=robot->getOrientation();

    //# ωR
    float v_theta = robot->getAngularSpeed();

    //# dr−g
    float dist_to_goal = Measurements::distance(*robot, gp_mid) / 1000.f;//Convert to m

    float top_angle_diff= Measurements::angleDiff(Measurements::angleBetween(*robot, gp_top), robot->getOrientation());
    float top_angle_s = sin(top_angle_diff);
    float top_angle_c = cos(top_angle_diff);

    float bottom_angle_diff= Measurements::angleDiff(Measurements::angleBetween(*robot, gp_bottom), robot->getOrientation());
    float bottom_angle_s = sin(bottom_angle_diff);
    float bottom_angle_c = cos(bottom_angle_diff);

    return {ball_proj.x, ball_proj.y, ball_v_proj.x, ball_v_proj.y, v_theta, dist_to_goal, top_angle_s, top_angle_c, bottom_angle_s, bottom_angle_c};

    //return {ball_x, ball_y, ball_v_x, ball_v_y, v_theta, dist_to_goal, top_angle_s, top_angle_c, bottom_angle_s, bottom_angle_c};
    //return {angle_2ball_s, angle_2ball_c, dist_to_ball, velocity.x, velocity.y, v_theta};
}



void ShootToGoalRL::takeAction(std::vector<float> action){
    //Output of network range [-1, 1]
    float v_theta = action[0] * max_w;

    bool kick = (action[1] > 0) ? true : false;

    //bool dribble  = (!kick && robot->hasBall()) ? true : false;
    bool dribble  = true;

    robot->setDribble(dribble);

    //qInfo() << "Target velocity: " <<v_theta << " Kick: "<<kick;

    robot->setTargetVelocityLocal(Point(0,0), v_theta);
    if(kick)
        robot->setKickSpeed(5000); /*our max was: 7500... used train env max instead*/

    return;
}

#include <torch/script.h>
bool ShootToGoalRL::perform(){
    //auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-time_start);
    //qInfo()<<"cycle time: " <<cycle_time.count()/1000.0;

    //get Observation and convert it to tensor
    std::vector<float> observation = getState();
    torch::Tensor t_observation = torch::tensor(observation, torch::dtype(torch::kFloat)).to(torch::kCUDA);
    //printState(observation);

    //Need another conversion to use TorchScript model (see: https://pytorch.org/tutorials/advanced/cpp_export.html)
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(t_observation);

    //Get actions from our trained actor
    at::Tensor t_actions =actor.forward(inputs).toTensor().to(torch::kCPU);
    std::vector<float> actions(t_actions.data_ptr<float>(), t_actions.data_ptr<float>() + t_actions.numel());

    //set actions
    takeAction(actions);

    //Set done

    //time_start = std::chrono::high_resolution_clock::now();

    return false;
}


std::pair<float, bool> ShootToGoalRL::getRewardAndDone(float d, float theta_r_b){
   float reward = 0;    //rtotal = rcontact + rdistance + rorientation

   if(robot->hasBall()){ //contact...perhaps change to robot has ball on dribbler like in paper?
       reward+= 100.f;
       done = true;
   }


   //distance
   reward += ( (5.f/sqrt(2.f*M_PI))*exp(-(d*d)/2.f) )- 2.f;
   //orientation
   reward += static_cast<float>( (1.f/sqrt(2.f*M_PI))*exp(-2.f*(theta_r_b/(M_PI*M_PI))) );

   return std::pair(reward, done);
}

void ShootToGoalRL::printState(std::vector<float> observation){
           qInfo() << "#-------------------------------Observation"
                     <<"ballProj: ("<<observation[0] <<',' <<observation[1]<<"\tBallv: (" <<observation[2] << "," <<observation[3]
                     <<")\nVTheta: " <<observation[4] << "\ndistgoal: " <<observation[5] << "\topangles: "
                    <<observation[6]<< "\ttopanglec: " <<observation[7]
                   << "\bottomangles: "<<observation[8]<< "\tbottomanglec: " <<observation[9]<<"#-------------------------------";
}



bool ShootToGoalRL::isFinished(){
   return done;
}

string ShootToGoalRL::getName(){
    return "Shoot to goal RL";
}
