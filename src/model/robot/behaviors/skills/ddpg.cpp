#include "ddpg.h"

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
*/

double stddev = 0.1;

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

{

    //critic_optimizer.options.weight_decay_ = WEIGHT_DECAY;
    cur_step = 0;
    ep_len = 0;
    cur_episode=0;

    std::normal_distribution<double> dist(0, stddev);

    //Copy weights
    for (size_t i = 0; i < actor_targ->parameters().size(); i++) {
            actor_targ->parameters()[i] = actor->parameters()[i];
        }

    for (size_t i = 0; i < critic_targ->parameters().size(); i++) {
            critic_targ->parameters()[i] = critic->parameters()[i];
        }

//   actor->to(device);
//   actor_targ->to(device);
//   critic->to(device);
//   critic_targ->to(device);

}


std::vector<float> DDPG_Agent::Act(std::vector<float> state, bool add_noise){
    torch::Tensor t_state = torch::tensor(state, torch::dtype(torch::kFloat));
    actor->eval();

    torch::NoGradGuard g;
    torch::Tensor action = actor->forward(t_state);
    actor->train();
    std::vector<float> actions(action.data_ptr<float>(), action.data_ptr<float>() + action.numel());
    if (add_noise)
        for(auto a : actions)
            a+= dist(generator); // add time value?

    for (size_t i =0; i < actions.size(); i++)
        actions[i] = std::fmin(std::fmax(actions[i] , (float) -1), (float) 1);
return actions;

}


bool DDPG_Agent::step(std::vector<float> prev_state, std::vector<float> action, float reward, std::vector<float> state, bool done)
{
    torch::Tensor t_prev_s = torch::tensor(prev_state, torch::dtype(torch::kFloat));
    torch::Tensor t_action = torch::tensor(action, torch::dtype(torch::kFloat));
    torch::Tensor t_reward = torch::tensor(reward, torch::dtype(torch::kFloat));
    torch::Tensor t_s = torch::tensor(state, torch::dtype(torch::kFloat));
    torch::Tensor t_d = torch::tensor(done, torch::dtype(torch::kFloat));

    replay_buffer.Store(t_prev_s, t_action, t_reward, t_s, t_d);
    // If ready to update
    if (cur_step >= update_after && cur_step % update_every == 0)
    {
        for(int i = 0; i<update_every; i++)
            update();
    }

    cur_step++; ep_len++;
    if(ep_len==max_ep_len || done){
        ep_len = 0;
        cur_episode++;
        return true;
    }
    return false;
}

void DDPG_Agent::update(){
    //std::vector<Experience> instances = replay_buffer.Sample(batch_size);
    auto [prev_s, actions, rewards, s, done] = replay_buffer.Sample(32);

            //Compute loss for Q(critic) and run gradient descent
    auto Q_loss = getQLoss(prev_s, actions, rewards, s, done);
    critic_optimizer.zero_grad();
    Q_loss.backward();
    critic_optimizer.step();

    //Compute lose for p(actor) and run gradient descent
    auto Pi_loss = getPiLoss(prev_s, s);
    actor_optimizer.zero_grad();
    Pi_loss.backward();
    actor_optimizer.step();


    //Update Target Networks with polyak averaging.
//        # Finally, update target networks by polyak averaging.
//               with torch.no_grad():
//                   for p, p_targ in zip(ac.parameters(), ac_targ.parameters()):
//                       # NB: We use an in-place operations "mul_", "add_" to update target
//                       # params, as opposed to "mul" and "add", which would make new tensors.
//                       p_targ.data.mul_(polyak)
//                       p_targ.data.add_((1 - polyak) * p.data)



            /*Need to adjust for handling tensors??*/
            torch::NoGradGuard no_grad;
            for (int i = 0; i < critic_targ->parameters().size(); i++)
                critic_targ->parameters()[i].copy_(critic->parameters()[i] * polyak + critic_targ->parameters()[i] * (1.0 - polyak));

            //torch::NoGradGuard no_grad;
            for (int i = 0; i < actor_targ->parameters().size(); i++)
                actor_targ->parameters()[i].copy_(actor->parameters()[i] * polyak + critic_targ->parameters()[i] * (1.0 - polyak));

    return;
}

torch::Tensor DDPG_Agent::getQLoss(torch::Tensor &prev_states, torch::Tensor &actions,
                    torch::Tensor &rewards, torch::Tensor &states, torch::Tensor &done){
    int gamma = 0; //change
    auto Q = critic->forward(prev_states, actions);
    auto actions_from_target = actor_targ->forward(states);
    auto Q_pi_targ = critic_targ->forward(states, actions_from_target);
    auto bell_backup = rewards + (gamma * Q_pi_targ * (1 - done));

    return torch::mse_loss(Q, bell_backup.detach());
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



torch::Tensor DDPG_Agent::getPiLoss(torch::Tensor &prev_states, torch::Tensor &states){
    auto action_pi = actor->forward(prev_states);
    auto Q_pi = critic->forward(states, action_pi);
    return -Q_pi.mean();
}


void DDPG_Agent::save(std::string a, std::string c){
    auto actor_file ("C:\\Users\\justi\\Documents\\RL\\actor_agent\\" + a + ".pt");
    auto critic_file("C:\\Users\\justi\\Documents\RL\\critic_agent\\"+ c + ".pt");
    torch::save(actor , actor_file);
    torch::save(critic , critic_file);
}

void DDPG_Agent::load(std::string a, std::string c){
    auto actor_file ("C:\\Users\\justi\\Documents\\RL\\actor_agent\\" + a + ".pt");
    auto critic_file("C:\\Users\\justi\\Documents\\RL\\critic_agent\\"+ c + ".pt");
    torch::load(actor, actor_file);
    torch::load(critic, critic_file);
}
