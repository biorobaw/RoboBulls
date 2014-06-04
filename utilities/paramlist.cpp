#include "utilities/paramlist.h"

/********************/
/* GenericParamList */
/********************/

std::string ParameterList::GenericParamList::toString() const
{
    return "GenericParamList";
}

/*****************/
/* ParameterList */
/*****************/

ParameterList::~ParameterList()
{
    params.clear();
}


std::string ParameterList::toString() const
{
    std::stringstream ss;

    for(const auto& entry : params)
        ss << entry.second->toString() << std::endl;

    return ss.str();
}
