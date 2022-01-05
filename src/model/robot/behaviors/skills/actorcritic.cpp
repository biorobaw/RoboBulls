#include "actorcritic.h"

ActorImpl::ActorImpl(int64_t state_size, int64_t action_size, int64_t seed,
             int64_t fc1_units, int64_t fc2_units) : torch::nn::Module(){
    torch::manual_seed(seed);
    fully_con1 = register_module("fully_con1", torch::nn::Linear(state_size, fc1_units));
    fully_con2 = register_module("fully_con2", torch::nn::Linear(fc1_units, fc2_units));
    fully_con3 = register_module("fully_con3", torch::nn::Linear(fc2_units, action_size));
    reset_parameters();
}

void ActorImpl::reset_parameters(){
    std::pair<double,double> fully_con1_init = hidden_init(fully_con1);
    torch::nn::init::uniform_(fully_con1->weight, fully_con1_init.first, fully_con1_init.second);

    std::pair<double,double> fully_con2_init = hidden_init(fully_con2);
    torch::nn::init::uniform_(fully_con2->weight, fully_con2_init.first, fully_con2_init.second);

    torch::nn::init::uniform_(fully_con3->weight, -3e-3, 3e-3);
}

std::pair<double,double> ActorImpl::hidden_init(torch::nn::Linear& layer) {
    double lim = 1.0 / sqrt(layer->weight.sizes()[0]);
    return std::make_pair(-lim, lim);
}

torch::Tensor ActorImpl::forward(torch::Tensor x_s){
    x_s = torch::relu(fully_con1->forward(x_s));
    x_s = torch::relu(fully_con2->forward(x_s));
    x_s = fully_con3->forward(x_s);

    return torch::tanh(x_s);

}

CriticImpl::CriticImpl(int64_t state_size, int64_t action_size, int64_t seed,
               int64_t fcs1_units, int64_t fc2_units) : torch::nn::Module(){
    torch::manual_seed(seed);
    fully_con1 = register_module("fully_con1", torch::nn::Linear(state_size, fcs1_units));
    fully_con2 = register_module("fully_con2", torch::nn::Linear(fcs1_units + action_size, fc2_units));
    fully_con3 = register_module("fully_con3", torch::nn::Linear(fc2_units, 1));
    reset_parameters();
}

std::pair<double,double> CriticImpl::hidden_init(torch::nn::Linear& layer) {
    double lim = 1.0 / sqrt(layer->weight.sizes()[0]);
    return std::make_pair(-lim, lim);
}

void CriticImpl::reset_parameters(){
    std::pair<double,double> fully_con1_init = hidden_init(fully_con1);
    torch::nn::init::uniform_(fully_con1->weight, fully_con1_init.first, fully_con1_init.second);

    std::pair<double,double> fully_con2_init = hidden_init(fully_con2);
    torch::nn::init::uniform_(fully_con2->weight, fully_con2_init.first, fully_con2_init.second);

    torch::nn::init::uniform_(fully_con3->weight, -3e-3, 3e-3);
}

torch::Tensor CriticImpl::forward(torch::Tensor x_s, torch::Tensor x_a){
    if (x_s.dim() == 1)
        x_s = torch::unsqueeze(x_s, 0);

    if (x_a.dim() == 1)
        x_a = torch::unsqueeze(x_a,0);

    auto xs = torch::relu(fully_con1->forward(x_s));
    x_s = torch::cat({xs,x_a}, 1);
    x_s = torch::relu(fully_con2->forward(x_s));

    return fully_con3->forward(x_s);
}


