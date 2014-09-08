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
	/* Constructor; Set single assignment on construction.
	 * This is a large convenience to calling setSingleAssignment after
	 * construction.
	 */
	BehaviorAssignment(bool assignment = false);

	
    /* setBehParam(name, value)
     * The setBehParam function is used to define parameters to
     * be passed to the underlying behavior when assignBeh is called.
     *
     * Enter a string to refer to the variable by name. If we wanted to
     * set an integer identified by "myValue" to 100:
     *
     *  BehaviorAssignment<MyRobotBehavior> myAssignment;
     *  myAssignment.setBehParam("myValue", 100);
     *  myAssignment.assignBeh();
     *
     * In MyRobotBehavior's constructor (One that takes a ParameterList called list)
     * you would do this the value of "myValue":
     *
     * int value = list.getBehParam<int>("myValue");
     */
    template<typename T>
    void setBehParam(std::string name, T value);


    /* assignBeh(void)
     * Assigns the current behavior configuration to
     * all robots on myTeam via a loop.
     */
    void assignBeh(void) const;


    /* assignBeh(predicate)
     * Assigns the current behavior config only to robots which
     * predicate returns true. The function passes all robots
     * in myTeam to predicate to determine this.
     */
    void assignBeh(std::function<bool(Robot*)> predicate) const;


    /* assignBeh(robot)
     * Assigns the behavior to only one robot. This could be:
     * assignBeh(model->getMyTeam().at(0));
     */
    void assignBeh(Robot* robot) const;


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
    void assignBeh(std::initializer_list<Robot*> robotList) const;


    /* assignBeh(robotIDList)
     * This version is similar to the above assignBeh, except
     * that this version takes a brace-enclosed list of IDs,
     * then the GameModel is utilized to find those robots and
     * if found, assigns the behavior
     */
    void assignBeh(std::initializer_list<int> robotIDList) const;


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
    bool singleAssignment;
    ParameterList params;
};


/**************************************************************/
/* BehaviorAssignment Class Implementation */
/**************************************************************/

template <typename BehaviorType>
BehaviorAssignment<BehaviorType>::BehaviorAssignment(bool assignment)
    : singleAssignment(assignment)
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
    GameModel* mod = GameModel::getModel();
    for(Robot* rob : mod->getMyTeam())
       this->assignBeh(rob);
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(std::function<bool(Robot*)> predicate) const
{
    GameModel* mod = GameModel::getModel();

    for(Robot* rob : mod->getMyTeam()) {
        if(predicate(rob))
            this->assignBeh(rob);
    }
}


template <typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(Robot* robot) const
{
    /* Lambda solely because this is used thrice below.
     */
    auto doAssignment = [&](Robot* rob) {
        rob->clearCurrentBeh();
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


template<typename BehaviorType>
void BehaviorAssignment<BehaviorType>::assignBeh(std::initializer_list<int> robotIDList) const
{
    GameModel* gm = GameModel::getModel();

    for(int robID : robotIDList) {
        Robot* rob = gm->find(robID, gm->getMyTeam());

        if(rob != NULL)
            this->assignBeh(rob);
    }
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
