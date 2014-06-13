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
    for(auto& entry : params)
    {
        auto tList = reinterpret_cast<TypedParamList<long>*>(entry.second);
        delete tList;
    }

    params.clear();
}


std::string ParameterList::toString() const
{
    std::stringstream ss;

    for(const auto& entry : params)
        ss << entry.second->toString() << std::endl;

    return ss.str();
}
