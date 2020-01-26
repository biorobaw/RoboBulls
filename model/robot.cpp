#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"
#include "team.h"

std::set<Robot*> Robot::all_robots;

Robot::Robot()
{
    LF = 0; LB = 0;
    RF = 0; RB = 0;
    orientation = 0;
    kick = 0;
    xvel_cmd = 0;
    yvel_cmd = 0;
    angvel_cmd = 0;
    dribble = false;
    hasBeh = false;
    behavior = nullptr;
    hasKickerVar = false;

    all_robots.insert(this);
}

Robot::Robot(int _id, int _team, std::string robot_type, RobotRole role) :
    Robot()
{
    id = _id;
    team = _team;
    team_role = role;

    if(robot_type == "yisibot"){
        hasKickerVar = true;
        driveType = DriveType::fourWheelOmni;

    } else if (robot_type == "grsim") {
        hasKickerVar = true;
        driveType = DriveType::fourWheelOmni;
    } else if (robot_type == "rpi_2019"){
        hasKickerVar = false;
        driveType = DriveType::differential;

    } else if (robot_type == "none") {
        // robot is not being controlled, set dummy values
        hasKickerVar = true;
        driveType = DriveType::fourWheelOmni;
    } else {
        std::cout << "ERROR in Robot contstructor, robot_type not recognized" <<std::endl;
        exit(-1);
    }
}

Robot::~Robot(){
    all_robots.erase(this);
}

/*! @{
 *  @brief Assigns velocites to a wheels <i>Do not use directly</i>  */
void Robot::setL(float left){LF = LB = left;}
void Robot::setR(float right){RF = RB = right;}
void Robot::setB(float back){LB = RB = back;}
void Robot::setLF(float left_forward){LF = left_forward;}
void Robot::setRF(float right_forward){RF = right_forward;}
void Robot::setLB(float left_backward){LB = left_backward;}
void Robot::setRB(float right_backward){RB = right_backward;}
void Robot::setVelCmd(float x, float y, float ang_vel){
    xvel_cmd = x;
    yvel_cmd = y;
    angvel_cmd = ang_vel;
}

void Robot::setXVel(float x) {xvel_cmd = x;}
void Robot::setYVel(float y) {yvel_cmd = y;}
void Robot::setAngVel(float ang_vel) {angvel_cmd = ang_vel;}

//! @}

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

/*! @{
 *  @brief Query the robot's current wheel velocities */
int Robot::getL() { return LF; }
int Robot::getR() { return RF; }
int Robot::getB() { return RB; }    //RB and RL are the same for three-wheel
int Robot::getLF() { return LF; }
int Robot::getRF() { return RF; }
int Robot::getLB() { return LB; }
int Robot::getRB() { return RB; }
int Robot::getXVel() { return xvel_cmd; }
int Robot::getYVel() { return yvel_cmd; }
float Robot::getAngVel() { return angvel_cmd; }
//! @}

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
bool Robot::hasBehavior() { return hasBeh; }

//! @brief Returns true if the robot has a hardware ability to kick.
bool Robot::hasKicker()
{
    return hasKickerVar;
}

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
    hasBeh = false;
    if(behavior != nullptr) {
        delete behavior;
        this->behavior = nullptr;
    }
}

/*! @brief Returns the robot wheel type.
 * @details This information is used by the velocity calculators in the Movement section of the code.
 * This function returns information specified in include/config/robot_types.h
 * @see DriveType
 * @return One of: (`differential`, `threeWheelOmni`, or `fourWheelOmni`). */
DriveType Robot::getDriveType()
{
    return driveType;
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
    hasBeh = true;
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
