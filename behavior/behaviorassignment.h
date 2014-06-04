#ifndef BEH_ASSIGNMENT_H
#define BEH_ASSIGNMENT_H

/**********************************************/
/*********** Behavior Assignment **************/
/**********************************************/
/* This class is meant to steamline specific behavior assigning
 * to an arbitrary amount of robots. It is created with a template
 * argument of a Behavior
 *
 * James W
 */

#include <string>
#include <typeinfo>
#include <functional>
#include <initializer_list>
#include "model/gamemodel.h"
#include "behavior/behavior.h"
#include "utilities/paramlist.h"

template <typename BehaviorType>
class BehaviorAssignment
{
public:
    BehaviorAssignment();

    template<typename T>
    void setBehParam(std::string name, T value);

    void assignBeh(void) const;
    void assignBeh(std::function<bool(Robot*)> predicate) const;
    void assignBeh(Robot* robot) const;
    void assignBeh(std::initializer_list<Robot*> robotList) const;

    void setSingleAssignment(bool assignment);

    std::string toString() const;

private:
    bool singleAssignment;

    ParameterList params;
};


/**************************************************************/
/* Strategy Class Implementation */
/**************************************************************/

template <typename BehaviorType>
BehaviorAssignment<BehaviorType>::BehaviorAssignment()
    : singleAssignment(false)
{
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::setSingleAssignment(bool assignment)
{
    this->singleAssignment = assignment;
}


template <typename BehaviorType>
template <typename T>
void BehaviorAssignment<BehaviorType>::setBehParam(std::string name, T value)
{
    params.setParam(name, value);
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(void) const
{
    //this->assignBeh([](Robot* rob){ return true; });	//Assign to all robots; inefficient

    GameModel* mod = GameModel::getModel();

    for(Robot* rob : mod->getMyTeam())
       this->assignBeh(rob);
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(std::function<bool(Robot*)> predicate) const
{
    GameModel* mod = GameModel::getModel();

    for(Robot* rob : mod->getMyTeam())
    {
        if(predicate(rob))
            this->assignBeh(rob);
    }
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(Robot* robot) const
{
    /* if BehaviorType has a ParameterList constructor, use it. Otherwise, default
     * should be all there is. Lambda solely because this is used twice below.
     * Honestly, this could cause some confusion down the road, watch for bugs.
     */
    auto doAssignment = [&](Robot* rob) {
        //delete rob->getCurrentBeh();
        rob->setCurrentBeh(new BehaviorType(params));
        };

    if(!this->singleAssignment) {
        doAssignment(robot);
    } else {
        if(!robot->hasBeh) {
            doAssignment(robot);
        } else if(typeid(*robot->getCurrentBeh()) != typeid(BehaviorType)) {
            doAssignment(robot);
        } else {
            robot->setCurrentBeh(robot->getCurrentBeh());
        }
    }
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(std::initializer_list<Robot*> robotList) const
{
    for(Robot* rob : robotList)
        this->assignBeh(rob);
}


template <typename BehaviorType>
std::string BehaviorAssignment<BehaviorType>::toString() const
{
    std::stringstream ss;

    ss << "Single Assignment: " << std::boolalpha
       << singleAssignment << std::noboolalpha
       << '\n' << std::endl;

    ss << "Parameters:" << std::endl;
    ss << params.toString() << std::endl;

    return ss.str();
}

#endif // BEH_ASSIGNMENT_H
