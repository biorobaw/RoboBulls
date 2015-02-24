#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <cstdlib>

namespace debug
{

//Map of registered variable names to integer pointers
static std::unordered_map<std::string, int*> commandMap;

//Thread for debug module
static std::thread debug_thread;


//Function to split a string based on a delimiter
static std::vector<std::string> stringSplit(const std::string& target, const std::string& delim)
{
    std::vector<std::string> result;
    size_t startPos = 0, it = 0;

    do {
        it = target.find(delim, startPos);
        result.push_back(target.substr(startPos, it - startPos));
        startPos = it + delim.length();
    }
    while(it != std::string::npos);

    return result;
}


static void debugListenFn()
{
    std::string input;

    while(1)
    {
        std::cout << "> ";
        std::getline(std::cin, input); //Blocking

        std::vector<std::string> arguments = stringSplit(input, " ");

        if(arguments.size() == 0)
            continue;

        if(arguments[0] == "set") //Set a variable
        {
            if(arguments.size() == 3) {
                auto it = commandMap.find(arguments[1]);
                if( it != commandMap.end() ) {
                    *(it->second) = std::atoi(arguments[2].c_str());    
                } else {
                    std::cout << "No entry \"" << arguments[1] << "\" exists" << std::endl;
                }
            }
        }
        else if(arguments[0] == "get") 
        {
            //Print only a selected variable
            if(arguments.size() == 2) {                     
                auto it = commandMap.find(arguments[1]);
                if( it != commandMap.end() ) {
                    std::cout << arguments[1] << " " << *(it->second) << std::endl;    
                } else {
                    std::cout << "No entry \"" << arguments[1] << "\" exists" << std::endl;
                }
            }
            //Print all registered variables
            else if(arguments.size() == 1) {
                for(const auto& entry : commandMap) {
                    std::cout << entry.first << " " << *(entry.second) << std::endl;
                }
            }
        } 
        else {
            std::cout << "Unrecognized command \"" << arguments[0] << "\"" << std::endl;
        }
        
        std::cout << std::endl;
    }
}


void registerVariable(const std::string& variable, int* pointer)
{
    commandMap[variable] = pointer;
}


void listenStart()
{
    std::cout << "********************************" << '\n'
              << "Utility Runtime Debugger Enabled" << '\n'
              << "********************************" << std::endl;

    debug_thread = std::thread(debugListenFn);  //Starts new thread
}


}
