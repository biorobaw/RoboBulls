#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"
#include "model/team.h"

std::set<Robot*> Robot::all_robots;


Robot::Robot(int id, int team, RobotRole role) :
    id(id),
    team(team),
    team_role(role),
    orientation(0),
    behavior(nullptr),
    kick(0),
    dribble(false)
{
    all_robots.insert(this);

//    if(robot_type == "yisibot"){

//    } else if (robot_type == "grsim") {
//    } else if (robot_type == "rpi_2019"){
//    } else if (robot_type == "none") {
//    } else exit(-1);
//    }
}

Robot::~Robot(){
    all_robots.erase(this);
}



/*! @brief Set power in m/s for the robot to kick
 * @details *Do not use;* use Skil::Kick instead
 * Sets the initial kick velocity for the robot in m/s
 * @see setDrible */
void Robot::setKick(float power){ kick = power; }

/*! @brief Set the robot to Dribble.
 * @details This is currently only functional in the simulator as
 * none of our robots are equipped with dribblers. The speed of
 * the Dribbler can be varied between 0 and 100 on the simulator.
 * Effective use in the simulator still requires study of the effect
 * of dribble speed on ball handling.
 * @see setKick*/
void Robot::setDribble(bool on){ dribble = on; }

void Robot::setChip(bool on){ chip = on; }


//! @brief Get the robot's Point position on the field
Point Robot::getPosition() { return robotPosition; }

//! @brief Returns the roobt's orientation in radians
float Robot::getOrientation() { return orientation; }

/*! @brief Get the robot's numerical ID
 * @details The robot's ID as indicated by the pattern on top of the robot. */
int Robot::getID() { return id; }

//! @brief Returns the current kick power of the robot (or 0 if not kicking)
float Robot::getKick() {return kick;}

//! @brief Returns a pointer to the robot's current behavior
Behavior* Robot::getBehavior(){ return behavior; }


//! @brief Returns true if the robot curently has a Behavior
bool Robot::hasBehavior() { return behavior != nullptr ; }

//! @brief Returns the current velocity in m/s
//! @see getRobotPosition
//! @see VelocityCalculator
Point Robot::getVelocityMetersPerSecond() { return vel_calc.getVelocityMetersPerSecond(); }

//! @brief Returns the current velocity in millimeters/frame
//! @see getRobotPosition
//! @see VelocityCalculator
Point Robot::getVelocityMillimetersPerFrame() { return vel_calc.getVelocityMillimetersPerFrame(); }

//! @brief Return the speed, the magnitude of the velocity (in <b>m/s</b>)
float Robot::getSpeedMetersPerSecond()
{
    Point vel = getVelocityMetersPerSecond();
    return sqrt((vel.x * vel.x) + (vel.y * vel.y));
}

//! @brief Return the speed, the magnitude of the velocity (in <b>millimeters/frame</b>)
float Robot::getSpeedMillimetersPerFrame()
{
    Point vel = getVelocityMillimetersPerFrame();
    return sqrt((vel.x * vel.x) + (vel.y * vel.y));
}

//! @brief Clears (deletes) the currnt robots behavior and sets hasBeh to false
void Robot::clearBehavior()
{
    if(behavior != nullptr) {
        delete behavior;
        this->behavior = nullptr;
    }
}

//! @brief Returns a string representation of the robot
std::string Robot::toString()
{
    std::stringstream ss;
    ss << "\t" << getPosition().toString() << "\t ID66666: " << getID();
    return ss.str();
}


//! @brief Return if the robot is currently dribbling
bool Robot::getDribble(){return dribble;}
bool Robot::getChip(){return chip;}

/****************** Private Functions ******************/

//! @brief Sets the robot's behavior pointer (do deletion is made)
void Robot::setCurrentBeh(Behavior *currentBeh)
{
    behavior = currentBeh;
}

void Robot::setRobotPosition(Point rbtPoint){
    robotPosition = rbtPoint;
    vel_calc.update(rbtPoint);
}

void Robot::setOrientation(float ornt){orientation = ornt;}

void Robot::setID(int ID){id = ID;}

//! @brief Sets which team the robot is on
void Robot::setTeam(int which) { team = which; }

int Robot::getTeamId(){
    return team;
}

Team* Robot::getTeam(){
    return Team::getTeam(team);
}

Team* Robot::getOpponentTeam(){
    return Team::getTeam(1-team);
}

RobotRole Robot::getRole(){
    return team_role;
}

bool Robot::isGoalie(){
    return team_role == RobotRole::GOALIE;
}

std::set<Robot*>& Robot::getAllRobots(){
    return all_robots;
}

bool  Robot::hasKicker(){
    return false;
};
