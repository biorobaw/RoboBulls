#include "robot_low_level_controls.h"

RobotLowLevelControls::RobotLowLevelControls(QObject* parent) :
    QObject(parent)
{

}


float RobotLowLevelControls::getTargetAngularSpeed(){
    return  target_angular_speed;
}
Point RobotLowLevelControls::getTargetVelocity(){
    return target_velocity;
}
float RobotLowLevelControls::getTargetSpeed(){
    return target_velocity.norm();
}
int   RobotLowLevelControls::getKickSpeed() {
    return kick_speed;
}
bool  RobotLowLevelControls::getDribble(){
    return dribble;
}
bool  RobotLowLevelControls::getChip(){
    return chip;
}


/*! @brief Set power in m/s for the robot to kick
 * @details *Do not use;* use Skil::Kick instead
 * Sets the initial kick velocity for the robot in m/s
 * @see setDrible */
void RobotLowLevelControls::setTargetVelocity(Point velocity, float angular_speed){
    target_velocity = velocity;
    target_angular_speed = angular_speed;
}
void RobotLowLevelControls::setKickSpeed(int speed){
    kick_speed = speed;
}
void RobotLowLevelControls::setDribble(bool on){
    dribble = on; }
void RobotLowLevelControls::setChip(bool on){
    chip = on;
}
