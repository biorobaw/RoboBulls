#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"

Robot::Robot()
{
    LF = 0; LB = 0;
    RF = 0; RB = 0;
    orientation = 0;
    kick = false;
    drible = true;
    hasBeh = false;
    currentBehavior = nullptr;
}

Robot::Robot(int id, int team) : Robot()
{
    setID(id);
    setTeam(team);
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
//! @}

/*! @brief Set the robot to kick (1/0)
 * @details A property that determines whether the robot should run its kick procedure
 * (which is defined in the robot firmware). If setKick(1) is called in
 * any iteration for a robot, setKick(0) is automatically called
 * when commands are sent to the robot so that the robot does not continuously
 * keep kicking.
 * @see setDrible */
void Robot::setKick(bool Kick){kick = Kick;}

/*! @brief Set the robot to Dribble.
 * @details This is currently only functional in the simulator as
 * none of our robots are equipped with dribblers. The speed of
 * the Dribbler can be varied between 0 and 100 on the simulator.
 * Effective use in the simulator still requires study of the effect
 * of dribble speed on ball handling.
 * @see setKick*/
void Robot::setDrible(bool drible){ drible = drible; }

/*! @{
 *  @brief Query the robot's current wheel velocities */
int Robot::getL() { return LF; }
int Robot::getR() { return RF; }
int Robot::getB() { return RB; }    //RB and RL are the same for three-wheel
int Robot::getLF() { return LF; }
int Robot::getRF() { return RF; }
int Robot::getLB() { return LB; }
int Robot::getRB() { return RB; }
//! @}

//! @brief Get the robot's Point position on the field
Point Robot::getRobotPosition() { return robotPosition; }

//! @brief Returns the roobt's orientation in radians
float Robot::getOrientation() { return orientation; }

/*! @brief Get the robot's numerical ID
 * @details The robot's ID as indicated by the pattern on top of the robot. */
int Robot::getID() { return id; }

//! @brief Returns true if the robot is currently kicking this iteration
int Robot::getKick() {return kick;}

//! @brief Returns a pointer to the robot's current behavior
Behavior* Robot::getCurrentBeh(){ return currentBehavior; }

//! @brief Returns true if the robot is a member of getMyTeam in GameModel
bool Robot::isOnMyTeam() { return team; }

//! @brief Returns true if the robot curently has a Behavior
bool Robot::hasBehavior() { return hasBeh; }

//! @brief Returns the current velocity in m/s
//! @see getRobotPosition
//! @see VelocityCalculator
Point Robot::getVelocity() { return velocity; }

//! @brief Return the speed, the magnitude of the velocity (in <b>m/s</b>)
float Robot::getSpeed()
{
    Point vel = getVelocity();
    return sqrt((vel.x * vel.x) + (vel.y * vel.y));
}

//! @brief Clears (deletes) the currnt robots behavior and sets hasBeh to false
void Robot::clearCurrentBeh()
{
    hasBeh = false;
    if(currentBehavior != nullptr) {
        delete currentBehavior;
        this->currentBehavior = nullptr;
    }
}

/*! @brief Returns the robot wheel type.
 * @details This information is used by the velocity calculators in the Movement section of the code.
 * This function returns information specified in include/config/robot_types.h
 * @see RobotType
 * @see robotIDTypes
 * @return One of: (`differential`, `threeWheelOmni`, or `fourWheelOmni`). */
RobotType Robot::type()
{
    assert(id >= 0 && id < 10);
    return robotIDTypes[id];
}

//! @brief Returns a string representation of the robot
std::string Robot::toString()
{
    stringstream ss;
    ss << "\t" << getRobotPosition().toString() << "\t ID: " << getID();
    return ss.str();
}

//! @brief Return if the robot is currently dribbling
bool Robot::getDrible(){return drible;}

/****************** Private Functions ******************/

//! @brief Sets the robot's behavior pointer (do deletion is made)
void Robot::setCurrentBeh(Behavior *currentBeh)
{
    currentBehavior = currentBeh;
    hasBeh = true;
}

void Robot::setRobotPosition(Point rbtPoint){robotPosition = rbtPoint;}

void Robot::setOrientation(float ornt){orientation = ornt;}

void Robot::setID(int ID){id = ID;}

void Robot::setVelocity(Point vel) { velocity = vel; }

//! @brief Sets which team the robot is on
void Robot::setTeam(bool which) { team = which; }
