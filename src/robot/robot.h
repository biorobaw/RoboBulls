#ifndef ROBOT_H
#define ROBOT_H

#include <typeinfo>
#include <type_traits>

#include "model/constants.h"
#include "model/moving_object.h"
#include "robot/robot_low_level_controls.h"
#include "navigation/commands/CmdGoToPose.h"
#include "strategy/behaviors/genericskillbehavior.h"
#include "strategy/behavior.h"

#include <QSet>
#include <QObject>

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
class Robot : public RobotLowLevelControls, public MovingObject
{
    Q_OBJECT
public:

    // ====== Constructor and basic info ==========================

    Robot(int _team_id, int _robot_id);
    virtual ~Robot();

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

    bool      hasBehavior();
    Behavior* getBehavior();
    void      performBehavior();
    void      clearBehavior();
    bool      ignoreController();


    void goToPose(CmdGoToPose& newCommand);
    bool completedGoToPoseCmd();

    bool      hasBall();
    void      setHasBall(bool new_value);

    template<typename BehaviorType, typename... Args>
    bool assignBeh(Args&&... args);

    template<typename SkillType, typename... Args>
    bool assignSkill(Args&&... args);

    // ============= LOW LEVEL CONTROL ==========================
    RobotLowLevelControls* getOverridenController();
    RobotLowLevelControls* getActiveController();

public slots:
    void  useOverridenControls(bool);

protected:

    // =============== PRIVATE FUNCTION ==========================
    void setID(int);
    void setTeam(int);
    void setBehavior(Behavior *);
    

    // =========== BASIC PRIVATE DATA ============================
    int id;
    int team_id;
    int team_role = -1; // robot has nor role until assigned

    // ====== HIGH LEVEL CONTROL =================================
    Behavior * behavior;
    bool has_ball = false;

    // ====== LOW LEVEL CONTROLS ================================
    RobotLowLevelControls* overriden_controls = new RobotLowLevelControls(this);
    bool use_overriden_controls = true;


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
        setBehavior(new BehaviorType(this, args...));
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
