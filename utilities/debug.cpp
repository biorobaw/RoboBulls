#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <cstdlib>
#include "model/gamemodel.h"
#include "include/config/team.h"
#include "utilities/debug.h"

namespace debug
{

//Map of registered functions to their functions
static std::unordered_map<std::string, debug_fn> funcMap;

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

    //Makes sure any initial printing is done first
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while(1)
    {
        std::cout << "> ";
        std::getline(std::cin, input); //Blocking

        std::vector<std::string> arguments = stringSplit(input, " ");

        if(arguments.size() == 0 || arguments[0].length() <= 1)
            continue;

        if(arguments[0] == "set") //Set a variable
        {
            if(arguments.size() == 3) {
                auto it = commandMap.find(arguments[1]);
                if( it != commandMap.end() ) {
                    *(it->second) = std::atoi(arguments[2].c_str()); //pointer is set here
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
            //Print all registered variables and functions
            else if(arguments.size() == 1) {
                for(const auto& entry : commandMap) {
                    std::cout << "i  " << entry.first << " " << *(entry.second) << std::endl;
                }
                for(const auto& entry : funcMap) {
                    std::cout << "f  " << entry.first << std::endl;
                }
            }
        }
        else if(arguments[0] == "call")
        {
            if(arguments.size() > 1) {
                const std::string& funcName = arguments[1];
                if(funcMap.find(funcName) != funcMap.end()) {
                    std::vector<std::string> args(arguments.begin()+2, arguments.end());
                    funcMap[funcName](args);    //Function is called here
                } else {
                    std::cout << "No registed function \"" << funcName << "\"" << std::endl;
                }
            } else {
                std::cout << "Enter a function name to call" << std::endl;
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

void registerFunction(const std::string& name, debug_fn function)
{
    funcMap[name] = function;
}

/**********************************************************************/
//Built-in functions are put and registered here
/**********************************************************************/

//utilities/debug function to remove a robot from GameModel
void builtin_remove_robot(const std::vector<std::string>& args)
{
    if(args.size() == 2 && (args[1] == "b" || args[1] == "y")) {
        int  id  = args[0][0] - '0';
        char tm  = args[1][0];
        int team = (tm == 'b') ? TEAM_BLUE : TEAM_YELLOW;
        gameModel->removeRobot(id, team);
        std::cout << "Removed robot " << id << " from team " << tm << std::endl;
    } else {
        std::cout << "Usage: <id> <team=b|y>" << std::endl;
    }
}

void listenStart()
{
    registerFunction("remove_robot", builtin_remove_robot);

    std::cout << "********************************" << '\n'
              << "Utility Runtime Debugger Enabled" << '\n'
              << "********************************" << std::endl;
    debug_thread = std::thread(debugListenFn);  //Starts new thread
}


}
