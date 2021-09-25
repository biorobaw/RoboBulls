#include "robot_low_level_controls.h"
#include "robot.h"
#include <iostream>

RobotLowLevelControls::RobotLowLevelControls(QObject* parent, Robot* robot) :
    QObject(parent), robot(robot)
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
void RobotLowLevelControls::setTargetVelocityLocal(Point velocity, float angular_speed){
    target_velocity = velocity;
    target_angular_speed = angular_speed;
}

void RobotLowLevelControls::setTargetVelocityGlobal(Point velocity, float angular_speed){
    auto v_local = velocity.rotate(-robot->getOrientation()); // convert to local
    auto flip_x = robot->flip_x;
    v_local.y *= flip_x; // undo inversion, note here we flip y instead of x because we are flipping local coordinates, not global
    target_velocity = v_local;
    target_angular_speed = flip_x*angular_speed;
}

void RobotLowLevelControls::setKickSpeed(int speed){

    kick_speed = speed;
}
void RobotLowLevelControls::setKickDistance(int distance){
    /* Function to translate a distance in meters into an initial velocity.
     * This was obtained from a third-order interpolation of
     * the following data, from an experiment on the simulator looking
     * at distance traveled based on initial kick velocity.
     *
     * Distance(m)  Initial Kick Velocity(m/s)
     * 0.04         0.5
     * 0.172        1
     * 0.352        1.5
     * 0.62         2
     * 0.952        2.5
     * 1.34         3
     * 1.836        3.5
     * 2.376        4
     * 3.000        4.5
     * 3.768        5
     * 4.512        5.5
     *
     *
     *
     *
     */
    float d = distance;
    //float d = (float)distance / 1000; // calculations were done in m/s
    //setKickSpeed((0.083*(d*d*d) - 0.759*(d*d) + 2.8812*(d) + 0.3785)*1000);
    setKickSpeed(( - 0.0000386164*(d*d) + 0.808328*(d) + 716.488));
    //setKickSpeed(5000);
    std::cout << "Kickspeed: " << kick_speed << std::endl;
}
void RobotLowLevelControls::setDribble(bool on){
    dribble = on; }
void RobotLowLevelControls::setChip(bool on){
    chip = on;
}
