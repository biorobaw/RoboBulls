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

    max_v = 2.5;/*rSoccer is in meters, we set in mm*/
    max_w = 10;/*rSoccer is in radians, we set in ??*/
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    time_start = std::chrono::high_resolution_clock::now();
    time_last = time_start;

    //Load TorchScript model of our trained actor(pi/policy) to predict actions.
    try {
      // Deserialize the ScriptModule from a file using torch::jit::load().
      actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\ddpg_spinuptester_actor_local_50.pt");
    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return;
    }

}

std::vector<float> ShootToGoalRL::getState(){
    float dist_to_ball = Measurements::distance(*ball, *robot) / 1000.f;

    float theta_s = sin(robot->getOrientation());
    float theta_c = cos(robot->getOrientation());

    //# ωR
    float v_theta = robot->getAngularSpeed();


            //# dr−g
            float dist_to_goal = Measurements::distance(*robot, Point(Field::FIELD_LENGTH/2, 0) ) / 1000.0;//Convert to m

            //# sin(θl), cos(θl)
            //angle_2top_s, angle_2top_c, dist_robot_top = self.get_sin_angle_dist(the_robot, rob_ang, self.goal_post_top)
            //observation.append(angle_2top_s)
            //observation.append(angle_2top_c)

            //#sin(θr), cos(θr) right or bottom
            //angle_2bottom_s, angle_2bottom_c, dist_robot_bottom = self.get_sin_angle_dist(the_robot, rob_ang, self.goal_post_bot)
            //observation.append(angle_2bottom_s)
           // observation.append(angle_2bottom_c)



    return {dist_to_ball, theta_s, theta_c, v_theta, dist_to_goal};
    //return {angle_2ball_s, angle_2ball_c, dist_to_ball, velocity.x, velocity.y, v_theta};
}



void ShootToGoalRL::takeAction(std::vector<float> action){
    //Output of network range [-1, 1]
    float v_theta = action[0] * max_w;
    bool kick = (action[1] > 0) ? true : false;

    robot->setTargetVelocityLocal(Point(0,0), v_theta);
    if(kick)
        robot->setKickSpeed(5000); /*our max was: 7500... used train env max instead*/

    return;
}

#include <torch/script.h>
bool ShootToGoalRL::perform(){
    auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-time_start);
    qInfo()<<"cycle time: " <<cycle_time.count()/1000.0;
    //get Observation and convert it to tensor
    std::vector<float> observation = getState();
    torch::Tensor t_observation = torch::tensor(observation, torch::dtype(torch::kFloat)).to(torch::kCUDA);
    printState(observation);

    //Need another conversion to use TorchScript model (see: https://pytorch.org/tutorials/advanced/cpp_export.html)
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(t_observation);

    //Get actions from our trained actor
//    at::Tensor t_actions =actor.forward(inputs).toTensor();
//    std::cout << "\n**********\nActions"<<t_actions<<"\n***************"<<std::endl;
//    t_actions = t_actions.to(torch::kCPU);

    at::Tensor t_actions =actor.forward(inputs).toTensor().to(torch::kCPU);
    std::vector<float> actions(t_actions.data_ptr<float>(), t_actions.data_ptr<float>() + t_actions.numel());
    //qInfo()<<"Actions as vector";
    //for(auto action : actions)
        //qInfo() << action;
    //set actions

    //calculate reward from previous action.

    time_start = std::chrono::high_resolution_clock::now();
    takeAction(actions);

    //Set done
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
                     <<"Theta: "<<atan2(observation[0], observation[1])<<"\tsin(theta): " <<observation[0] << "\tcos(theta): " <<observation[1]
                     <<"\ndistance_to_ball: " <<observation[2] << "\nvx(meters): " <<observation[3] << "\tvy(meters): "
                    <<observation[4]<< "\tvtheta(radians/sec): " <<observation[5] <<"#-------------------------------";
}



bool ShootToGoalRL::isFinished(){
   return done;
}

string ShootToGoalRL::getName(){
    return "Shoot to goal RL";
}
