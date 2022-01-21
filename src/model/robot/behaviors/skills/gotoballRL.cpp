#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "utilities/debug.h"
#include "gotoballRL.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/game_state.h"
#include "model/team/team.h"
#include <iostream>
#include "utilities/measurements.h"

#include "ddpg.h"

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

int max_episode_length= 100/*350*/;
int num_episodes = 100/*500*/ /*1500*/;
int burn_in_time = -1;
int random_burn_in_time = -1;
int save_frequency= 5;
float max_speed = 1500;     float max_ang_speed = 80/180.0*3.14;
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_stop, time_last;
/************************************************************************/


GoToBallRL::GoToBallRL(Robot* robot, float targetTolerance)
    : Behavior(robot)
    , agent(/*num observations*/2/*9*/,/*num actions*/ 1, max_episode_length, num_episodes, 500/*25*burn_in_time*/)
{
    train = true;
    episode_status.episode_complete = false; episode_status.train_complete = false;
    done = false;
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    time_start = std::chrono::high_resolution_clock::now();
    time_last = time_start;
    //agent.loadBuffer("replay_buffer_angle");
    //agent.load("actor_angle10","critic_angle10");
    //agent.saveBuffer("replaybuffer_overfit_meters_4in");
    //agent.printBuffer();
//    for(int i = 0; i <4000; i++)
//        agent.update();

    torch::jit::script::Module module;
    try {
      // Deserialize the ScriptModule from a file using torch::jit::load().
      module = torch::jit::load("C:\\Users\\justi\\Documents\\ThesisRL\\rSoccer\\ddpg_spinuptester_actor_local_50.pt");
    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return;
    }
    test= module;

}

std::vector<float> GoToBallRL::getState(){
    //State input - convert everything to meters so it doesnt hold more weight in NN over actions.
    Point ball_pos = (  (*ball - *robot).rotate(-(robot->getOrientation())) / 1000.f); //relative to robot
    float angle_to_ball =Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation());
    float angle_2ball_s = sin(angle_to_ball);
    float angle_2ball_c = cos(angle_to_ball);
    qInfo() <<"Ange to ball: " <<angle_to_ball<<"\nsin: "<<angle_2ball_s<<"\ncos: "<<angle_2ball_c;

    Point velocity = (robot->getVelocity().rotate(-(robot->getOrientation())) / 1000.f); //translational
    float v_theta = robot->getAngularSpeed();
    float dist_to_ball = Measurements::distance(*ball, *robot) / 1000.f;

    //return {/*dist_to_ball,*/ angle_2ball_s, angle_2ball_c};
    return {angle_2ball_s, angle_2ball_c, dist_to_ball, velocity.x, velocity.y, v_theta};

//    float dist_to_top_field = (Field::FIELD_WIDTH/2 - robot->y)/ 1000.f;
//    float dist_to_right_field = (Field::FIELD_LENGTH/2 - robot->x)/ 1000.f;
//    float dist_to_bot_field =  (robot->y - (-Field::FIELD_WIDTH/2))/1000.f;
//    float dist_to_left_field = (robot->x - (-Field::FIELD_LENGTH/2))/1000.f;

    return {/*ball_pos.x, ball_pos.y,*/ angle_to_ball, velocity.x, velocity.y, dist_to_ball /*dist_to_top_field, dist_to_right_field, dist_to_bot_field, dist_to_left_field*/};

    //return {ball_pos.x, ball_pos.y, velocity.x, velocity.y, dist_to_ball /*dist_to_top_field, dist_to_right_field, dist_to_bot_field, dist_to_left_field*/};
}

#include <torch/script.h>
float angularVel;
bool GoToBallRL::perform(){

    std::vector<float> observation = getState();
    std::vector<torch::jit::IValue> inputs;

    //torch::Tensor t_obs = torch::Tensor({})
    torch::Tensor t_observation = torch::tensor(observation, torch::dtype(torch::kFloat)).to(torch::kCUDA);
    inputs.push_back(t_observation);

    qInfo()<<"{angle_2ball_s, angle_2ball_c, dist_to_ball, velocity.x, velocity.y, v_theta}";
    for(auto el : observation)
        qInfo() <<el;
    //auto action =  test.
    //at::Tensor output = test.forward(inputs).toTensor();
    at::Tensor actions =test.forward(inputs).toTensor();
    std::cout << "\n**********\nActions"<<actions<<"\n***************"<<std::endl;
    return false;

//                std::vector<float> observation = getState();
//                printState(observation);
//                action = agent.Act(observation, true /*adds noise if training*/);
//                for(int i = 0; i <100; i++)
//                    agent.update();
//                action = agent.Act(observation, true /*adds noise if training*/);
//                qInfo() << "Number cycles: " <<step_num++;

//                return false;

//    if(train){
//        return Train();
//    }
//    else
//        return Test();
}

bool GoToBallRL::almost_out_of_bounds(){
        if(/*dist_to_top_field*/(Field::FIELD_WIDTH/2 - robot->y) < 300) return true;
        if(/*dist_to_right_field*/ (Field::FIELD_LENGTH/2 - robot->x) <300)return true;
        if(/*dist_to_bot_field*/ (robot->y - (-Field::FIELD_WIDTH/2)) <300)return true;
        if(/*dist_to_left_field*/ (robot->x - (-Field::FIELD_LENGTH/2))<300)return true;
return false;
};
bool GoToBallRL::Train(){

  if(step_num ==0){qInfo() <<"\n\n\n\nEpisode number: "<<cur_ep;    if(cur_ep<burn_in_time) qInfo()<<"Burning in";              else qInfo()<<"Training";
            time_start = std::chrono::high_resolution_clock::now();     time_last = time_start;
        }

        bool burning_in = cur_ep<burn_in_time;

        std::vector<float> observation = getState();  //printState(observation);

        if(episode_status.episode_complete)
        {
                    if(episode_status.train_complete)
                    {       endTraining();
                            return false;
                    }
                    if(cur_ep%save_frequency==0){
                        agent.save("actor_angle2test"+std::to_string(cur_ep), "critic_angle2test"+std::to_string(cur_ep));
                        agent.saveBuffer("replay_buffer_2test");
                    }
//                    if(cur_ep==burn_in_time-1){
//                        agent.saveBuffer(); agent.printBuffer();}

                episode_status.episode_complete = false; cur_ep++; step_num = 0;
                total_reward = 0;   done = false; burning_in = cur_ep<burn_in_time;
                robot->setTargetVelocityLocal(Point(0,0),0);
                return false;
       }
        else if (step_num != 0)
        {   //Algorithm updates after each step, other examples step the simulation, this is realtime, need to update on consecutive cycle. Cant do on step 0.
           float reward = getReward(/*distance moved to ball*/observation[3/*4*/]);
           total_reward += reward;
           done = robot->hasBall(); // add comparison
           //if(cur_ep%5 == 0)            std::cout << action[0]<<","<<action[1]<<"\t"<<action[2]<<std::endl;
           if(burning_in && (cur_ep)%2 == 0)                       //Need to get burned in actions on the next cycle because of the way the Go-To command works
                {action = getBurnInAction(); action = {/*action[0],*/ action[2]};}

           episode_status =  agent.step(prev_observation, action, reward, observation, done);  //step stores to replay buffer and updates networks. returns whether episode is complete

           if(almost_out_of_bounds()){episode_status.episode_complete=true; agent.resetEpisode();   qInfo() <<"Robot too close to out of bounds";}
        }

        if(episode_status.episode_complete){
            time_stop = std::chrono::high_resolution_clock::now();  qInfo() << "Episode duration(seconds): " << std::chrono::duration_cast<std::chrono::milliseconds>(time_stop - time_start).count()/1000.0;       time_start = time_stop;
            qInfo() << "episode complete, reset training environment(move ball and robot) override and release the robot: ";
            robot->clearCmd(); robot->setTargetVelocityLocal(Point(0,0),0);
            robot->setUseOverridenControls(1);
            return done;
        }

        //Get action from ddpg_agent actor_critic network. or burn in via random actions or hardcoded behaviors. Burn in does not need to step
        if(burning_in && cur_ep%2 == 0)
                BurnIn();
        else{
            action = agent.Act(observation, true /*adds noise if training*/);
            Step(); //Step just sets robot commands by the action

            //Action space
            Point vel(/*action[0]*/0,0);                    //max speeds defined in omni pilot. No function to get max velocity
            float angularVel = action[0];           // for different robots? Is there a robocup limitation?

            robot->setTargetVelocityLocal(vel*max_speed, angularVel*max_ang_speed);
        }

        prev_observation = observation; step_num++;

        return done;         //Normally we would update here(simulator examples), but we must wait for the next cycle

}
bool GoToBallRL::Test(){
    if(step_num ==0){
              time_start = std::chrono::high_resolution_clock::now();
              time_last = time_start;
          }
          auto time_now = std::chrono::high_resolution_clock::now();
          auto cycle_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_last);
          time_last = time_now;
          //if(step_num % 50 == 0)qInfo() << "Cycle num: "<< step_num << " Dur(ms): "<<cycle_dur.count();

          std::vector<float> observation = getState();
          printState(observation);

          if(episode_status.episode_complete)
          {
                  episode_status.episode_complete = false; cur_ep++;
                  step_num = 0;   total_reward = 0;   done = false;
         }
          else if (step_num != 0)
          {   //Algorithm updates after each step, in other examples they are able to step the simulation, but here we must perform the update
              //steps on the next call to perform.. cant do on step 0 since timestep needs to pass.calculate reward and determine if done
             float reward = getReward((/*distance to ball*/observation[3/*4*/]));
             total_reward += reward;
             done = robot->hasBall(); // add comparison

             //Store state or reward history for stats????
             episode_status.episode_complete = done;
          }

          if(episode_status.episode_complete)
                  return done;

          //Get action from ddpg_agent actor_critic network.
          action = agent.Act(observation, false /*adds noise if training*/);
          Step();   //Step just sets robot commands by the action


          prev_observation = observation; step_num++;

          //Normally we would update here(simulator examples), but we must wait for the next cycle

          /*return false;*/ //if it hit this point, then it was not complete
          return done; // was false
}

void GoToBallRL::Step(){
    //Action space
    Point vel(action[0], action[1]);                    //max speeds defined in omni pilot. No function to get max velocity
    float angularVel = action[2];           // for different robots? Is there a robocup limitation?

    robot->setTargetVelocityLocal(vel*max_speed, angularVel*max_ang_speed);
}


float random_float(float low, float high){return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(high- low)));};

void GoToBallRL::BurnIn(){
    cmd.velocity_multiplier =1;
//    if(cur_ep>random_burn_in_time)
    auto angle_ball =fabs(Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation()));
//        if(angle_ball<.07)
//            cmd.setTarget(*ball, Measurements::angleBetween(*robot,*ball));
//        else
            cmd.setTarget(*robot, Measurements::angleBetween(*robot,*ball));
//    else{
//        float rand_ang =  random_float(-3.14, 3.14);
//        float rand_x =  random_float((-Field::FIELD_LENGTH/2.0), (Field::FIELD_LENGTH/2.0));
//        float rand_y =  random_float((-Field::FIELD_WIDTH/2.0), Field::FIELD_WIDTH/2.0);

//        cmd.setTarget(Point(rand_x, rand_y), rand_ang); /**///qInfo() << "("<<rand_x<<" , "<<rand_y<<") - "<<rand_ang;
////    }

//    cmd.avoid_ball = cmd.avoid_obstacles = true; // true
    robot->goToPose(cmd);
//    robot->setTargetVelocityLocal(Point(0,0),-max_ang_speed);
}

std::vector<float> GoToBallRL::getBurnInAction(){
    Point vel = robot->getTargetVelocity();
    float ang_speed = robot->getTargetAngularSpeed() / (80/180.0*3.14);

//    qInfo() <<"Velocity: "<<robot->getTargetVelocity() <<" \t" <<robot->getTargetAngularSpeed()
//           <<"\nafter convert Vel: "<<vel <<" \t"<<ang_speed;
    return std::vector<float>({vel.x/max_speed,vel.y/max_speed,ang_speed/max_ang_speed});
}


void GoToBallRL::endTraining(){
    qInfo() <<"Max number episodes trained(hit enter to save)";
    std::string dummy;
    std::cin >>dummy;
    string time_stamp=getTimeStamp();
    agent.save("actor1"+time_stamp,"critic1"+time_stamp);
}

void GoToBallRL::savePrompt(){
qInfo() <<  "Do you want to save? if so hit enter **debug menu annoyance sorry** then enter the name and hit enter\nif not enter N instead of a name";
    std::string name;  std::cin >>name;
    if(name.length() == 1 && name[0] == 'N')    return;

    string info = "_Epnum-"+std::to_string(cur_ep);
qInfo() <<  "Do you want to use a time stamp(same process)\nif not enter N instead of a name";
    if(name.length() == 1 && name[0] == 'N'){
        agent.save(name+"actor"+info,name+"critic"+info);
        return;
    }
    string time_stamp=getTimeStamp();
    agent.save(name+"actor"+info+time_stamp,name+"critic"+info+time_stamp);
}


float GoToBallRL::getReward(float d){
   float reward = 0;    //rtotal = rcontact + rdistance + rorientation

   if(robot->hasBall()) //contact...perhaps change to robot has ball on dribbler like in paper?
       reward+= 100.f;
                                                        //qInfo()<<"Reward done: "<<reward << d;
   //distance
//   reward += ( (5.f/sqrt(2.f*M_PI))*exp(-(d*d)/2.f) )- 2.f;                 //qInfo()<<"Reward dist: "<<reward;
   //orientation
   float theta_r_b  = fabs(Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation()));
   reward += static_cast<float>( (1.f/sqrt(2.f*M_PI))*exp(-2.f*(theta_r_b/(M_PI*M_PI))) );          //qInfo()<<"Reward angle: "<<reward;


   return reward;
}

void GoToBallRL::printState(std::vector<float> observation){
//        qInfo() << "#-------------------------------Observation"
//                <<"\nBall position relative" <<observation[0] <<", " <<observation[1] <<"\trobot angle: "<< robot->getOrientation()
//                <<"\nRobot velocity" << observation[2] <<", "<< observation[3] << "\ndist_to_ball" << observation[4]
////                <<"\nDist to field\n Top:" <<observation[5] << "\tright:" <<observation[6] <<"\tbottom: " <<observation[7] << "\tleft: " <<observation[8]
//                <<"#-------------------------------";

            qInfo() << "#-------------------------------Observation"
                    <<"\nangletoball" <<observation[0]
                    <<"\nRobot velocity" << observation[1] <<", "<< observation[2] << "\ndist_to_ball" << observation[3]
                    <<"#-------------------------------";
}

void GoToBallRL::printAction(){
        qInfo() << "#-------------------------------ActionVelocity (" <<action[0]*max_speed<<", " << action[1]*max_speed    <<")\tAngular_vel:"
                <<action[2]*max_ang_speed  <<"\n("<<action[0]<<", " << action[1] <<  ")\t" <<action[2]  <<"#-------------------------------";
}


bool GoToBallRL::isFinished(){
   return done;
}

string GoToBallRL::getName(){
    return "Go to ball RL";
}


#include <regex>
string getTimeStamp(){
std::time_t ts_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
string time_stamp = string(ctime(&ts_t));
std::regex r(" |\n");   time_stamp = std::regex_replace(time_stamp, r, "_");
r= (":");      time_stamp = std::regex_replace(time_stamp, r, "-");

return time_stamp;
}
