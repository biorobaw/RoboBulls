#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include <unordered_map>
#include <iostream>
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
	 * @param name :
	 *  The name of the variable to store
	 * @param value :
	 *  The value to set the variable to
	 */
	template<typename T>
    void setParam(const std::string& name, T value);

	 /**
	 * @brief ParameterList::getParam<T> :
	 *	Get a parameter by name, set by setParam
	 * @param name :
	 *	Name of the parameter requested. Must exist.
	 * @return :
	 *  The value associated with name, of type T.
	 */
	template<typename T>
    T getParam(const std::string& name) const;

    std::string toString() const;
	
	/* Returns the number of parameters entered across
	 * all types 
	 */
	size_t size() const;

private:

	/* ParameterList implementation classes.
	 * Uses inheritance between these two classes 
	 * to manage an unordered_map of any type.
	 * Made possible by C++11 type_index
	 */

    class GenericParamList
    {
    public:
        virtual std::string toString() const;
		virtual size_t size() const;
    };

    template<typename T>
    class TypedParamList : public GenericParamList
    {
    public:
        virtual ~TypedParamList();
        void insertParam(const std::string& name, T value);
        T findParam(const std::string& name) const;

        std::string toString() const;
		
		size_t size() const;
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
void ParameterList::TypedParamList<T>::insertParam
	(const std::string& name, T value)
{
	myParams[name] = value;
}


template <typename T>
T ParameterList::TypedParamList<T>::findParam
	(const std::string& name) const
{
	auto entry = myParams.find(name);
	
	/* Instead of asserts, this throws and exception when
	 * the name cannot be found.
	 */
	if(entry == myParams.end()) {
		std::string msg = "Unable to find parameter \"" + name + "\"";
		throw std::runtime_error(msg.c_str());
	}

	return entry->second;
}


template<typename T>
size_t ParameterList::TypedParamList<T>::size() const
{
	return myParams.size();
}


template <typename T>
std::string ParameterList::TypedParamList<T>::toString() const
{
	std::stringstream ss;

	for(const auto& entry : myParams)
    {
		ss << entry.first << " : ";
        ss << "(Value)" << std::endl;
	}

	return ss.str();
}

template<typename T>
ParameterList::TypedParamList<T>::~TypedParamList()
{}


/*****************/
/* ParameterList */
/*****************/

template<typename T>
void ParameterList::setParam(const std::string& name, T value)
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
T ParameterList::getParam(const std::string& name) const
{
    auto entry = params.find(typeid(T));
	
	if(entry == params.end()) {
        std::string msg = "\"" + std::string(typeid(T).name())
                + "\" does not have any entries";
		throw std::runtime_error(msg.c_str());
	}

    return ((TypedParamList<T>*)entry->second)->findParam(name);
}


#endif
