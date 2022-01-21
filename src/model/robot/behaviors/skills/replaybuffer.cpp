#include "replaybuffer.h"

//ReplayBuffer::ReplayBuffer(){
//    circular_queue.reserve(max_buff_size);
//};
#include <iostream>

void ReplayBuffer::Store(torch::Tensor observation, torch::Tensor action,
           torch::Tensor reward,torch::Tensor next_observation, torch::Tensor done){
    circular_queue[size%max_buff_size] = std::make_pair(std::make_tuple(observation, action, reward, next_observation, done), size);
    size++;

}

Experience ReplayBuffer::Sample(int num_samples) {
    std::vector<torch::Tensor> obs, actions, rewards, next_obs, dones;
    for(int i = 0; i<num_samples; i++){
        auto [new_sample, sample_id] = SampleWithId();
        obs.push_back(std::get<0>(new_sample).unsqueeze(0));
        actions.push_back(std::get<1>(new_sample).unsqueeze(0));
        rewards.push_back(std::get<2>(new_sample).unsqueeze(0));
        next_obs.push_back(std::get<3>(new_sample).unsqueeze(0));
        dones.push_back(std::get<4>(new_sample).unsqueeze(0));
    }
    torch::Tensor t_obs = torch::cat({obs});
    torch::Tensor t_actions = torch::cat({actions});
    torch::Tensor t_rewards = torch::cat({rewards});
    torch::Tensor t_next_obs = torch::cat({next_obs});
    torch::Tensor t_dones = torch::cat({dones});
   return std::make_tuple(t_obs, t_actions, t_rewards, t_next_obs, t_dones);
}

Experience ReplayBuffer::NonRandomSample(int num_samples) {
    std::vector<torch::Tensor> obs, actions, rewards, next_obs, dones;
    for(int i = 0; i<num_samples; i++){
        auto [new_sample, sample_id] = NonRandomSampleWithId(i);
        obs.push_back(std::get<0>(new_sample).unsqueeze(0));
        actions.push_back(std::get<1>(new_sample).unsqueeze(0));
        rewards.push_back(std::get<2>(new_sample).unsqueeze(0));
        next_obs.push_back(std::get<3>(new_sample).unsqueeze(0));
        dones.push_back(std::get<4>(new_sample).unsqueeze(0));
    }
    torch::Tensor t_obs = torch::cat({obs});
    torch::Tensor t_actions = torch::cat({actions});
    torch::Tensor t_rewards = torch::cat({rewards});
    torch::Tensor t_next_obs = torch::cat({next_obs});
    torch::Tensor t_dones = torch::cat({dones});
   return std::make_tuple(t_obs, t_actions, t_rewards, t_next_obs, t_dones);
}
//Experience ReplayBuffer::Sample(int num_samples) {
//    auto [new_sample, sample_id] = SampleWithId();
//    Experience samples = std::make_tuple(std::get<0>(new_sample).unsqueeze(0), std::get<1>(new_sample).unsqueeze(0),
//                                         std::get<2>(new_sample).unsqueeze(0),std::get<3>(new_sample).unsqueeze(0), std::get<4>(new_sample).unsqueeze(0));
//    for (int i = 1; i < num_samples; i++){
//        auto [new_sample, sample_id] = SampleWithId();
//        std::get<0>(samples) = torch::cat({std::get<0>(samples), std::get<0>(new_sample).unsqueeze(0)}, 0);
//        std::get<1>(samples) = torch::cat({std::get<1>(samples), std::get<1>(new_sample).unsqueeze(0)}, 0);
//        std::get<2>(samples) = torch::cat({std::get<2>(samples), std::get<2>(new_sample).unsqueeze(0)}, 0);
//        std::get<3>(samples) = torch::cat({std::get<3>(samples), std::get<3>(new_sample).unsqueeze(0)}, 0);
//        std::get<4>(samples) = torch::cat({std::get<4>(samples), std::get<4>(new_sample).unsqueeze(0)}, 0);
//    }
//    std::cout <<"prev_observation: " <<std::get<0>(samples) <<"\naction:" <<std::get<1>(samples) <<
//    "\nreward: "<<std::get<2>(samples)
//    <<"\nobservation: "<<std::get<3>(samples)<<
//    "\ndone: "<<std::get<4>(samples)<<std::endl;
//    return samples;
//}
    Experience ReplayBuffer::getBufferTensorTuple() const{
        std::vector<torch::Tensor> obs, actions, rewards, next_obs, dones;
        int rb_len = std::min(max_buff_size, size);
        for(int i = 0; i<rb_len; i++){
            auto [o, a, r, next_o, d] = circular_queue[i].first;
            obs.push_back(o.unsqueeze(0));
            actions.push_back(a.unsqueeze(0));
            rewards.push_back(r.unsqueeze(0));
            next_obs.push_back(next_o.unsqueeze(0));
            dones.push_back(d.unsqueeze(0));
        }
        torch::Tensor t_obs = torch::cat({obs});
        torch::Tensor t_actions = torch::cat({actions});
        torch::Tensor t_rewards = torch::cat({rewards});
        torch::Tensor t_next_obs = torch::cat({next_obs});
        torch::Tensor t_dones = torch::cat({dones});
        return std::make_tuple(t_obs, t_actions, t_rewards, t_next_obs, t_dones);

    }



//std::string buff_file_name ="replaybuffer";


void tensorsToCSV(torch::Tensor obs,torch::Tensor actions, torch::Tensor rewards,torch::Tensor next_obs,torch::Tensor dones, std::string file_name);

void ReplayBuffer::saveReplayBuffer(std::string file_name) const{                std::cout << "saving replaybuffer"<<std::endl;

    auto obs_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_obs" + ".pt");
    auto actions_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_actions" + ".pt");
    auto rewards_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_rewards" + ".pt");
    auto next_obs_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_next_obs" + ".pt");
    auto dones_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_dones" + ".pt");

    auto [obs, actions, rewards, next_obs, dones] = getBufferTensorTuple();

    torch::save(obs, obs_file);;
    torch::save (actions, actions_file);
    torch::save (rewards, rewards_file);
    torch::save (next_obs, next_obs_file);
    torch::save (dones, dones_file);

    tensorsToCSV(obs, actions, rewards, next_obs, dones, file_name+"_saved");
}


void ReplayBuffer::loadReplayBuffer(std::string file_name){                std::cout << "Loading replaybuffer: "<<std::endl;

        auto obs_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_obs" + ".pt");
        auto actions_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_actions" + ".pt");
        auto rewards_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_rewards" + ".pt");
        auto next_obs_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_next_obs" + ".pt");
        auto dones_file ("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+"_dones" + ".pt");

        torch::Tensor obs, actions, rewards, next_obs, dones;

        torch::load(obs, obs_file);;
        torch::load(actions, actions_file);
        torch::load(rewards, rewards_file);
        torch::load(next_obs, next_obs_file);
        torch::load(dones, dones_file);
        if( !(obs.dim()==2 && actions.dim()== 2 && rewards.dim()== 2 && next_obs.dim()== 2 && dones.dim()== 2)   ){
            std::cout << "Tensor dimensions incorrect. Cant load"<<std::endl;
            return;
        }
        if(!    (obs.size(0) == actions.size(0) && actions.size(0) == rewards.size(0) && rewards.size(0) == next_obs.size(0) && next_obs.size(0) == dones.size(0)) ){

            std::cout << "Ammount of experiences in each tensor incorrect. Cant load"<<std::endl;
        }
        int num_experiences = obs.size(0);
        for(int i = 0; i < num_experiences; i++){
            //std::cout<< "Device: "<<(obs.device().type() == torch::kCUDA ? "GPU" : "CPU") << std::endl;
            Store(obs[i] ,actions[i],rewards[i],next_obs[i],dones[i]);
        }
        //tensorsToCSV(obs, actions, rewards, next_obs, dones, file_name+"_loaded");
        std::cout << "Loaded buffer: obs: " << (obs.device() == torch::kCUDA ? "GPU":"CPU") << obs.sizes() <<"\tactions: " << (actions.device() == torch::kCUDA ? "GPU":"CPU") << actions.sizes()
                  <<"\trewards: " << (rewards.device() == torch::kCUDA ? "GPU":"CPU") << rewards.sizes() <<"\tnext_obs: " << (next_obs.device() == torch::kCUDA ? "GPU":"CPU") << next_obs.sizes()
                  <<"\tdone: " << (dones.device() == torch::kCUDA ? "GPU":"CPU") << dones.sizes() << std::endl;
}


std::ostream& operator<<(std::ostream& os, const ReplayBuffer& rb){
    Experience tensors = rb.getBufferTensorTuple();
    os << "Previous Observation: "<< std::get<0>(tensors) << "\nAction: " <<std::get<1>(tensors)
       << "\nReward: "<<std::get<2>(tensors) <<"\nObservation: " << std::get<3>(tensors)<< "\nDone: " <<std::get<4>(tensors);


    //for(auto sample : rb.circular_queue){
    //    os << "Previous Observation: "<< std::get<0>(sample.first) << "\nAction: " <<std::get<1>(sample.first) << "\nReward: "<<std::get<2>(sample.first)
    //                              <<"\nObservation: " << std::get<3>(sample.first) << "\nDone: " <<std::get<4>(sample.first) <<"\nID: " << sample.second;
    //if(++i==fmin(rb.size, rb.max_buff_size)) break;
    //}
    return os;
}

void tensorsToCSV(torch::Tensor obs,torch::Tensor actions, torch::Tensor rewards,torch::Tensor next_obs,torch::Tensor dones, std::string file_name)
{
    obs = obs.flatten(1).contiguous().cpu();                float* o_ptr = obs.data_ptr<float>();
    actions = actions.flatten(1).contiguous().cpu();        float* a_ptr = actions.data_ptr<float>();
    rewards = rewards.flatten(1).contiguous().cpu();        float* r_ptr = rewards.data_ptr<float>();
    next_obs = next_obs.flatten(1).contiguous().cpu();      float* next_obs_ptr = next_obs.data_ptr<float>();
    dones = dones.flatten(1).contiguous().cpu();            float* d_ptr = dones.data_ptr<float>();


    std::ofstream csvHandle("C:\\Users\\justi\\Documents\\ThesisRL\\replaybuffer\\" + file_name+".csv");

            csvHandle<< "observation,";             for (size_t k = 0; k < obs.sizes()[1]; ++k)         csvHandle << ',';
            csvHandle<<"actions,";                  for (size_t k = 0; k < actions.sizes()[1]; ++k)     csvHandle << ',';
            csvHandle<<"rewards,";                  for (size_t k = 0; k < rewards.sizes()[1]; ++k)     csvHandle << ',';
            csvHandle <<"next_observation,";        for (size_t k = 0; k < next_obs.sizes()[1]; ++k)    csvHandle << ',';
            csvHandle <<"done,";                    for (size_t k = 0; k < dones.sizes()[1]; ++k)       csvHandle << ',';
            csvHandle<< "\n";

    for (size_t i = 0; i < obs.sizes()[0]; ++i)
    {
        for (size_t k = 0; k < obs.sizes()[1]; ++k)
            csvHandle << *o_ptr++ << ',';
        csvHandle << ',';
        for (size_t k = 0; k < actions.sizes()[1]; ++k)
            csvHandle << *a_ptr++ << ',';
        csvHandle << ',';
        for (size_t k = 0; k < rewards.sizes()[1]; ++k)
            csvHandle << *r_ptr++<< ',';
        csvHandle << ',';
        for (size_t k = 0; k < next_obs.sizes()[1]; ++k)
            csvHandle << *next_obs_ptr++    << ',';

        csvHandle << ',';
        for (size_t k = 0; k < dones.sizes()[1]; ++k)
        {
            csvHandle << *d_ptr++;
            if (k < (dones.sizes()[1] - 1))
                csvHandle << ',';
        }
        csvHandle << "\n";
    }

};
