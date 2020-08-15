#ifndef MYYAML_H
#define MYYAML_H

#include <QDebug>
#include "yaml-cpp/yaml.h"

QDebug operator<<(QDebug debug, const YAML::Node& node);


#endif // MYYAML_H
