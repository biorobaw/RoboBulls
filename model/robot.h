#ifndef ROBOT_H
#define ROBOT_H
#include <typeinfo>
#include <type_traits>
#include "utilities/point.h"
#include "utilities/velocitycalculator.h"
#include "behavior/behavior.h"
#include "skill/skill.h"
#include "behavior/genericskillbehavior.h"
#include "include/config/robot_types.h"
#include "iostream"

/*! @addtogroup everydayuse
 * @{ */

/*! The Robot class representing Robots on the field
 * The robot class includes the required information about robot
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

    //! @name Information Retrevial
    //! @{
    Point getPosition();
    Point getVelocityMetersPerSecond();
    Point getVelocityMillimetersPerFrame();
    float getSpeedMetersPerSecond();
    float getSpeedMillimetersPerFrame();
    float getOrientation();
    int   getID();
    float getKick();
    bool  getDribble();
    bool  getChip();
    bool  isOnMyTeam();
    bool  hasBehavior();
    bool  hasKicker();
    RobotType type();
    //! @}

    //! @name Behavior and Skill Assignment
    //! @{
    template<typename BehaviorType, typename... Args>
    bool assignBeh(Args&&... args);
    template<typename SkillType, typename... Args>
    bool assignSkill(Args&&... args);
    //! @}

    //! @name Information Setting
    //! @{
    void setR(float);
    void setL(float);
    void setB(float);
    void setLF(float left_forward);
    void setRF(float right_forward);
    void setLB(float left_backward);
    void setRB(float right_backward);
    void setVelCmd(float xvel, float yvel, float angvel);
    void setXVel(float xvel);
    void setYVel(float yvel);
    void setAngVel(float angvel);
    void setKick(float power = 5.0);
    void setDribble(bool);
    void setChip(bool);
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
    int getXVel();
    int getYVel();
    int getAngVel();
    //! @}

    //! @name Misc information functions
    //! @{
    Behavior* getBehavior();
    void clearBehavior();
    std::string toString();
    //! @}

private:
    void setCurrentBeh(Behavior *);
    void setTeam(bool);
    void setRobotPosition(Point);
    void setOrientation(float);
    void setID(int);
    
    int id;                     //!< Robot numerical ID
    Point robotPosition;        //!< X/Y Position in points
    VelocityCalculator vel_calc;
    float orientation;          //!< orientation of the robot
    float LF, RF, LB, RB;       //!< used for robot's movements
    float xvel_cmd, yvel_cmd, angvel_cmd;   //!< used for yisi robot's movements
    Behavior * behavior; //!< Pointer to current behavior
    float kick = 0;                 //!< Robot kick power in m/s
    bool chip = false;                  //!< Robot chip
    bool dribble = false;                //!< Robot is dribbling?
    bool team;                  //!< On myTeam? 1/0
    bool hasBall;               //!< Have the ball? 1/0
    bool hasBeh;                //!< Currently has a Behavior? 1/0

    friend class GameModel;
    friend class VisionComm;
};

//! @}

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
    if(not(hasBeh) or typeid(*getBehavior()) != typeid(BehaviorType)) {
//        std::cout << "Clearing Behavior on robot " << id << std::endl;
        clearBehavior();
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
