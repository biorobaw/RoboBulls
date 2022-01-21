#include "ddpg.h"

#include <iostream>
/*
 *NEEEDED VARIABLES????
 * int BUFFER_SIZE = int(1e5);  // replay buffer size
int BATCH_SIZE = 128;        // minibatch size
double GAMMA = 0.99;            // discount factor
double TAU = 1e-3;              // for soft update of target parameters
double LR_ACTOR = 1e-4;         // learning rate of the actor
double LR_CRITIC = 1e-3;        // learning rate of the critic
double WEIGHT_DECAY = 0;        // L2 weight decay

int Agent::totalNumberOfAgents = 0;

DDPG_Agent::DDPG_Agent(int o_size, int a_size, int num_epoch_steps, int total_epochs, int ep_len,
                       int num_eps,
                       int update_after_numsteps, int update_every_numsteps, int batchsize,
                                                     float poly):
      actor(o_size, a_size),
      critic(o_size, a_size),
      actor_targ(o_size, a_size),
      critic_targ(o_size, a_size),
      actor_optimizer(actor->parameters(), 1e-4),
      critic_optimizer(critic->parameters(), 1e-3),
      obs_size(o_size),
      action_size(a_size),
      update_after(update_after_numsteps),
      update_every(update_every_numsteps),
      batch_size(batchsize),
      steps_per_epoch(num_epoch_steps),
      num_epochs(total_epochs),
      polyak(poly),
      max_ep_len(ep_len),
      num_episodes(num_eps)
*/
double stddev = 0.1;
const float gamma = 0.99;
const float actor_learn_rate = 5e-5;
const float critic_learn_rate = 1e-3; //1e-3;
//const float polyak = 0.995;

//bool load_replay_buffer = false;

DDPG_Agent::DDPG_Agent(int o_size, int a_size, int maximum_ep_len, int num_eps, int update_after_steps,
                                            int update_every_numsteps, int batchsize):
      device( (torch::cuda::is_available() ? torch::kCUDA : torch::kCPU) ),
      actor(Actor(o_size, a_size)),
      critic(Critic(o_size, a_size)),
      actor_targ(Actor(o_size, a_size)),
      critic_targ(Critic(o_size, a_size)),
      //actor_optimizer(actor->parameters(), 1e-4),
      //critic_optimizer(critic->parameters(), 1e-3),
      replay_buffer(/*load_replay_buffer*/),
      obs_size(o_size),
      action_size(a_size),
      max_ep_len(maximum_ep_len),
      update_after(update_after_steps),
      update_every(update_every_numsteps),
      batch_size(batchsize),
      num_episodes(num_eps),
      num_updates(0),
//      polyak(poly),
      dist(0, stddev)

{
    std::cout <<"CUDA device count : " << torch::cuda::device_count()
             << std::endl;
    if(device.type() == torch::kCUDA)
        std::cout << "DDPG_Agent using GPU"<< std::endl;
    else
        std::cout << "DDPG_Agent using CPU"<< std::endl;

    actor->to(device);  actor_targ->to(device);
    critic->to(device); critic_targ->to(device);
    torch::Device temp_device(torch::kCPU);
    //critic_optimizer.options.weight_decay_ = WEIGHT_DECAY;

    cur_step = 0;
    ep_len = 0;
    cur_episode=0;

    actor_optimizer = new torch::optim::Adam(actor->parameters(), actor_learn_rate);
    critic_optimizer = new torch::optim::Adam(critic->parameters(), critic_learn_rate);

    //Copy weights
    {torch::NoGradGuard no_grad;
    auto actor_targ_p = actor_targ->parameters();       auto actor_p = actor->parameters();
    for (size_t i = 0; i < actor_targ_p.size(); i++)
            actor_targ_p[i] = actor_p[i];}

    {torch::NoGradGuard no_grad;
    auto  critic_targ_p = critic_targ->parameters();    auto critic_p = critic->parameters();
    for (int i = 0; i < critic_targ_p.size(); i++)
            critic_targ_p[i] = critic_p[i]; }


}
std::vector<float> DDPG_Agent::random_action(){
torch::Tensor action = torch::randn(action_size,torch::dtype(torch::kFloat)).to(torch::kCPU);
std::vector<float> actions(action.data_ptr<float>(), action.data_ptr<float>() + action.numel());
for (size_t i =0; i < actions.size(); i++)
    actions[i] = std::fmin(std::fmax(actions[i] , -1.f), 1.f);
return actions;
}


std::vector<float> DDPG_Agent::Act(std::vector<float> observation, bool add_noise){
        /********************/if(cur_step < 1000) return random_action();/********************/

    torch::Tensor t_observation = torch::tensor(observation, torch::dtype(torch::kFloat)).to(device);
    actor->eval();

    torch::NoGradGuard g;
    torch::Tensor action = (actor->forward(t_observation) ).to(torch::kCPU); //Move to cpu to put into c++ vector.
    if(cur_step%50==0)std::cout <<"action(before noise): "<<action<<std::endl;
                            //action = action.to(torch::kCPU);
    actor->train();
    std::vector<float> actions(action.data_ptr<float>(), action.data_ptr<float>() + action.numel());

    if (add_noise)
        for(auto &a : actions)
            a+= dist(generator) * (1 - (cur_episode/num_episodes)); // Reduce noise based on episodes

    for (size_t i =0; i < actions.size(); i++)
        actions[i] = std::fmin(std::fmax(actions[i] , -1.f), 1.f);
return actions;

}

#include <QDebug>
#include <chrono>
EpisodeStatus DDPG_Agent::step(std::vector<float> observation, std::vector<float> action, float reward, std::vector<float> next_observation, bool done)
{
    torch::Tensor t_obs = torch::tensor(observation, torch::dtype(torch::kFloat)).to(device);
    torch::Tensor t_action = torch::tensor(action, torch::dtype(torch::kFloat)).to(device);
    torch::Tensor t_reward = torch::tensor({reward}, torch::dtype(torch::kFloat)).to(device);
    torch::Tensor t_next_obs = torch::tensor(next_observation, torch::dtype(torch::kFloat)).to(device);
    torch::Tensor t_d = torch::tensor({done}, torch::dtype(torch::kFloat)).to(device);

    replay_buffer.Store(t_obs, t_action, t_reward, t_next_obs, t_d);

    // If ready to update.  **Note**: Update every might need to be 1.
    if (cur_step >= update_after && cur_step % update_every == 0 && replay_buffer.getLength()>batch_size)
    {            //auto total_strt = std::chrono::high_resolution_clock::now();
        for(int i = 0; i<update_every; i++)
            update();

                //auto total_end = std::chrono::high_resolution_clock::now();
                //auto up_len_tot = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_strt);
                //qInfo() << "Time for "<<update_every << " updates batch size "<<batch_size <<" dur(ms): "<<up_len_tot.count();
    }

    cur_step++; ep_len++;

    if(ep_len==max_ep_len || done){
        resetEpisode();
        if(cur_episode >= num_episodes)
                return EpisodeStatus(true, true);
        return EpisodeStatus(true, false);
    }

    return EpisodeStatus(false, false);
}

void DDPG_Agent::resetEpisode(){
    std::cout << /*replay_buffer*/"eplen: "<<ep_len<<std::endl;
    ep_len = 0;
    cur_episode++;
}

void DDPG_Agent::update(){//batch_size = 32;
    auto [obs, actions, rewards, next_obs, done] = replay_buffer.Sample(batch_size); //Sampling takes ~5 ms with batch size 25


//            if(num_updates<200){
//            auto actions_actor = actor->forward(obs);
//            auto Pi_loss = torch::mse_loss(actions_actor, actions.detach());
//            actor_optimizer->zero_grad();
//            Pi_loss.backward();
//            actor_optimizer->step();

//            auto actor_targ_p = actor_targ->parameters();   auto actor_p = actor->parameters();
//            {   torch::NoGradGuard no_grad;
//                for (int i = 0; i < actor_targ_p.size(); i++){
//                    actor_targ_p[i].mul_(polyak);
//                    actor_targ_p[i].add_((1 - polyak) * actor_p[i]);
//                }
//            }

//            num_updates++;
//            return;


//            }






    //Compute loss for Q(critic) and run gradient descent
            auto Q = critic->forward(obs, actions);
            torch::Tensor bell_backup;
            //{torch::NoGradGuard no_grad;
            auto actions_from_target = actor_targ->forward(next_obs);
            auto Q_pi_targ = critic_targ->forward(next_obs, actions_from_target);
            bell_backup = rewards + (gamma * (1 - done) * Q_pi_targ );//}
            auto Q_loss = torch::mse_loss(Q, bell_backup.detach());

//                                   << Q <<"\nQ_pi_targ: " <<Q_pi_targ << std::endl;
    //auto Q_loss = getQLoss(prev_s, actions, rewards, s, done);
    critic_optimizer->zero_grad();
    Q_loss.backward();
    critic_optimizer->step();

    //Compute loss for p(actor) and run gradient ascent (~18 ms)
        auto action_pi = actor->forward(obs);
        auto Q_pi = critic->forward(obs, action_pi);
        auto Pi_loss = -Q_pi.mean();
    //auto Pi_loss = getPiLoss(obs);

//                    std::cout <<"action_pi: " <<action_pi <<"\nQ_pi: \n"<<Q_pi<<"\nLOSS:\n"<<Pi_loss   <<std::endl;

    actor_optimizer->zero_grad();
    Pi_loss.backward();
    actor_optimizer->step();

    //Update Target Networks with polyak averaging. (~5-10ms) i.e. Target = polyak*Target + (1-polyak)*non_target
    auto critic_targ_p = critic_targ->parameters(); auto critic_p = critic->parameters();
    {    torch::NoGradGuard no_grad;
        for (int i = 0; i < critic_targ_p.size(); i++){
            critic_targ_p[i].mul_(polyak);
            critic_targ_p[i].add_((1 - polyak) * (critic_p[i]));
        }
    }
    auto actor_targ_p = actor_targ->parameters();   auto actor_p = actor->parameters();
    {   torch::NoGradGuard no_grad;
        for (int i = 0; i < actor_targ_p.size(); i++){
            actor_targ_p[i].mul_(polyak);
            actor_targ_p[i].add_((1 - polyak) * actor_p[i]);
        }
    }

    num_updates++;

    if(num_updates%100 == 0)
        std::cout <<"\nAction and obs: " <<torch::cat({actions, obs},1)<<"\nQ critic criticizing action and rewards: "<< torch::cat({Q,actions,rewards},1) << "\nbellbackuo:"<<bell_backup<<"\nQ_pi_targ: " <<Q_pi_targ
                     <<"\nQloss: "<<Q_loss <<std::endl<<"\nactionfrompi:"<<action_pi<<"\nQ(expected from action): " <<"\nPI_ LOSS:\n"<<Pi_loss <<std::endl;


    return;
}

torch::Tensor DDPG_Agent::getQLoss(torch::Tensor &observations, torch::Tensor &actions,
                    torch::Tensor &rewards, torch::Tensor &next_observations, torch::Tensor &done){
    torch::NoGradGuard no_grad;
    auto Q = critic->forward(observations, actions);
    auto actions_from_target = actor_targ->forward(next_observations);
    auto Q_pi_targ = critic_targ->forward(next_observations, actions_from_target);
    auto bell_backup = rewards + (gamma * (1 - done) * Q_pi_targ);

    return torch::mse_loss(Q, bell_backup.detach());
}

torch::Tensor DDPG_Agent::getPiLoss(torch::Tensor &observations){
    auto action_pi = actor->forward(observations);
    auto Q_pi = critic->forward(observations, action_pi);
    return -Q_pi.mean();
}


bool DDPG_Agent::startEpisode(){
    return ep_len==0;
}

bool DDPG_Agent::endEpisode(){
    if(ep_len<max_ep_len)
        return false;
    ep_len=0;
    return true;
}

bool DDPG_Agent::endTraining(){
    return cur_episode == num_episodes;
}

void DDPG_Agent::save(std::string a_file_name, std::string c_file_name){
    auto actor_file ("C:\\Users\\justi\\Documents\\ThesisRL\\actor\\" + a_file_name + ".pt");
    auto critic_file("C:\\Users\\justi\\Documents\\ThesisRL\\critic\\" + c_file_name + ".pt");
    torch::save(actor , actor_file);
    torch::save(critic , critic_file);
}

void DDPG_Agent::load(std::string a_file_name, std::string c_file_name){
    auto actor_file ("C:\\Users\\justi\\Documents\\ThesisRL\\actor\\" + a_file_name + ".pt");
    auto critic_file("C:\\Users\\justi\\Documents\\ThesisRL\\critic\\"+ c_file_name + ".pt");
    torch::load(actor, actor_file);
    torch::load(critic, critic_file);
}

void DDPG_Agent::printBuffer(){     if(replay_buffer.getLength()>0)     std::cout <<replay_buffer <<"\n num updates: "<<num_updates<<std::endl;   }

void DDPG_Agent::saveBuffer(std::string file_name){     if(replay_buffer.getLength()>0)      replay_buffer.saveReplayBuffer(file_name);  }
void DDPG_Agent::loadBuffer(std::string file_name){     replay_buffer.loadReplayBuffer(file_name);  }



DDPG_Agent::~DDPG_Agent(){
    delete actor_optimizer;
    delete critic_optimizer;
}
