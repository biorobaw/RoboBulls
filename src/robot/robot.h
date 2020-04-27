#ifndef ROBOT_H
#define ROBOT_H
#include <typeinfo>
#include <type_traits>
#include "utilities/point.h"
#include "utilities/velocitycalculator.h"
#include "strategy/behavior.h"
#include "strategy/behaviors/genericskillbehavior.h"
#include "iostream"
#include <set>
#include "model/constants.h"




class RobotTeam;
class Pilot;

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
//    //! @brief Robot Constructor (does nothing)
//    Robot();

    static Robot* loadRobot(std::string type,int id, int color, RobotRole role);

    virtual ~Robot();
    static std::set<Robot*>& getAllRobots();

    //! @brief Robot Constructor with ID and team
    Robot(int _id, int _team, RobotRole role);

    //! @name Information Retrevial
    //! @{

    //! @brief Get robot id, note that ids are unique only within robot team
    int getID();

    //! @brief Get the id of the team that the robot belongs to
    int getTeamId();

    //! @brief Get a pointer to the team that the robot belongs to
    RobotTeam* getTeam();

    //! @brief Get a pointer to the robot's opponent team
    RobotTeam* getOpponentTeam();

    //! @brief Get the role of the robot in the team
    RobotRole getRole();


    Point getPosition();
    float getOrientation();
    Point getVelocityMetersPerSecond();
    Point getVelocityMillimetersPerFrame();
    float getSpeedMetersPerSecond();
    float getSpeedMillimetersPerFrame();


    bool  hasBehavior();
    void  performBehavior();
    bool  isGoalie();

    float getKick();
    bool  getDribble();
    bool  getChip();

    virtual Pilot* getPilot() = 0;
    virtual bool hasKicker() = 0;





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

    void setKick(float power = 5.0);
    void setDribble(bool);
    void setChip(bool);
    //! @}



    //! @name Misc information functions
    //! @{
    Behavior* getBehavior();
    void clearBehavior();
    std::string toString();
    //! @}

private:
    void setCurrentBeh(Behavior *);
    void setTeam(int);
    void setRobotPosition(Point);
    void setOrientation(float);
    void setID(int);
    
    int id;                     //!< Robot numerical ID
    int team;                  //!< On myTeam? 1/0
    RobotRole team_role = RobotRole::NONE;
    Point robotPosition;        //!< X/Y Position in points
    float orientation;          //!< orientation of the robot
    VelocityCalculator vel_calc;
    Behavior * behavior; //!< Pointer to current behavior



    float kick = 0;                 //!< Robot kick power in m/s
    bool  chip = false;                  //!< Robot chip
    bool  dribble = false;                //!< Robot is dribbling?


    bool hasBall;               //!< Have the ball? 1/0

    friend class GameModel;
    friend class SSLVisionListener;
    friend class Ball;

    static std::set<Robot*> all_robots;
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
    static_assert(std::is_constructible<BehaviorType, Robot*, Args...>::value,
        "Behavior must be constructible with these arguments");
    static_assert(std::is_base_of<Behavior, BehaviorType>::value,
        "This Behavior must derive from the Behavior base class");
    if( !hasBehavior() || typeid(*getBehavior()) != typeid(BehaviorType)) {
//        std::cout << "Clearing Behavior on robot " << id << std::endl;
        clearBehavior();
        setCurrentBeh(new BehaviorType(this, args...));
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
