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
 *Utilizing a similar state action space as prupose in: https://zhuyifengzju.github.io/files/2018Robocup.pdf
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
#include <regex>
string getTimeStamp(){
std::time_t ts_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
string time_stamp = string(ctime(&ts_t));
std::regex r(" |\n");   time_stamp = std::regex_replace(time_stamp, r, "_");
r= (":");      time_stamp = std::regex_replace(time_stamp, r, "-");

return time_stamp;
}

int max_episode_length= 350;
int num_episodes = 100/*500*/ /*1500*/;
int burn_in_time = 3;
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_stop, time_last;
/************************************************************************/


//DDPG_Agent(int o_size, int a_size, int update_after_numsteps = 1000, int update_every_numsteps = 50,
//int batchsize = 32, int steps_per_epoch = 4000);
GoToBallRL::GoToBallRL(Robot* robot,
                                 float targetTolerance, float kickDistance, bool useFullPower)
    : Behavior(robot)
    , m_moveCompletionCount(0)
    , agent(/*num observations*/9,/*num actions*/ 3, max_episode_length, num_episodes)
//    , m_targetTolerance((targetTolerance < 0) ? STRICTEST_ANG_TOL : targetTolerance)
//    , m_kickDistance(kickDistance)
//    , m_kickLockCount(0)
//    , m_hasRecoveredKickLock(true)
//    , m_useFullPower(useFullPower)
//    , m_lastBallAwayDist(0)
//    , m_ballMovingAwayCount(0)
//    , m_kickCommandCount(0)
//    , m_hasKicked(false)

{
    train = true;
    episode_status.episode_complete = false; episode_status.train_complete = false;
    done = false;
    //qInfo() <<"New kick to point omni robot "<<robot->getId();
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);
    time_start = std::chrono::high_resolution_clock::now();
    time_last = time_start;
}




float angularVel;




bool GoToBallRL::perform(){
    if(train)
        return Train();
    else
        return Test();


}



bool GoToBallRL::Train(){
  if(step_num ==0){
            time_start = std::chrono::high_resolution_clock::now();
            time_last = time_start;
        }
        auto time_now = std::chrono::high_resolution_clock::now();
        auto cycle_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_last);
        time_last = time_now;
        //if(step_num % 50 == 0)qInfo() << "Cycle num: "<< step_num << " Dur(ms): "<<cycle_dur.count();
        bool burning_in = cur_ep<burn_in_time;

        std::vector<float> observation = getState();
        //printState(observation);

        if(episode_status.episode_complete)
        {
                    qInfo() << robot->getTargetAngularSpeed() <<" "<<robot->getTargetVelocity();
                    time_stop = std::chrono::high_resolution_clock::now();  qInfo() << "Episode duration(seconds): " << std::chrono::duration_cast<std::chrono::milliseconds>(time_stop - time_start).count()/1000.0;       time_start = time_stop;
                    if(episode_status.train_complete)
                    {       endTraining();
                            return false;
                    }

                episode_status.episode_complete = false; cur_ep++;
                step_num = 0;   total_reward = 0;   done = false;
       }
        else if (step_num != 0)
        {   //Algorithm updates after each step, in other examples they are able to step the simulation, but here we must perform the update
            //steps on the next call to perform.. cant do on step 0 since timestep needs to pass.calculate reward and determine if done
           float reward = getReward(dist_to_ball);
           total_reward += reward;
           done = robot->hasBall(); // add comparison

           if(burning_in)                       //This is a really weird way around the fact pilot commands override setting the  robot velocity
               action = getBurnInAction();      //mannualy and are executed AFTERWARDS, need to get burned in action on NEXT cycle

           //step stores replay buffer and updates networks. returns if episode is complete. test just checks if episode is over
           episode_status =  agent.step(prev_observation, action, reward, observation, done);
        }

        if(episode_status.episode_complete){
            qInfo() << "episode complete, reset training environment(move ball and robot) and release the robot: ";
            robot->clearCmd();//
            robot->setUseOverridenControls(1);
            return done;
        }

        //Get action from ddpg_agent actor_critic network. or burn in via random actions or hardcoded behaviors. Burn in does not need to step
        if(burning_in)
                BurnIn();
        else{
            action = agent.Act(observation, true /*adds noise if training*/);
            Step(); //Step just sets robot commands by the action
        }

        prev_observation = observation; step_num++;

        //Normally we would update here(simulator examples), but we must wait for the next cycle

        //DDPG
        /*return false;*/ //if it hit this point, then it was not complete
        return done; // was false


    return true;
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
             float reward = getReward(dist_to_ball);
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


      return true;
}

std::vector<float> GoToBallRL::getState(){
    //State input
    Point ball_pos = (*ball - *robot).rotate(-(robot->getOrientation())); //relative to robot
    Point velocity = robot->getVelocity().rotate(-(robot->getOrientation())); //translational
    float dist_to_ball = Measurements::distance(*ball, *robot);
    float dist_to_top_field = Field::FIELD_WIDTH/2 - robot->y;
    float dist_to_right_field = Field::FIELD_LENGTH/2 - robot->x;
    float dist_to_bot_field =  robot->y - (-Field::FIELD_WIDTH/2);
    float dist_to_left_field = robot->x - (-Field::FIELD_LENGTH/2);
    return {ball_pos.x, ball_pos.y, velocity.x, velocity.y, dist_to_ball, dist_to_top_field, dist_to_right_field, dist_to_bot_field, dist_to_left_field};
}

float max_speed = 1500;     float max_ang_speed = 80/180.0*3.14;
void GoToBallRL::Step(){
    //Action space
    Point vel(action[0], action[1]);                    //max speeds defined in omni pilot. No function to get max velocity
    float angularVel = action[2];           // for different robots? Is there a robocup limitation?

//    qInfo() << "speed: " <<(vel*max_speed) <<" angular: "<<(angularVel*max_ang_speed) << " Max ang: "<<max_ang_speed;
    robot->setTargetVelocityLocal(vel*max_speed, angularVel*max_ang_speed);
}

//float max_ang =0;

float random_float(float low, float high){
    return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(high- low)));
};
void GoToBallRL::BurnIn(){
    cmd.velocity_multiplier =1;
    if(cur_ep%2 == 0)
        cmd.setTarget(*ball, Measurements::angleBetween(*robot,*ball));
    else{
        float rand_ang =  random_float(-3.14, 3.14);
        float rand_x =  random_float((-Field::FIELD_LENGTH/2.0), (Field::FIELD_LENGTH/2.0));
        float rand_y =  random_float((-Field::FIELD_WIDTH/2.0), Field::FIELD_WIDTH/2.0);

        cmd.setTarget(Point(rand_x, rand_y), rand_ang);
    }

    //cmd.avoid_ball = cmd.avoid_obstacles = true; // true
    robot->goToPose(cmd);
}
    //robot->pilo->executeCommands();

std::vector<float> GoToBallRL::getBurnInAction(){
    Point vel = robot->getTargetVelocity();
    float ang_speed = robot->getTargetAngularSpeed() / (80/180.0*3.14);
                ///**/max_ang= (max_ang<robot->getTargetAngularSpeed())? robot->getTargetAngularSpeed(): max_ang;/**/ /**/qInfo() << "Max ang: " <<max_ang;/**/
    return std::vector<float>({vel.x/max_speed,vel.y/max_speed,ang_speed/max_ang_speed});
}

void GoToBallRL::resetEnvironment(){
    qInfo() << "episode complete, reset training environment(move ball and robot)(hit enter, then any key then enter to continue):\n**I'm a grad student with no time to add this to debug console I'm sorry**";
    std::string dummy;
    std::cin >>dummy;
}

void GoToBallRL::endTraining(){
    qInfo() <<"Max number episodes trained(hit enter to save)";
    std::string dummy;
    std::cin >>dummy;
    string time_stamp=getTimeStamp();
    agent.save("actor1"+time_stamp,"critic1"+time_stamp);
}
float GoToBallRL::getReward(float d){
   float reward = 0;

   //rtotal = rcontact + rdistance + rorientation

   if(robot->hasBall()) //contact...perhaps change to robot has ball on dribbler like in paper?
       reward+=100;

   //distance
   reward +=( (5/sqrt(2*M_PI))*exp(-(d*d)/2) )- 2;
   //orientation
   float theta_r_b  = fabs(Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation()));
   reward += (1/sqrt(2*M_PI))*exp(-2*(theta_r_b/(M_PI*M_PI)));


   return reward;
    // return m_hasKicked; // TODO: Actually this will always return false, needs to be fixed
}

void GoToBallRL::printState(std::vector<float> observation){
        qInfo() << "#-------------------------------Observation"
                <<"\nBall position relative" <<observation[0] <<", " <<observation[1] <<"\trobot angle: "
                << robot->getOrientation()<<"\n"
                <<"Robot velocity" << observation[2] <<", "<< observation[3] << "\n"
               << "dist_to_ball" << observation[4]
               <<"Dist to field\n Top:" <<observation[5] << "\tright:" <<observation[6] <<"\tbottom: " <<observation[7] << "\tleft: " <<observation[8]
              <<"#-------------------------------";

}
void GoToBallRL::printAction(){
        qInfo() << "#-------------------------------ActionVelocity (" <<action[0]*max_speed<<", " << action[1]*max_speed    <<")\tAngular_vel:"
                <<action[2]*max_ang_speed  <<"\n("<<action[0]<<", " << action[1] <<  ")\t" <<action[2]  <<"#-------------------------------";
}


bool GoToBallRL::isFinished(){
   return true;
    // return m_hasKicked; // TODO: Actually this will always return false, needs to be fixed
}
string GoToBallRL::getName(){
    return "Kick 2 point omni";
}
//********************************************
//bool GoToBallRL::perform()
//{   if(step_num ==0){
//        time_start = std::chrono::high_resolution_clock::now();
//        time_last = time_start;
//    }
//    auto time_now = std::chrono::high_resolution_clock::now();
//    auto cycle_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_last);
//    time_last = time_now;
//    //if(step_num % 50 == 0)qInfo() << "Cycle num: "<< step_num << " Dur(ms): "<<cycle_dur.count();

//    std::vector<float> observation = getState();
////    qInfo() << "Ball position relative" <<observation[0] <<", " <<observation[1] <<" robot angle: "
////            << robot->getOrientation()<<"\n"
////            <<"Robot velocity" << observation[2] <<", "<< observation[3] << "\n"
////           << "dist_to_ball" << observation[4]
////           <<"Dist to field\n Top:" <<observation[5] << " right:" <<observation[6] <<" bottom: " <<observation[7] << " left: " <<observation[8];

////    std::string dummy;
////    std::cin >>dummy;
////    return false;
//    if(episode_status.episode_complete)
//    {
//            if(train)
//            {qInfo() << robot->getTargetAngularSpeed() <<" "<<robot->getTargetVelocity();
//                    time_stop = std::chrono::high_resolution_clock::now();
//                    qInfo() << "Episode duration(seconds): " << std::chrono::duration_cast<std::chrono::milliseconds>(time_stop - time_start).count()/1000.0;
//                    time_start = time_stop;
//                    if(episode_status.train_complete)
//                    {       endTraining();
//                            return false;
//                    }
//                    /*resetEnvironment();*/

//            }
//            episode_status.episode_complete = false; cur_ep++;
//            step_num = 0;   total_reward = 0;   done = false;
//   }
//    else if (step_num != 0)
//    {   //Algorithm updates after each step, in other examples they are able to step the simulation, but here we must perform the update
//        //steps on the next call to perform.. cant do on step 0 since timestep needs to pass.calculate reward and determine if done
//       float reward = getReward(dist_to_ball);
//       total_reward += reward;
//       done = robot->hasBall(); // add comparison

//       //**if training** step stores replay buffer and updates networks. returns if episode is complete. test just checks if episode is over
//       episode_status = (train) ? /*training*/ agent.step(prev_observation, action, reward, observation, done)
//                                  : /*testing*/ EpisodeStatus(done || agent.endEpisode(), false);
//    }

//    if(episode_status.episode_complete){
//            if(train){ qInfo() << "episode complete, reset training environment(move ball and robot) and release the robot: ";
//                robot->setUseOverridenControls(1);
//            }
//            return done;
//    }

//    //Get action from ddpg_agent actor_critic network.
//    if(train && cur_ep<burn_in_time)
//            action = BurnIn();
//    else{
//        action = agent.Act(observation, train /*adds noise if training*/);

//        //Action space
//        Point vel(action[0], action[1]);        float max_speed = 1500;             //max speeds defined in omni pilot. No function to get max velocity
//        float angularVel = action[2];           float max_ang_speed = 80/180.0*3.14;// for different robots? Is there a robocup limitation?

//        qInfo() << "speed: " <<(vel*max_speed) <<" angular: "<<(angularVel*max_ang_speed);
//        robot->setTargetVelocityLocal(vel*max_speed, angularVel*max_ang_speed);
//    }



//    prev_observation = observation; step_num++;

//    //Normally we would update here(simulator examples), but we must wait for the next cycle

//    //DDPG
//    /*return false;*/ //if it hit this point, then it was not complete
//    return done; // was false

//}

//*********************************************








//bool GoToBallRL::perform()
//{
//    std::vector<float> observation = getState();

//    if(episode_complete){
//        if(train){
//            resetEnvironment();
//            if(agent.endTraining()){
//                qInfo() <<"Max number episodes trained";
//                return false;
//            }
//        }
//        episode_complete = false;
//        step_num = 0;
//        total_reward = 0;
//        done = false;

//    }
//    else if (step_num != 0){
//        //Algorithm updates after each step, in other examples they are
//        //able to step the simulation, but here we must perform the update
//        //steps on the next call to perform.. cant do on step 0 since timestep needs to pass.
//        //calculate reward and determine if done
//       float reward = getReward(dist_to_ball);
//       total_reward += reward;
//       done = robot->hasBall(); // add comparison

//       //update stores replay buffer and performs updating to the networks.
//       //returns whether episode is complete
//       if(train)
//            episode_complete = agent.step(prev_observation, action, reward,observation, done);
//       else
//           episode_complete = done || agent.endEpisode();

//    }
//    if(episode_complete)
//            return done;

//    //Get action from ddpg_agent actor_critic network.
//    action = agent.Act(observation, train /*adds noise if training*/);

//    //Action space
//    Point vel(action[0], action[1]);
//    float angularVel = action[2];

//    robot->setTargetVelocityLocal(vel, angularVel);
//    prev_observation = observation; step_num++;

//    //Normally we would update here(simulator examples), but we must wait for the next cycle

//    //DDPG
//    return done; // was false
//}
