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
 * Justin Rodney
 */


class ActorImpl : public torch::nn::Module{
public:
    ActorImpl(int64_t state_size, int64_t action_size, int64_t seed = 0, int64_t fc1_units=400, int64_t fc2_units=300);

    torch::Tensor forward(torch::Tensor x_s);
    std::pair<double,double> hidden_init(torch::nn::Linear& layer);

    void reset_parameters();



private:
    torch::nn::Linear fully_con1{nullptr}, fully_con2{nullptr}, fully_con3{nullptr};
};
TORCH_MODULE(Actor);

class CriticImpl : public torch::nn::Module{
public:
    CriticImpl(int64_t state_size, int64_t action_size, int64_t seed = 0, int64_t fcs1_units=400, int64_t fc2_units=300);

    torch::Tensor forward(torch::Tensor x_s, torch::Tensor x_a);
    std::pair<double,double> hidden_init(torch::nn::Linear& layer);

    void reset_parameters();

private:
    torch::nn::Linear fully_con1{nullptr}, fully_con2{nullptr}, fully_con3{nullptr};
};

TORCH_MODULE(Critic);

#endif // ACTORCRITIC_H
