#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <cstdlib>
#include "model/gamemodel.h"

#include "utilities/debug.h"
#include "parameters/game_constants.h"

namespace debug
{

//Map of registered functions to their functions
static std::unordered_map<std::string, debug_fn> funcMap;

//Map of registered variable names to integer pointers
static std::unordered_map<std::string, float*> commandMap;

//Thread to use to listen for commands
static std::thread debugThread;

//Generic string split function
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
    std::cout << '\n';

    while(1)
    {
        std::cout << "> ";
        std::getline(std::cin, input); //Blocking

        std::vector<std::string> arguments = stringSplit(input, " ");
        if(arguments.empty() || arguments[0].length() <= 1)
            continue;

        if(arguments[0] == "help")
        {
            std::cout << "set <i>          Set an integer\n"
                      << "get <i>          Retrieve integer value\n"
                      << "get              Retrieve all registered values\n"
                      << "call <f> [args]  Call a registerd function\n"
                      << "help             Show this help text" << std::endl;
        }
        else if(arguments[0] == "set") //Set a variable
        {
            if(arguments.size() == 3) {
                auto it = commandMap.find(arguments[1]);
                if( it != commandMap.end() ) {
                    *(it->second) = std::atof(arguments[2].c_str()); //pointer is set here
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

void registerVariable(const std::string& name, float *pointer)
{
    commandMap[name] = pointer;
}

void registerFunction(const std::string& name, debug_fn function)
{
    funcMap[name] = function;
}

/**********************************************************************/
//Built-in functions are put and registered here
/**********************************************************************/
//! @cond

/* Generic template function that looks for a robot and a team, and calls a function
 * with parsed information (<id> <team>) as a callback */
template<typename Function>
void buildin_robot_action(const std::vector<std::string>& args, Function callback)
{
    if(args.size() == 2 && (args[1] == "b" || args[1] == "y")) {
        int  id  = args[0][0] - '0';
        char tm  = args[1][0];
        int team = (tm == 'b') ? TEAM_BLUE : TEAM_YELLOW;
        callback(id, team);
    } else {
        std::cout << "Usage: <id> <team=b|y>" << std::endl;
    }
}

//builtin to remove a robot from GameModel
void builtin_remove_robot(const std::vector<std::string>& args)
{
    buildin_robot_action(args, [&](int id, int team) {
        Team::getTeam(team)->removeRobot(id);
        std::cout << "Removed robot " << id << " from team " << team << std::endl;
    });
}

//builtin to a robot to GameModel
void builtin_add_robot(const std::vector<std::string>& args)
{
    buildin_robot_action(args, [&](int id, char team) {

        Team::getTeam(team)->addRobot(id);
        std::cout << "Added robot " << id << " to team " << team << std::endl;
    });
}

//! @endcond

void listenStart()
{
    //The default builtin functions and variables are registed here
    registerFunction("remove_robot", builtin_remove_robot);
    registerFunction("add_robot", builtin_add_robot);
    //TODO: FIX FOLLOWING LINE (IF NECCESARY) as variable no longer exists
//    registerVariable("goalie_id", &GOALIE_ID);

    std::cout
        << '\n'
        << "Utility Command Line Enabled      \n"
        << "Type \"help\" for more information\n"
        << "********************************  \n";

    //Starts the new thread and returns
    debugThread = std::thread(debugListenFn);
}


}
