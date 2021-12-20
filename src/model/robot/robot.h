#ifndef ROBOT_H
#define ROBOT_H

#include <typeinfo>
#include <type_traits>

#include "configuration/constants.h"
#include "model/moving_object.h"
#include "model/robot/robot_low_level_controls.h"
#include "navigation/commands/CmdGoToPose.h"
#include "model/robot/behavior.h"

#include <QSet>
#include <QObject>
#include <QDebug>


class RobotTeam;
class RobotImplementation;
class RobotPilot;
class GameState;

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

    int  getId();
    std::string  toString();

    // ======= IMPLEMENTATION SPECIFIC FUNCTIONS ===================

    Robot* specifyImplementation(RobotImplementation* impl);
    bool   hasKicker();
    bool   isHolonomic();

    // ================ TEAM RELATED ==============================

    int        getTeamId();
    RobotTeam* getTeam();
    Robot*     setTeam(RobotTeam* team);


    int        getRole(); // get the role of the robot in the team
    Robot*     setRole(int role); // role 0 is reserved for the goalie
    bool       isGoalie();

    // =============== High Level Control =========================

    Robot*     runControlCycle();

    template<typename BehaviorType, typename... Args>
    bool setBehavior(Args&&... args);

    //sets behavior using factory constructor.
    bool setBehavior(std::string BehaviorName);

    bool       hasBehavior();
    Behavior*  getBehavior();
    Robot*     clearBehavior();
    bool       useOverridenControls();

    GameState* getGameState();
    Robot*     setGameState(GameState* state);


    Robot* goToPose(CmdGoToPose& newCommand);
    bool   completedGoToPoseCmd();

    bool      hasBall();
    Robot*    setHasBall(bool new_value);


    // ============= LOW LEVEL CONTROL ==========================
    RobotLowLevelControls* getOverridenController();
    RobotLowLevelControls* getActiveController();

public slots:
    void  setUseOverridenControls(bool);

protected:

    // =============== PRIVATE FUNCTION ==========================
    Robot* setBehavior(Behavior *);
    

    // =========== BASIC PRIVATE DATA ============================
    int id;
    int team_id;
    int team_role = -1; // robot has nor role until assigned
    RobotTeam* team;

    // ====== HIGH LEVEL CONTROL =================================
    Behavior * behavior = nullptr;
    bool has_ball = false;

    GameState* game_state;

    // ====== LOW LEVEL CONTROLS ================================
    RobotLowLevelControls* overriden_controls = new RobotLowLevelControls(this,this);
    bool use_overriden_controls = true;


    // ====== IMPLEMENTATION DEPENDENT VARIABLES =================
    bool has_kicker = true;
    bool is_holonomic = true;
    RobotPilot* pilot = nullptr;

    // ====== Friend classes =====================================

    friend class RobotPilot;
    friend class RobotLowLevelControls;

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
bool Robot::setBehavior(Args&&... args)
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

/*! Following are functions to allow easy swapping in and out of behaviors
 *  loosely following the factory constructor structure.
*/
// ============= Allows For Interchangable Behavior at Runtime ==========================

template<typename B> bool createB(Robot *robot){return robot->setBehavior<B>();}

struct BehaviorCreator
{
    typedef std::map<std::string, bool(*)(Robot *robot)> behavior_map_type;

    static bool createInstance(std::string const& str,Robot* robot)
    {
        behavior_map_type::iterator it = getBehaviorMap()->find(str);
        if(it == getBehaviorMap()->end())//If behavior is not registered.
            return false;
        return it->second(robot);
    }
protected:
    static behavior_map_type * getBehaviorMap() //This might not be thread safe?
    {
        if(!behavior_map)
            behavior_map = new behavior_map_type;
        return behavior_map;
    }

private:
    static behavior_map_type * behavior_map;
};

template<typename B>
struct BehaviorRegister : BehaviorCreator
{
    BehaviorRegister(std::string const& str)
    {
        getBehaviorMap()->insert(std::pair(str, &createB<B>));
    }

};
/*! Each behavior that needs to be registered will need to
 have a private static BehaviorRegister<BehaviorName> reg; and will need to initialize
 it in its corresponding cpp file. see attackmain for reference.*/

#endif // ROBOT_H
