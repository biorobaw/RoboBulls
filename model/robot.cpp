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

void setRobotPosition(Point); Point getRobotPosition();
/*! The robot's current position as a Point object.
 * The setter is used by VisionComm to update the
 * robot's position at each iteration. */

Point getVelocity(); float getSpeed();
/*! These functions return the robot’s instantaneous velocity in m/s
 * as an X/Y pair and speed. Speed is the magnitude of velocity. */

void setOrientation(float); float getOrientation();
/*! The robot's current position orientation as a float.
 * The setter is used by VisionComm to update the robot's
 * orientation at each iteration. */

void setID(int); int getID();



void Robot::setRobotPosition(Point rbtPoint){robotPosition = rbtPoint;}

void Robot::setOrientation(float ornt){orientation = ornt;}

void Robot::setID(int ID){id = ID;}

/*! @name Wheel Velocity Assignment @{
 *  @brief Assigns velocites to wheels
 * <i>Do not use directly</i>. These functions are used to sey
 * the wheel velocities on the robots. These setters directly
 * affect the commands that get sent to the robots as the
 * communication class uses the getters to determine what
 * velocities to send for each robot.
 * <b>For differential drives</b>, only setR (right wheel velocity)
 * and setL (left wheel velocity) are necessary. On the simulator,
 * this sets both right and both left wheel velocities to the same
 * value to mimic a differential drive.
 * <b>For three wheeled robots</b>, only setR (right wheel velocity), setL
 * (left wheel velocity) and setB (back wheel velocity) are necessary.
 * This is not designed for the simulator, but it could be tested by
 * adapting the simulator settings file if necessary.
 * <b>For four wheeled robots</b>, only setLF (left front wheel velocity),
 * setRF (right front wheel velocity), setLB (left back wheel velocity)
 * and setRB (right back wheel velocity) are necessary. */
void Robot::setL(float left){LF = LB = left;}
void Robot::setR(float right){RF = RB = right;}
void Robot::setB(float back){LB = RB = back;}
void Robot::setLF(float left_forward){LF = left_forward;}
void Robot::setRF(float right_forward){RF = right_forward;}
void Robot::setLB(float left_backward){LB = left_backward;}
void Robot::setRB(float right_backward){RB = right_backward;}
//!@}

void Robot::setTeam(bool which) { team = which; }

/*! @brief Set the robot to kick (1/0) *
 * A property that determines whether the robot should run its kick procedure
 * (which is defined in the robot firmware). If setKick(1) is called in
 * any iteration for a robot, setKick(0) is automatically called
 * when commands are sent to the robot so that the robot does not continuously
 * keep kicking.
 * \see setDrible
 */
void Robot::setKick(bool Kick){kick = Kick;}

/*! @brief Set the robot to Dribble.
 * This is currently only functional in the simulator as
 * none of our robots are equipped with dribblers. The speed of
 * the Dribbler can be varied between 0 and 100 on the simulator.
 * Effective use in the simulator still requires study of the effect
 * of dribble speed on ball handling.
 * @see setKick
 */
void Robot::setDrible(bool drible){ drible = drible; }

void Robot::setCurrentBeh(Behavior *currentBeh)
{
    currentBehavior = currentBeh;
    hasBeh = true;
}

void Robot::setVelocity(Point vel) { velocity = vel; }

Point Robot::getRobotPosition() { return robotPosition; }

float Robot::getOrientation() { return orientation; }

/*! @brief Get the robot's numerical ID
 * The robot's ID as indicated by the pattern on top of the robot. */
int Robot::getID() { return id; }

/*! @name Wheel Velocitiy Retrevial
 * @brief Query the robot's current wheel velocities
 * Check out the setters for details
 * @see setL
 * @see setR */
int Robot::getL() { return LF; }

int Robot::getR() { return RF; }

int Robot::getB() { return RB; }    //RB and RL are the same for three-wheel

int Robot::getLF() { return LF; }

int Robot::getRF() { return RF; }

int Robot::getLB() { return LB; }

int Robot::getRB() { return RB; }

int Robot::getKick() {return kick;}

Behavior* Robot::getCurrentBeh(){ return currentBehavior; }

bool Robot::isOnMyTeam() { return team; }

Point Robot::getVelocity() { return velocity; }

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

/*! @brief Returns the robot type.
 * This information is used by the velocity calculators in the Movement section of the code.
 * This function returns information specified in include/config/robot_types.h
 * @see RobotType
 * @see robotIDTypes
 * @return One of: (`differential`, `threeWheelOmni`, or `fourWheelOmni`). */
RobotType Robot::type()
{
    assert(id >= 0 && id < 10);
    return robotIDTypes[id];
}

std::string Robot::toString()
{
    stringstream ss;
    ss << "\t" << getRobotPosition().toString() << "\t ID: " << getID();
    return ss.str();
}

bool Robot::getDrible(){return drible;}

