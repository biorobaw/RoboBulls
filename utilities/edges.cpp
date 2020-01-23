#include <string>
#include <unordered_map>
#include "utilities/edges.h"


/*************************************************************************/

namespace edges
{

//This is the magic: A map to map a function with its last result
static std::unordered_map<boolFunctionPtr, bool> lastResultMap;

bool edge_detect_impl(boolFunctionPtr function, bool lastResultVal, bool thisResultVal)
{
    if( lastResultMap.find(function) == lastResultMap.end() ) {
        lastResultMap[function] = false;
    }
    bool& lastResult = lastResultMap[function];
    bool  thisResult = function();
    bool  ret = false;

    if( (lastResult==lastResultVal) and (thisResult==thisResultVal) ) {
        ret = true;
    }

    lastResult = thisResult;

    return ret;
}
    
bool posedge_impl(boolFunctionPtr function)
{
    return edge_detect_impl(function, false, true);
}

bool negedge_impl(boolFunctionPtr function)
{
    return edge_detect_impl(function, true, false);
}

}

/*************************************************************************/
//Sample test module

#if 0
#include <iostream>

int i = 10;

int main()
{
    for(; i != 30; ++i) {
        std::cout << i << " " << posedge( i > 20 ) << " " << negedge( i < 25 ) << std::endl;
    }
    for(; i != 9; --i) {
        std::cout << i << " " << negedge( i > 20 ) << " " << posedge( i < 25 ) << std::endl;    
    }
    return 0;
}

#endif
