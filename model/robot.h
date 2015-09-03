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
 * @brief The Robot class representing Robots on the field
 * @details The robot class includes the required information about robot
 * such position, orientation, id, velocity, and behavior information.<br>
 * These are usually returned by a GameModel function. They should not be created
 * directly under normal circumstances
 */
class Robot
{
public:
    //! @brief Robot Constructor (does nothing)
    Robot();

    //! @brief Robot Constructor with ID and team
    Robot(int id, int team);

    //! @name Information Setting (setters)
    //! @{
    void setR(float);
    void setL(float);
    void setB(float);
    void setLF(float left_forward);
    void setRF(float right_forward);
    void setLB(float left_backward);
    void setRB(float right_backward);
    void setKick(float kick = 4.0);
    void setDrible(bool);
    //! @}
    
    //! @name Behavior and Skill Assignment
    //! @{
    template<typename BehaviorType, typename... Args>
    bool assignBeh(Args&&... args);
    template<typename SkillType, typename... Args>
    bool assignSkill(Args&&... args);
    //! @}

    //! @name Information Retrevial (getters)
    //! @{
    Point getRobotPosition();
    Point getVelocity();
    float getSpeed();
    float getOrientation();
    int   getID();
    float getKick();
    bool  getDrible();
    bool  isOnMyTeam();
    bool  hasBehavior();
    RobotType type();
    //! @}

    //! @name Wheel Velocity Query
    //! @{
    int getR();
    int getL();
    int getB();
    int getLF();
    int getRF();
    int getLB();
    int getRB();
    //! @}

    //! @name Misc information functions
    //! @{
    Behavior* getCurrentBeh();
    void clearCurrentBeh();
    std::string toString();
    //! @}

private:
    void setCurrentBeh(Behavior *);
    void setTeam(bool);
    void setVelocity(Point);
    void setRobotPosition(Point);
    void setOrientation(float);
    void setID(int);
    
    int id;                     //!< Robot numerical ID
    Point robotPosition;        //!< X/Y Position in points
    Point velocity;             //!< Velocity in m/s
    float orientation;          //!< orientation of the robot
    float LF, RF, LB, RB;       //!< used for robot's movements
    Behavior * currentBehavior; //!< Pointer to current behavior
    float kick;                 //!< Robot kick power in m/s
    bool drible;                //!< Roboty is dribbling?
    bool team;                  //!< On myTeam? 1/0
    bool hasBall;               //!< Have the ball? 1/0
    bool hasBeh;                //!< Currently has a Behavior? 1/0

    friend class GameModel;
    friend class VisionComm;
};


/*********************************************/

/*! @brief Assign a Behavior to a robot
 * @details This template function is used to assign a behavior to a robot.
 * Users can optionally pass in parameters to be forwarded to your behavior’s
 * constructor (`BehaviorType`). The robot will be assigned that behavior if it is
 * not already doing so.
 * @see Behavior
 * @see assignSkill */
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

/*! @brief Assign a single Skill to a robot
 * @details assignSkill can be used to assign a single skill to a robot.
 * This is done by assigning a transparent behavior (GenericSkillBehavior)
 * that performs a single skill. Use this exactly like assignBeh,
 * and passing the Skill’s arguments instead
 * @see assignBeh
 * @see Skill
 * @see GenericSkillBehavior */
template<typename SkillType, typename... Args>
bool Robot::assignSkill(Args&&... args)
{
    return assignBeh<GenericSkillBehavior<SkillType>>(args...);
}

#endif // ROBOT_H
