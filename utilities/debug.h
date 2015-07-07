#ifndef UTIL_DEBUG_H
#define UTIL_DEBUG_H

/* DEBUG
 * `Debug` is a small utility with a big impact.
 * It allows the changing of an arbitraty integer variable and calling of arbitrary
 * functions at runtime from the console. In essensnce, a terminal for Robobulls.
 *
 * Pointers to integers are registered to a name using "registerVariable".
 * Functions are registed to a name using "registerFunction".
 * Once running, "set" and "get" commands on the comamnd line can be used to set a new value
 * and retreieve the current value to an interger pointer, and "call" can be used to
 * call a function.
 *
 * The "listen start" function creates a new non-blocking thread that listens for 
 * lines on stdin. The accepted commands are the following:
 *
 * - set <variable> <new_value>
 *     Set a variable. Looks for a pointer registered to `variable`, and if found, changes
 *     the value pointed to it to new_value
 * - call <function> <args...>
 *     Calls the registered function "function" with args "args". "args" is given as a vector
 *     of strings passed to the function.
 * - get <variable> 
 *     Shows the current value of the variable associated by `variable`
 * - get
 *     Shows the current values of all registered variables
 */
#include <string>
#include <vector>
#include <functional>

namespace debug
{

//Function prototypes should be "void f(const std::vector<std::string>&)"
typedef std::function<void(const std::vector<std::string>&)> debug_fn;

void registerVariable(const std::string& variable, int* pointer);

void registerFunction(const std::string& name, debug_fn function);
    
void listenStart();

}

#endif

