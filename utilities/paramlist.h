#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include <unordered_map>
#include <sstream>
#include <assert.h>
#include <utility>
#include <typeindex>
#include <type_traits>

/******************************/
/****** ParameterList *********/
/******************************/

class ParameterList
{
public:
	~ParameterList();
	 /**
	 * @brief ParameterList::setParam<T> :
	 *	Set a parameter by a name to a value.
	 * @param value :
	 * 	The name of the variable to store
	 * @param T value :
	 *  The value to set the variable ID'd by name to
	 */
	template<typename T>
    void setParam(std::string name, T value);

	 /**
	 * @brief ParameterList::getParam<T> :
	 *	Get a parameter by name, set by setParam
	 * @param name :
	 *	Name of the parameter requested. Must exist.
	 * @return :
	 *  The value associated with name, of type T.
	 */
	template<typename T>
    T getParam(std::string name) const;

    std::string toString() const;

private:

    class GenericParamList
    {
    public:
        virtual std::string toString() const;
    };

    template<typename T>
    class TypedParamList : public GenericParamList
    {
    public:
        void insertParam(std::string name, T value);
        T findParam(std::string name) const;

        std::string toString() const;
    private:
        std::unordered_map<std::string, T> myParams;
    };

	std::unordered_map<std::type_index, GenericParamList*> params;
};


/**************************************************************/
/* ParameterList Implementation */
/**************************************************************/


/******************/
/* TypedParamList */
/******************/

template <typename T>
void ParameterList::TypedParamList<T>::insertParam(std::string name, T value)
{
	myParams[name] = value;
}


template <typename T>
T ParameterList::TypedParamList<T>::findParam(std::string name) const
{
	auto entry = myParams.find(name);
	assert(entry != myParams.end());

	return entry->second;
}


template <typename T>
std::string ParameterList::TypedParamList<T>::toString() const
{
	std::stringstream ss;

	for(const auto& entry : myParams)
    {
		ss << entry.first << " : ";

        //if(std::is_fundamental<T>::value) {		//Has ostream operator <<
            //ss << entry.second << std::endl;
       // } else {
            ss << "(Value)" << std::endl;
        //}
	}

	return ss.str();
}


/*****************/
/* ParameterList */
/*****************/

template<typename T>
void ParameterList::setParam(std::string name, T value)
{
    if(params.find(typeid(T)) == params.end())
        params[typeid(T)] = new TypedParamList<T>();

	/* Find the GenericParamList at typeid(T),
       then cast result to TypedParamList<T>*.
	   This is where the magic happens.
	   */
	((TypedParamList<T>*)params[typeid(T)])->insertParam(name, value);
}


template<typename T>
T ParameterList::getParam(std::string name) const
{
	assert(params.find(typeid(T)) != params.end());

    auto entry = params.find(typeid(T));

    return ((TypedParamList<T>*)entry->second)->findParam(name);
}


#endif
