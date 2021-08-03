#include "CmdGoToPose.h"
#include "utilities/measurements.h"



float CmdGoToPose::default_velocity_multiplier = 1;
float CmdGoToPose::default_distance_tolerance = 5;
float CmdGoToPose::default_angle_tolerance = 5*M_PI/180;



CmdGoToPose::CmdGoToPose(Point targetPose, float targetAngle,bool avoidObstacles, bool avoidBall) :
    target_pose(targetPose),
    target_angle(targetAngle),
    avoid_obstacles(avoidObstacles),
    avoid_ball(avoidBall),
    velocity_multiplier(default_velocity_multiplier),
    distance_tolerance(default_distance_tolerance),
    angle_tolerance(default_angle_tolerance)
{}


CmdGoToPose::CmdGoToPose(Point targetPose,bool avoidObstacles, bool avoidBall) :
    CmdGoToPose(targetPose,UNUSED_ANGLE_VALUE,avoidObstacles,avoidBall)
{}

CmdGoToPose::CmdGoToPose(CmdGoToPose& cmd) :
    target_pose(cmd.target_pose),
    target_angle(cmd.target_angle),
    avoid_obstacles(cmd.avoid_obstacles),
    avoid_ball(cmd.avoid_ball),
    velocity_multiplier(cmd.velocity_multiplier),
    distance_tolerance(cmd.distance_tolerance),
    angle_tolerance(cmd.angle_tolerance)
{}


bool CmdGoToPose::hasTargetAngle()  {
    return target_angle != UNUSED_ANGLE_VALUE;
}

bool CmdGoToPose::completed(Point currentPosition, float currentAngle) {
    return Measurements::isClose(currentPosition, target_pose, distance_tolerance) &&
      (!hasTargetAngle() || Measurements::isClose(currentAngle, target_angle, angle_tolerance));
}

void CmdGoToPose::setTarget(Point targetPose,float targetAngle){
    this->target_pose = targetPose;
    this->target_angle = targetAngle;
}
