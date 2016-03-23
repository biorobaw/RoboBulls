#ifndef GETCLASSNAME_H
#define GETCLASSNAME_H
#include <iostream>
#include <typeinfo>
#include <typeindex>
#ifdef __GNUC__
 #include <cxxabi.h>
 #include <unordered_map>
#endif


/* getClassName(variable)
 * Returns a string of the name of the class of a variable.
 * e.g, if "SomeClass i;" is declared, getClassName(i) will return "SomeClass".
 * if an error occurs, an empty string is returned.
 */

template<typename T>
class getclassname
{
public:
    getclassname()
    {};

    const std::string& getClassName(const T& variable);

};
#endif // GETCLASSNAME_H
