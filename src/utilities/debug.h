#ifndef UTIL_DEBUG_H
#define UTIL_DEBUG_H
#include <string>
#include <vector>
#include <functional>

/*! @brief RoboBulls command line
 * @author JamesW
 * @details The debug command line allows the changing of an arbitraty
 * floating point variables calling of arbitrary functions at runtime from the console.
 * In essensnce, a terminal for Robobulls.<br>
 *
 * Pointers to floats are registered to a name using "registerVariable".
 * Functions are registed to a name using "registerFunction". Once running,
 * "set" and "get" commands on the comamnd line can be used to set a new value
 * and retreieve the current value to a pointer, and "call" can be
 * used tocall a function.
 *
 * The `listenStart` function spawns new thread that listens on for
 * lines on stdin. The accepted commands are the following. In addition, some
 * built-in functions are in the command line.
 *
 * Command Line Reference
 * ----------------------
 * Command                    | Effect
 * ---------------------------|-------
 * set <variable> <new_value> | Set a variable. Looks for a pointer registered to `variable`, and if found, changes the value pointed to it to new_value
 * call <function> <args...>  | Calls the registered function "function" with args "args". "args" is given as a vector of strings passed to the function.
 * get <variable>             | Shows the current value of the variable associated by `variable`
 * get                        | Shows the current values of all registered variables and functions
 * help                       | Shows this help
 *
 * Builtin Functions
 * -----------------
 * Function                      | Effect
 * ------------------------------|-------
 * remove_robot <id> <team=b\|y> | Removes a robot with id *id* to team *team*
 * add_robot <id> <team=b\|y>    | Adds a robot with id *id* from team *team*
 */
namespace debug
{

/*! @brief Registered function prototype.
 * @details The arguments typed from a "call" command will be passed to the function,
 * split by space */
typedef std::function<void(const std::vector<std::string>&)> debug_fn;

/*! @brief Register a variable for appearance in the command line
 * @param variable The name of the variable in the command line
 * @param pointer Pointer to float that will be written to with a "set" command */
void registerVariable(const std::string& name, float* pointer);

/*! @brief Register a function to be called from the command line
 * @details A function
 * @param name The name to refer to the function by
 * @param function The fuction to register, called with a `call name` command*/
void registerFunction(const std::string& name, debug_fn function);

/*! @brief Starts the command line listener thread
 * @details Starts a new thread that listens for commands */
void listenStart();

void debugCallFn(std::string args);

}

#endif

