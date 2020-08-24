#ifndef GOTOPOSE_H
#define GOTOPOSE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "utilities/point.h"

#define UNUSED_ANGLE_VALUE -10

class CmdGoToPose {


public:
    static float default_velocity_multiplier;
    static float default_distance_tolerance;
    static float default_angle_tolerance;


public:
    //! @brief create a 'GoToPose' command given a target position and angle, optionally specify
    //! wheter to avoid obstacles and/or the ball
    CmdGoToPose(Point target_pose, float target_angle,bool avoid_obstacles = true, bool avoid_ball = false);

    //! @brief create a 'GoToPose' command given a target position. Use this function when a
    //! target angle is not required. Optionally specify wheter to avoid obstacles and/or the ball
    CmdGoToPose(Point target_pose,bool avoid_obstacles = true, bool avoid_ball = false);

    CmdGoToPose(CmdGoToPose& cmd);

    void setTarget(Point target_pose,float target_angle = UNUSED_ANGLE_VALUE);
    bool hasTargetAngle();
    bool completed(Point currentPosition, float currentAngle);

    Point target_pose;
    float target_angle;
    bool avoid_obstacles;
    bool avoid_ball;

    float velocity_multiplier = default_velocity_multiplier;

    float distance_tolerance = default_distance_tolerance;
    float angle_tolerance = default_angle_tolerance;


};

#endif // GOTOPOSE_H
