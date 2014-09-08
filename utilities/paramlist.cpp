#include "utilities/paramlist.h"

/********************/
/* GenericParamList */
/********************/

std::string ParameterList::GenericParamList::toString() const
{
    return "GenericParamList";
}


size_t ParameterList::GenericParamList::size() const
{
	return 0;
}


size_t ParameterList::size() const
{
	size_t m_size = 0;
    for(auto& entry : params)
        m_size += entry.second->size();
	return m_size;
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
