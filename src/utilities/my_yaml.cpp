#include "my_yaml.h"


QDebug operator<<(QDebug debug, const YAML::Node& node){
    QDebugStateSaver saver(debug);
    debug.nospace() << node.Scalar().c_str();
    return debug;
}
