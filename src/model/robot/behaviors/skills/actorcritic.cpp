#include "actorcritic.h"
#include <iostream>

ActorImpl::ActorImpl(int64_t state_size, int64_t action_size, int64_t seed,
             int64_t a_layer1_units, int64_t a_layer2_units) : torch::nn::Module(){
    torch::manual_seed(seed);
    a_layer1 = register_module("a_layer1", torch::nn::Linear(state_size, a_layer1_units));
    a_layer2 = register_module("a_layer2", torch::nn::Linear(a_layer1_units, a_layer2_units));
    a_layer3 = register_module("a_layer3", torch::nn::Linear(a_layer2_units, action_size));
    reset_parameters();
    //auto batch_norm = register_module("batch_norm", torch::nn::BatchNorm2d(torch::nn::BatchNormOptions(256)));

    //torch::nn::init::uniform_(a_layer3->weight, -3e-3, 3e-3);

}

void ActorImpl::reset_parameters(){
    std::pair<double,double> a_layer1_init = hidden_init(a_layer1);
    torch::nn::init::uniform_(a_layer1->weight, a_layer1_init.first, a_layer1_init.second);

    std::pair<double,double> a_layer2_init = hidden_init(a_layer2);
    torch::nn::init::uniform_(a_layer2->weight, a_layer2_init.first, a_layer2_init.second);

    torch::nn::init::uniform_(a_layer3->weight, -3e-3, 3e-3);
}

std::pair<double,double> ActorImpl::hidden_init(torch::nn::Linear& layer) {
    double lim = 1.0 / sqrt(layer->weight.sizes()[0]);
    return std::make_pair(-lim, lim);
}

torch::Tensor ActorImpl::forward(torch::Tensor x_s){
//    if (x_s.dim() == 1)
//        x_s = torch::unsqueeze(x_s, 0);
            //std::cout << "X_s input: " << x_s<<std::endl;
    x_s = torch::relu(a_layer1->forward(x_s));
    x_s = torch::relu(a_layer2->forward(x_s));
    x_s = a_layer3->forward(x_s);

            //std::cout << "fully_con3: " << x_s<<std::endl;
            x_s = torch::tanh(x_s);
            //std::cout << "tanh: " << x_s<<std::endl;
            return x_s;
    return torch::tanh(x_s);

}

CriticImpl::CriticImpl(int64_t state_size, int64_t action_size, int64_t seed, int64_t state_c_layer1_units,
  int64_t state_c_layer2_units,int64_t action_c_layer1_units,int64_t c_layer1_units, int64_t c_layer2_units)
    : torch::nn::Module(){
    torch::manual_seed(seed);
//    state_c_layer1 = register_module("state_c_layer1", torch::nn::Linear(state_size, state_c_layer1_units));
//    state_c_layer2 = register_module("state_c_layer2", torch::nn::Linear(state_c_layer1_units, state_c_layer2_units));
//    action_c_layer1 = register_module("action_c_layer1", torch::nn::Linear(action_size, action_c_layer1_units));
//    c_layer1 = register_module("c_layer1", torch::nn::Linear(state_c_layer2_units + action_c_layer1_units /*state_size*/, c_layer1_units));
//    c_layer2 = register_module("c_layer2", torch::nn::Linear(c_layer1_units /*+ action_size*/, c_layer2_units));
    c_layer1 = register_module("c_layer1", torch::nn::Linear(state_size , c_layer1_units));
    c_layer2 = register_module("c_layer2", torch::nn::Linear(c_layer1_units + action_size, c_layer2_units));

    c_layer3 = register_module("c_layer3", torch::nn::Linear(c_layer2_units, 1));
    reset_parameters();
}

std::pair<double,double> CriticImpl::hidden_init(torch::nn::Linear& layer) {
    double lim = 1.0 / sqrt(layer->weight.sizes()[0]);
    return std::make_pair(-lim, lim);
}

void CriticImpl::reset_parameters(){
//    std::pair<double,double> state_c_layer1_init = hidden_init(state_c_layer1);
//    torch::nn::init::uniform_(state_c_layer1->weight, state_c_layer1_init.first, state_c_layer1_init.second);

//    std::pair<double,double> state_c_layer2_init = hidden_init(state_c_layer2);
//    torch::nn::init::uniform_(state_c_layer2->weight, state_c_layer2_init.first, state_c_layer2_init.second);

//    std::pair<double,double> action_c_layer1_init = hidden_init(action_c_layer1);
//    torch::nn::init::uniform_(action_c_layer1->weight, action_c_layer1_init.first, action_c_layer1_init.second);

    std::pair<double,double> c_layer1_init = hidden_init(c_layer1);
    torch::nn::init::uniform_(c_layer1->weight, c_layer1_init.first, c_layer1_init.second);

    std::pair<double,double> c_layer2_init = hidden_init(c_layer2);
    torch::nn::init::uniform_(c_layer2->weight, c_layer2_init.first, c_layer2_init.second);

    torch::nn::init::uniform_(c_layer3->weight, -3e-3, 3e-3);
}
#include <iostream>
torch::Tensor CriticImpl::forward(torch::Tensor x_s, torch::Tensor x_a){
    if (x_s.dim() == 1)
        x_s = torch::unsqueeze(x_s, 0);

    if (x_a.dim() == 1)
        x_a = torch::unsqueeze(x_a,0);
    //fully_con1->pretty_print(std::cout);
    //std::cout <<"\n"<< x_s << "\nDIms: "<<x_s.sizes()<<std::endl;
//    x_s = torch::relu(state_c_layer1(x_s));
//    auto xs = torch::relu(state_c_layer2(x_s));

//    auto xa = torch::relu(action_c_layer1(x_a));

//    x_s = torch::cat({xs,xa}, 1);

//    x_s = torch::relu(c_layer1->forward(x_s));
//    x_s = torch::relu(c_layer2->forward(x_s));
//    x_s = c_layer3->forward(x_s);

//    return x_s;
//    return torch::tanh(x_s);

    auto xs = torch::relu(c_layer1->forward(x_s));
    x_s = torch::cat({xs,x_a}, 1);
    x_s = torch::relu(c_layer2->forward(x_s));
    x_s = c_layer3->forward(x_s);
    return x_s;
    //return torch::tanh(x_s);
}


