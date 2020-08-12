#ifndef ROBOT_H
#define ROBOT_H
#include <typeinfo>
#include <type_traits>
#include "utilities/point.h"
#include "strategy/behavior.h"
#include "strategy/behaviors/genericskillbehavior.h"
#include "iostream"
#include <set>
#include "model/constants.h"
#include "model/moving_object.h"
#include "navigation/commands/CmdGoToPose.h"

#include <QSet>

class RobotTeam;
class RobotProxy;
class Pilot;

#define ROLE_GOALIE 0

/*! @addtogroup everydayuse
 * @{ */

/*! The Robot class representing Robots on the field
 * The robot class includes the required information about robot
 * such position, orientation, id, velocity, and behavior information.<br>
 * These are usually returned by a GameModel function. They should not be created
 * directly under normal circumstances
 */
class Robot : public MovingObject
{
public:

    // ====== Constructor and basic info ==========================

    Robot(int _id, int _team);
    ~Robot();

    int  getID();
    std::string  toString();

    // ======= IMPLEMENTATION SPECIFIC FUNCTIONS ===================

    void setRobotProxy(RobotProxy* proxy);
    bool hasKicker();
    bool isHolonomic();

    // ================ TEAM RELATED ==============================

    void       setRole(int role); // role 0 is reserved for the goalie
    int        getRole(); // get the role of the robot in the team
    bool       isGoalie();
    int        getTeamId();
    RobotTeam* getTeam();
    QSet<Robot*>& getOpponentRobots();
    Robot* getOpponentRobot(int id);

    // =============== High Level Control =========================

    Pilot*    getPilot();
    bool      hasBehavior();
    Behavior* getBehavior();
    void      performBehavior();
    void      clearBehavior();


    void goToPose(CmdGoToPose& newCommand);
    bool completedGoToPoseCmd();

    bool      hasBall();

    template<typename BehaviorType, typename... Args>
    bool assignBeh(Args&&... args);

    template<typename SkillType, typename... Args>
    bool assignSkill(Args&&... args);


    // ============= Low Level Control ============================

    float getTargetAngularSpeed();
    Point getTargetVelocity();
    float getTargetSpeed();
    int   getKickSpeed();
    bool  getDribble();
    bool  getChip();

    /*! @brief Set speed ath which the ball should be kicked in mm/s
     * @details *Do not use;* use Skil::Kick instead
     */
    void  setTargetVelocity(Point velocity, float angular_speed);
    void  setKickSpeed(int speed = 5000);
    void  setDribble(bool); // TODO: dribble speed can be set in the simulator
    void  setChip(bool);




protected:

    // =============== PRIVATE FUNCTION ==========================
    void setID(int);
    void setTeam(int);
    void setCurrentBeh(Behavior *);
    

    // =========== BASIC PRIVATE DATA ============================
    int id;
    int team_id;
    int team_role = -1; // robot has nor role until assigned

    // ====== HIGH LEVEL CONTROL =================================
    Behavior * behavior;
    bool has_ball = false;

    // ====== LOW LEVEL CONTROL ==================================
    int  kick_speed = 0;
    bool dribble = false;
    bool chip = false;
    Point target_velocity = Point(0,0);
    float target_angular_speed = 0;

    // ====== IMPLEMENTATION DEPENDENT VARIABLES =================
    bool has_kicker = false;
    bool is_holonomic = true;
    Pilot* pilot = nullptr;

    // ====== Friend classes =====================================

    friend class Pilot;

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
