#include "CmdGoToPose.h"
#include "utilities/measurements.h"



float CmdGoToPose::default_velocity_multiplier = 1;
float CmdGoToPose::default_distance_tolerance = 40;
float CmdGoToPose::default_angle_tolerance = 5*M_PI/180;



CmdGoToPose::CmdGoToPose(Point targetPose, float targetAngle,bool avoidObstacles, bool avoidBall) :
    targetPose(targetPose),
    targetAngle(targetAngle),
    avoidObstacles(avoidObstacles),
    avoidBall(avoidBall),
    velocity_multiplier(default_velocity_multiplier),
    distance_tolerance(default_distance_tolerance),
    angle_tolerance(default_angle_tolerance)
{}


CmdGoToPose::CmdGoToPose(Point targetPose,bool avoidObstacles, bool avoidBall) :
    CmdGoToPose(targetPose,UNUSED_ANGLE_VALUE,avoidObstacles,avoidBall)
{}

CmdGoToPose::CmdGoToPose(CmdGoToPose& cmd) :
    targetPose(cmd.targetPose),
    targetAngle(cmd.targetAngle),
    avoidObstacles(cmd.avoidObstacles),
    avoidBall(cmd.avoidBall),
    velocity_multiplier(cmd.velocity_multiplier),
    distance_tolerance(cmd.distance_tolerance),
    angle_tolerance(cmd.angle_tolerance)
{}


bool CmdGoToPose::hasTargetAngle()  {
    return targetAngle != UNUSED_ANGLE_VALUE;
}

bool CmdGoToPose::completed(Point currentPosition, float currentAngle) {
    return Measurements::isClose(currentPosition, targetPose, distance_tolerance) &&
      (!hasTargetAngle() || Measurements::isClose(currentAngle, targetAngle, angle_tolerance));
}

void CmdGoToPose::setTarget(Point targetPose,float targetAngle){
    this->targetPose = targetPose;
    this->targetAngle = targetAngle;
}
