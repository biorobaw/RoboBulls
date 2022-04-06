#include "gotoballRL.h"
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

//#endif
string getTimeStamp();

#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_stop, time_last;
/************************************************************************/


GoToBallRL::GoToBallRL(Robot* robot, float targetTolerance)
    : Behavior(robot)
{
    max_v = 1.5; /*2.5;*//*rSoccer is in meters, we set in mm*/
    max_w = 1.395; /*10;*//*rSoccer is in radians, we set in ??*/

    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    time_start = std::chrono::high_resolution_clock::now();
    time_last = time_start;

    //Load TorchScript model of our trained actor(pi/policy) to predict actions.
    try {
      // Deserialize the ScriptModule from a file using torch::jit::load().
      //actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\ddpg_spinuptester_actor_local_50.pt");
      actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\Feb_6GTB_robobullspeeds100.pt");
      //actor = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\Feb13GTB100.pt");


    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return;
    }

}

std::vector<float> GoToBallRL::getState(){
    float angle_to_ball =Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation());

    float angle_2ball_s = sin(angle_to_ball);
    float angle_2ball_c = cos(angle_to_ball);
    float dist_to_ball = Measurements::distance(*ball, *robot) / 1000.f;


    Point velocity = (robot->getVelocity().rotate(-(robot->getOrientation())) / 1000.f); //translational
    float v_theta = robot->getAngularSpeed();

    return {angle_2ball_s, angle_2ball_c, dist_to_ball, velocity.x, velocity.y, v_theta};
}



void GoToBallRL::takeAction(std::vector<float> action){
    //Output of network range [-1, 1]
    float v_x = action[0] * max_v;
    float v_y = action[1] * max_v;
    float v_theta = action[2] * max_w;

    //# clip by max absolute
    Point v(v_x,v_y);
    float v_norm = Measurements::mag(v);
    if(v_norm > max_v)
        v *= (max_v / v_norm);

    //qInfo() <<"Target Vel: "<<v <<"\tv_theta: "<<v_theta;
    robot->setTargetVelocityLocal(v*1000.0, v_theta);

    return;
}

#include <torch/script.h>
bool GoToBallRL::perform(){
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

    //time_start = std::chrono::high_resolution_clock::now();
    takeAction(actions);

    //Set done
    return false;
}


std::pair<float, bool> GoToBallRL::getRewardAndDone(float d, float theta_r_b){
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

void GoToBallRL::printState(std::vector<float> observation){
           qInfo() << "#-------------------------------Observation"
                     <<"Theta: "<<atan2(observation[0], observation[1])<<"\tsin(theta): " <<observation[0] << "\tcos(theta): " <<observation[1]
                     <<"\ndistance_to_ball: " <<observation[2] << "\nvx(meters): " <<observation[3] << "\tvy(meters): "
                    <<observation[4]<< "\tvtheta(radians/sec): " <<observation[5] <<"#-------------------------------";
}



bool GoToBallRL::isFinished(){
   return done;
}

string GoToBallRL::getName(){
    return "Go to ball RL";
}


