#include "getbehavior.h"
#include "mainwindow.h"
#include "model/robot.h"
#include "ui_mainwindow.h"

#ifdef __GNUC__
 #include <cxxabi.h>
 #include <unordered_map>
#endif

#include <typeindex>


GetBehavior::GetBehavior(MainWindow * mw) {
    dash = mw;

}

void GetBehavior::setupBehaviors() {

}

const std::string& GetBehavior::getBehaviorName(Robot* robot)
{
    static std::string noBehavior = "No Behavior";

    if(robot == NULL or not(robot->hasBeh)) {
        return noBehavior;
    }

    try
    {
#ifdef __GNUC__
    //https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
    //Map to hold names, because demangle mallocs a string, inefficient.
    static std::unordered_map<std::type_index, std::string> nameMemory;

    //Get type info
    const std::type_index& info = typeid(*(robot->getCurrentBeh()));

    //Memory lookup?
    if(nameMemory.find(info) == nameMemory.end()) {
        int status;
        char* name = abi::__cxa_demangle(info.name(), 0, 0, &status);
        nameMemory[info] = std::string(name);
        free(name);
    }

    return nameMemory[info];
#else
    return typeid(*(robot->getCurrentBeh())).name();
#endif
    }
    catch(...) {
        return noBehavior;
    }
}

void GetBehavior::printBehavior() {

}
