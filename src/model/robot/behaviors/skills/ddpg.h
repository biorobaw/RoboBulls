#ifndef DDPG_H
#define DDPG_H

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include "actorcritic.h"
#include "utilities/point.h"
#include <random>

/*
 * Referenced:
 * https://spinningup.openai.com/en/latest/_modules/spinup/algos/pytorch/ddpg/ddpg.html
 * https://spinningup.openai.com/en/latest/algorithms/ddpg.html (for algorithm and pseudocode)
 * & https://github.com/EmmiOcean/DDPG_LibTorch(for C++ implementation)
 * Justin Rodney
 */
class DDPG_Agent
{
public:
    /*!
     * \brief DDPG_Agent
     * \param o_size Size of observations vector
     * \param a_size Size of actions vector
     * \param update_after_numsteps Number of interactions/steps in the environment to sample before updating the network
     * \param update_every_numsteps Number of interactions/steps between updates.
     * \param batchsize Batchsize for gradient descent.
     * \param steps_per_epoch Number of steps for each epoch.
     */
    DDPG_Agent(int o_size, int a_size, int steps_per_epoch = 4000, int num_epochs = 5,
               int ep_len = 1000, int num_eps = 5, int update_after_numsteps = 1000,
               int update_every_numsteps = 50, int batchsize = 32,  float poly = 0.995);

    /*!
     *Returns a set of actions
     */
    std::vector<float> Act(std::vector<float> state, bool add_noise = false);

    /*!
     *To be called each step to change states/observations and rewards and store experience/interaction in replay buffer.
     *Also updates the network when it is ready.
     *returns whether max steps reached(episode is finished).
     */
    bool step(std::vector<float> prev_state, std::vector<float> action, float reward, std::vector<float> state, bool done);

    /*!
     * Called from step. Updates the network
     */
    void update();

    void save(std::string a, std::string c);
    void load(std::string a, std::string c);

    torch::Tensor getQLoss(torch::Tensor &prev_states, torch::Tensor &actions,
                        torch::Tensor &rewards, torch::Tensor &states, torch::Tensor &done);
    torch::Tensor getPiLoss(torch::Tensor &prev_states, torch::Tensor &states);


    /*!
     * Following functions return whether:
     *  the episode has just started and theEnvironment(robot, ball, ...) should be reset,
     */
    bool startEpisode();
    /*! whether the episode should end (stop acting, and reset episode) */
    bool endEpisode();
    /*! and whether the training should end entirely. */
    bool endTraining();



private:
        //actor
    Actor actor;
    Critic critic;
    Actor actor_targ;
    Critic critic_targ;

    torch::optim::Adam actor_optimizer;
    torch::optim::Adam critic_optimizer;

    ReplayBuffer replay_buffer;
    int obs_size, action_size, cur_step, update_after,
    update_every, batch_size, steps_per_epoch, num_epochs, ep_len, max_ep_len,
    num_episodes, cur_episode;

    float polyak;

    std::default_random_engine generator;
    std::normal_distribution<double> dist;

};


#endif // DDPG_H
