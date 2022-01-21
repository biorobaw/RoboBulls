#ifndef ACTORCRITIC_H
#define ACTORCRITIC_H

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include "replaybuffer.h"
/*
 * Referenced:
 * https://spinningup.openai.com/en/latest/_modules/spinup/algos/pytorch/ddpg/ddpg.html
 * https://spinningup.openai.com/en/latest/algorithms/ddpg.html (for algorithm and pseudocode)
 * & https://github.com/EmmiOcean/DDPG_LibTorch(for C++ implementation)
 * & https://pytorch.org/tutorials/advanced/cpp_frontend.html(C++)
 * + another good example for libtorch RL networks:https://github.com/i-m-iron-man/libtorch_mujoco_DDPG/blob/main/model.hpp
 * For ideas look at MLPActorCritic here:  https://genrl.readthedocs.io/en/latest/api/core/index.html
 *
 * https://github.com/openai/spinningup/blob/master/spinup/algos/pytorch/ppo/core.py
 * https://keras.io/examples/rl/ddpg_pendulum/
 * Justin Rodney
 */


class ActorImpl : public torch::nn::Module{
public:
    ActorImpl(int64_t state_size, int64_t action_size, int64_t seed = 0, int64_t a_layer1_units=256/*10*//*128*/ /*256*//*400*/, int64_t a_layer2_units=256 /*10*//*64*//*256*//*300*/);

    torch::Tensor forward(torch::Tensor x_s);
    std::pair<double,double> hidden_init(torch::nn::Linear& layer);

    void reset_parameters();

private:
    torch::nn::Linear a_layer1{nullptr}, a_layer2{nullptr}, a_layer3{nullptr};
    //torch::batch_norm()
};
TORCH_MODULE(Actor);

class CriticImpl : public torch::nn::Module{
public:
    CriticImpl(int64_t state_size, int64_t action_size, int64_t seed = 0, int64_t state_c_layer1_units=16,
               int64_t state_c_layer2_units=32,int64_t action_c_layer1_units=32,int64_t c_layer1_units=256,
                                                        int64_t c_layer2_units= /*10*//*64*/ /*256*/ 256);

    torch::Tensor forward(torch::Tensor x_s, torch::Tensor x_a);
    std::pair<double,double> hidden_init(torch::nn::Linear& layer);

    void reset_parameters();

private:
    torch::nn::Linear state_c_layer1{nullptr},state_c_layer2{nullptr},action_c_layer1{nullptr},  c_layer1{nullptr},
                                                    c_layer2{nullptr}, c_layer3{nullptr};
};

TORCH_MODULE(Critic);

#endif // ACTORCRITIC_H
