#ifndef UTIL_DEBUG_H

/* DEBUG
 * `Debug` is a small utility with a big impact.
 * It allows the changing of an arbitraty integer variable at runtime from the console.
 * Pointers to integers are registered to a variable name, and once run, either the 
 * "set" and "get" commands on the comamnd line can be used to set a new value 
 * and retreieve the current value.
 *
 * The "listen start" function creates a new non-blocking thread that listens for 
 * lines on stdin. The accepted commands are the following:
 *
 * - set <variable> <new_value>
 *     Set a variable. Looks for a pointer registered to `variable`, and if found, changes
 *  the value pointed to it to new_value
 * - get <variable> 
 *     Shows the current value of the variable associated by `variable`
 * - get
 *     Shows the current values of all registered variables
 */
#include <string>

namespace debug
{

void registerVariable(const std::string& variable, int* pointer);
    
void listenStart();

}

#endif

