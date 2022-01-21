#ifndef DDPG_H
#define DDPG_H

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include "actorcritic.h"
#include "utilities/point.h"
#include <random>
#include<torch/script.h>
                        //Episode done, training done.

struct EpisodeStatus{
    EpisodeStatus(bool ep, bool train): episode_complete(ep), train_complete(train){}
    EpisodeStatus():EpisodeStatus(false, false){}
    bool episode_complete = false;
    bool train_complete = false;
};
/*
 * Referenced:
 * https://spinningup.openai.com/en/latest/_modules/spinup/algos/pytorch/ddpg/ddpg.html
 * https://spinningup.openai.com/en/latest/algorithms/ddpg.html (for algorithm and pseudocode)
 * & https://github.com/EmmiOcean/DDPG_LibTorch(for C++ implementation)
 * Justin Rodney
 */
//You will most likely want(NEED) to train in release mode. Debug mode greatly increases the time, and can eat up cycles.

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

//Batch size too high or too long wait between updates results in updating take too long to train in realtime
//**Note**: Update every might need to be every 1 cycle. Even on GPU, a single update takes ~30 ms, which is the time for a cycle.
//As per the OpenAI reference, each number of steps to number updates is a 1:1 ratio, so it should not have a significant change. Updating every 50 cycles still results in 50 updates
// on the 50th cycle, but doing multiple samples/update in a single cycle takes too much time to operate in real time currently.
//**********YOU WILL WANT TO TRAIN ON RELEASE MODE, DEBUG MODE RESULTS IN CYCLE TIME DOUBLING****************************
    DDPG_Agent(int o_size, int a_size, int maximum_ep_len = 350, int num_eps = 1500, int update_after_steps = (350*10),
                                          int update_every_numsteps = 1/*50*/, int batchsize = 64/*100*/);

    ~DDPG_Agent();

    /*!
     *Returns a set of actions
     */
    std::vector<float> Act(std::vector<float> observation, bool add_noise = false);

    /*!
     *To be called each step to change states/observations and rewards and store experience/interaction in replay buffer.
     *Also updates the network when it is ready.
     *returns whether max steps reached(episode is finished).
     */
    //bool step(std::vector<float> state, std::vector<float> action, float reward, std::vector<float> next_state, bool done);
    EpisodeStatus step(std::vector<float> observation, std::vector<float> action, float reward, std::vector<float> next_observation, bool done);

    /*!
     * Called from step. Updates the network
     */
    void update();

    void save(std::string a_file_name, std::string c_file_name);
    void load(std::string a_file_name, std::string c_file_name);

    torch::Tensor getQLoss(torch::Tensor &observations, torch::Tensor &actions,
                        torch::Tensor &rewards, torch::Tensor &next_observations, torch::Tensor &done);
    torch::Tensor getPiLoss(torch::Tensor &observations);


    void DDPG_Agent::resetEpisode();

    /*!
     * Following functions return whether:
     *  the episode has just started and theEnvironment(robot, ball, ...) should be reset,
     */
    bool startEpisode();
    /*! whether the episode should end (stop acting, and reset episode) */
    bool endEpisode();
    /*! and whether the training should end entirely. */
    bool endTraining();




    /*! makes a call to store/load the experiences in the Replay Buffer as PyTorch tensor files.*/
    void saveBuffer(std::string file_name = "replaybuffer");
    void loadBuffer(std::string file_name = "replaybuffer");
    void printBuffer();



private:
        //actor
    std::vector<float> random_action();

    torch::Device device;

    Actor actor;
    Critic critic;
    Actor actor_targ;
    Critic critic_targ;

    torch::optim::Adam *actor_optimizer;
    torch::optim::Adam *critic_optimizer;

    ReplayBuffer replay_buffer;
    int obs_size, action_size, cur_step, max_ep_len, update_after,
    update_every, batch_size, steps_per_epoch, num_epochs, ep_len,
    num_episodes, cur_episode, num_updates;

    float polyak = 0.995;

    std::default_random_engine generator;
    std::normal_distribution<double> dist;


};


#endif // DDPG_H
