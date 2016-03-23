#include "getclassname.h"

//**** Attempting to get the class name so that it can be added to the UI to show Robots current behavior


template<typename T>

const std::string& getClassName(const T& variable)
{
    static std::string empty;

    try
    {
    #ifdef __GNUC__
        //On GNUC, we need to demangle the type names to be human-readable. See:
        //https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
        //This is a map to hold results, to store already-processed names.
        static std::unordered_map<std::type_index, std::string> nameMemory;

        //Get type info
        const std::type_index& info = typeid(variable);

        //Memory lookup. If not there, ask the compiler
        if(nameMemory.find(info) == nameMemory.end()) {
            int status;
            char* name = abi::__cxa_demangle(info.name(), 0, 0, &status);
            nameMemory[info] = std::string(name);
            free(name);
        }

        return nameMemory[info];
    #else
        return typeid(type).name();	//If not GNUC, we just assume correct name returned
    #endif
    }
    catch(...) {
        return empty;
    }

    return empty;
}


