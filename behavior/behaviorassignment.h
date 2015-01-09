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
#include <type_traits>
#include <functional>
#include <initializer_list>
#include "model/gamemodel.h"

template <typename BehaviorType>
class BehaviorAssignment
{
public:
    /* Constructor; Set single assignment on construction.
     * This is a large convenience to calling setSingleAssignment after
     * construction.
     */
    BehaviorAssignment(bool assignment = false);

    /* assignBeh(void)
     * Assigns the current behavior configuration to
     * all robots on myTeam via a loop.
     */
    template<typename... Args>
    void assignBeh(Args&&... args) const;


    /* assignBeh(predicate)
     * Assigns the current behavior config only to robots which
     * predicate returns true. The function passes all robots
     * in myTeam to predicate to determine this.
     */
    template<typename... Args>
    void assignBeh(std::function<bool(Robot*)> predicate, Args&&... args) const;


    /* assignBeh(robot)
     * Assigns the behavior to only one robot. This could be:
     * assignBeh(model->getMyTeam().at(0));
     */
    template<typename... Args>
    void assignBeh(Robot* robot, Args&&... args) const;


    /* assignBeh(robotList)
     * Given a brace-enclosed list of Robot*, assigns the behavior
     * to all of them. This is a shortcut of assigning to single
     * robots one-by-one.
     * Example:
     *
     * Robot* r1 = model->getMyTeam().at(0);
     * Robot* r2 = model->getMyTeam().at(1);
     * myBehaviorAssignment.assignBeh( {r1, r2} );
     */
    template<typename... Args>
    void assignBeh(std::initializer_list<Robot*> robotList, Args&&... args) const;

    
    /* assignBeh(robotID)
     * Can be used to assign a behavior to a robot by ID, abstracting the 
     * need to use gameModel->find or whatever.
     */    
    template<typename... Args>
    void assignBeh(int robotID, Args&&... args) const;

    /* assignBeh(robotIDList)
     * This version is similar to the above assignBeh, except
     * that this version takes a brace-enclosed list of IDs,
     * then the GameModel is utilized to find those robots and
     * if found, assigns the behavior
     */
    template<typename... Args>
    void assignBeh(std::initializer_list<int> robotIDList, Args&&... args) const;


    /* setSingleAssignment(bool)
     * Set this true if you want the robot to retain its behavior each
     * iteration. For example, PenaltyBehavior uses this to retain its
     * Behavior state (moving, kicking, stopping) each update instead
     * of being overwritten.
     */
    void setSingleAssignment(bool assignment);


    /* toString()
     * Returns a string representation of the BehaviorAssignment.
     * Includes Behavior Params and singleAssignment's status
     */
    std::string toString() const;

private:
    template<typename... Args>
    void doAssignment(Robot* robot, Args&&... args) const;

    bool singleAssignment;
};


/**************************************************************/
/* BehaviorAssignment Class Implementation */
/**************************************************************/

template <typename BehaviorType>
BehaviorAssignment<BehaviorType>::BehaviorAssignment(bool assignment)
    : singleAssignment(assignment)
    { }


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::setSingleAssignment(bool assignment)
{
    this->singleAssignment = assignment;
}


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh(Args&&... args) const
{
    GameModel* mod = GameModel::getModel();
    for(Robot* rob : mod->getMyTeam())
       this->assignBeh(rob, args...);
}


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh
    (std::function<bool(Robot*)> predicate, Args&&... args) const
{
    GameModel* mod = GameModel::getModel();

    for(Robot* rob : mod->getMyTeam()) {
        if(predicate(rob))
            this->assignBeh(rob, args...);
    }
}


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh
    (std::initializer_list<Robot*> robotList,  Args&&... args) const
{
    for(Robot* rob : robotList)
        this->assignBeh(rob, args...);
}


template <typename BehaviorType>
template<typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh(int robotID, Args&&... args) const
{
    assignBeh({robotID}, args...);
}    


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh
    (std::initializer_list<int> robotIDList, Args&&... args) const
{
    GameModel* gm = GameModel::getModel();

    for(int robID : robotIDList) {
        Robot* rob = gm->findMyTeam(robID);
        if(rob != NULL)
            this->assignBeh(rob, args...);
    }
}


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::assignBeh(Robot* robot, Args&&... args) const
{
    if(!this->singleAssignment) {
        doAssignment(robot, args...);
    } else {
        if(!robot->hasBeh) {
            doAssignment(robot, args...);
        } else if(typeid(*robot->getCurrentBeh()) != typeid(BehaviorType)) {
            doAssignment(robot, args...);
        } 
    }
}


template <typename BehaviorType>
template <typename... Args>
void BehaviorAssignment<BehaviorType>::doAssignment(Robot* robot, Args&&... args) const
{
    static_assert(std::is_constructible<BehaviorType, Args...>::value,
        "Behavior must be constructible with these arguments");

    robot->clearCurrentBeh();
    robot->assignBeh<BehaviorType>(args...);
}


template <typename BehaviorType>
std::string BehaviorAssignment<BehaviorType>::toString() const
{
    return "BehaviorAssignment";
}


#endif // BEH_ASSIGNMENT_H
