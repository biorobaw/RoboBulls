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
#include <iostream>

class ReplayBuffer
{

public:
    ReplayBuffer(): circular_queue(max_buff_size){
    //circular_queue.reserve(max_buff_size);
    };
    ReplayBuffer(bool load): circular_queue(max_buff_size){
    if(load)    loadReplayBuffer("replaybuffer");
    };
    void Store(torch::Tensor observation, torch::Tensor action,
               torch::Tensor reward,torch::Tensor next_observation, torch::Tensor done);
//    std::vector<Experience> Sample(int num_samples) {
//        std::vector<Experience> samples;
//        for (int i = 0; i < num_samples; i++)
//            samples.push_back(Sample());
//        return samples;
//    }
    Experience Sample(int num_samples);
    Experience Sample() {   return circular_queue[rand() % std::min(max_buff_size, size)].first; }

    Experience NonRandomSample(int num_samples);
    std::pair<Experience, int> NonRandomSampleWithId(int sample_id) {   return circular_queue[sample_id% std::min(max_buff_size, size)]; }


    std::pair<Experience, int> SampleWithId() {   return circular_queue[rand() % std::min(max_buff_size, size)]; }

    size_t getLength() const{    return std::min(max_buff_size, size);   }

    Experience getBufferTensorTuple() const;

    void saveReplayBuffer(std::string file_name = "replaybuffer") const;
    void loadReplayBuffer(std::string file_name = "replaybuffer");
    friend std::ostream& operator<<(std::ostream& os, const ReplayBuffer& rb);


private:
    int max_buff_size = 500000/*1000000*/;
    int size = 0;
    std::vector<std::pair<Experience,int>> circular_queue;
};

#endif // REPLAYBUFFER_H
