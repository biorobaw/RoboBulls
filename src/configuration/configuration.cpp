#include "configuration.h"

Configuration* Configuration::instance=nullptr;

Configuration::Configuration(std::string folder)
{
    configs["comm"]   = YAML::LoadFile(folder + "/comm.yaml"  );
    configs["teams"]  = YAML::LoadFile(folder + "/team.yaml"  );
    configs["field"]  = YAML::LoadFile(folder + "/field.yaml" );
    configs["motion"] = YAML::LoadFile(folder + "/motion.yaml");

    instance= this;
}


Configuration* Configuration::get(){
    return instance;
}

YAML::Node& Configuration::operator[](QString config_folder){
    return configs[config_folder];
}
