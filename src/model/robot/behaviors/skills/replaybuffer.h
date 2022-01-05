#ifndef REPLAYBUFFER_H
#define REPLAYBUFFER_H

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

using Experience = std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor>;
/*
 * Referenced:
 * https://spinningup.openai.com/en/latest/_modules/spinup/algos/pytorch/ddpg/ddpg.html
 * https://spinningup.openai.com/en/latest/algorithms/ddpg.html (for algorithm and pseudocode)
 * & https://github.com/EmmiOcean/DDPG_LibTorch (for C++ implementation)
 * & https://github.com/i-m-iron-man/libtorch_mujoco_DDPG/blob/main/model.hpp (for multi batches)
 * Justin Rodney
 */
class ReplayBuffer
{

public:
    ReplayBuffer(){
    circular_queue.reserve(max_buff_size);
    };
    void Store(torch::Tensor prev_observation, torch::Tensor action,
               torch::Tensor reward,torch::Tensor observation, torch::Tensor done){
//        if(index_to_enter < max_buff_size)
//        circular_queue.push_back(std::make_tuple(prev_observation, action, reward, observation, done))
//                ;
        circular_queue[size%max_buff_size] = std::make_tuple(prev_observation, action, reward, observation, done);
        size++;

    };
//    std::vector<Experience> Sample(int num_samples) {
//        std::vector<Experience> samples;
//        for (int i = 0; i < num_samples; i++)
//            samples.push_back(Sample());
//        return samples;
//    }
    Experience Sample(int num_samples) {
        Experience samples = Sample();
        for (int i = 1; i < num_samples; i++){
            Experience new_sample = Sample();
            std::get<0>(samples) = torch::cat({std::get<0>(samples), std::get<0>(new_sample)}, 0);
            std::get<1>(samples) = torch::cat({std::get<1>(samples), std::get<1>(new_sample)}, 0);
            std::get<2>(samples) = torch::cat({std::get<2>(samples), std::get<2>(new_sample)}, 0);
            std::get<3>(samples) = torch::cat({std::get<3>(samples), std::get<3>(new_sample)}, 0);
            std::get<4>(samples) = torch::cat({std::get<4>(samples), std::get<4>(new_sample)}, 0);
        }
        return samples;
    }

    Experience Sample() {
            //return circular_queue.at(static_cast<size_t>(rand() % static_cast<int>(circular_queue.size())));
        return circular_queue[rand() % std::min(max_buff_size, size)];
    }

    size_t getLength() {
        return std::min(max_buff_size, size);
        //return circular_queue.size();
    }

private:
    int max_buff_size = 1000000;
    int size = 0;
    std::vector<Experience> circular_queue;
};

#endif // REPLAYBUFFER_H
