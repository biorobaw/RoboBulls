#ifndef ROBOT_H
#define ROBOT_H

#include <typeinfo>
#include <type_traits>
#include "utilities/point.h"
#include "behavior/behavior.h"
#include "skill/skill.h"
#include "behavior/genericskillbehavior.h"
#include "include/config/robot_types.h"

/**
 * @brief The Robot class
 * The robot class includes the required information about robot
 * such position, orientation, id, and etc.
 * you can assign these information to robot or access them when required
 */
class Robot
{
public:
    Robot();

    //sets
    void setRobotPosition(Point);
    void setOrientation(float);
    void setID(int);
    void setR(float);
    void setL(float);
    void setB(float);
    void setLF(float left_forward);
    void setRF(float right_forward);
    void setLB(float left_backward);
    void setRB(float right_backward);
    void setKick(bool);
    void setDrible(bool);
    void setTeam(bool);
    void setVelocity(Point);
    
    //Used to assign a behavior to the robot
    template<typename BehaviorType, typename... Args>
    bool assignBeh(Args&&... args);

    //Used to assign a single skill to a robot
    template<typename SkillType, typename... Args>
    bool assignSkill(Args&&... args);

    //gets
    Point getRobotPosition();
    Point getVelocity();
    float getSpeed();
    float getOrientation();
    int   getID();
    int   getR();
    int   getL();
    int   getB();
    int   getLF();
    int   getRF();
    int   getLB();
    int   getRB();
    int   getKick();
    bool  getDrible();
    bool  isOnMyTeam();
    Behavior* getCurrentBeh();
    robotType type();

    void clearCurrentBeh();
    std::string toString();

    bool hasBall;
    bool hasBeh;

private:
    void setCurrentBeh(Behavior *);
    
    int id;                     //Robot numerical ID
    Point robotPosition;        // X/Y Position in points
    Point velocity;             // Velocity in m/s
    float orientation;          // orientation of the robot
    float LF, RF, LB, RB;       // used for robot's movements
    Behavior * currentBehavior; // Pointer to current behavior
    bool kick;                  // Robot is kicking
    bool drible;                // Roboty is dribbling
    bool team;                  // On myTeam? 1/0
};


/*********************************************/


template<typename BehaviorType, typename... Args>
bool Robot::assignBeh(Args&&... args)
{
    static_assert(std::is_constructible<BehaviorType, Args...>::value,
        "Behavior must be constructible with these arguments");

    static_assert(std::is_base_of<Behavior, BehaviorType>::value,
        "This Behavior must derive from the Behavior base class");

    if(not(hasBeh) or typeid(*getCurrentBeh()) != typeid(BehaviorType)) {
        clearCurrentBeh();
        setCurrentBeh(new BehaviorType(args...));
        return true;
    }
	
    return false;
}

template<typename SkillType, typename... Args>
bool Robot::assignSkill(Args&&... args)
{
    return assignBeh<GenericSkillBehavior<SkillType>>(args...);
}

#endif // ROBOT_H
